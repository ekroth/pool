import com.google.protobuf.Descriptors.{Descriptor, EnumDescriptor, EnumValueDescriptor}
import com.google.protobuf.{Message, MessageOrBuilder, Parser, ProtocolMessageEnum}
import javaapi.JavaAPI._
import javaapi.Test.{Address => SAddress, Gender => SGender, Person => SPerson}
import scalapb.lenses.{Lens, Mutation}
import scalapb.{JavaProtoSupport, GeneratedEnum => SGeneratedEnum, GeneratedEnumCompanion => SGeneratedEnumCompanion, GeneratedMessage => SGeneratedMessage, GeneratedMessageCompanion => SGeneratedMessageCompanion, Message => SMessage}

import scala.collection.JavaConverters._
import scala.util.Try

object Lib {

  trait JavaProtoConverters[JavaPB] extends Any {
    type ScalaPB
    def javaProtoSupport: JavaProtoSupport[ScalaPB, JavaPB]
    def fromScalaProto(scalaProto: ScalaPB): JavaPB = javaProtoSupport.toJavaProto(scalaProto)
    def toScalaProto(javaProto: JavaPB): ScalaPB = javaProtoSupport.fromJavaProto(javaProto)
  }

  trait RichEnum[E <: ProtocolMessageEnum] extends Any {
    def companion: EnumCompanion[E]
  }

  trait EnumCompanion[E <: ProtocolMessageEnum] extends Any with JavaProtoConverters[E] {
    type ScalaPB <: SGeneratedEnum
    type ScalaCompanion <: SGeneratedEnumCompanion[ScalaPB]

    def descriptor: EnumDescriptor
    def forNumber(i: Int): Option[E]
    def valueOf(str: String): Option[E]
    def valueOf(enumValueDescriptor: EnumValueDescriptor): Option[E]
    def values: Seq[E]

    def scalaCompanion: ScalaCompanion
    def javaProtoSupport: JavaProtoSupport[ScalaPB, E]
  }

  trait RichMessage[M <: Message] extends Any {
    type B <: Message.Builder
    def underlying: M
    def companion: MessageCompanion[M]
    def update(ms: BuilderOps.Updater[B]*): M
  }

  trait RichMessageOrBuilder[M <: Message, OrB <: MessageOrBuilder] extends Any {
    def underlying: OrB
    def companion: MessageCompanion[M]
  }

  trait RichMessageBuilder[M <: Message, B <: Message.Builder] extends Any {
    def underlying: B
    def companion: MessageCompanion[M]
    def update(ms: BuilderOps.Updater[B]*): B
  }

  trait MessageCompanion[M <: Message] extends Any with JavaProtoConverters[M] {
    type ScalaPB <: SGeneratedMessage with SMessage[ScalaPB]
    type ScalaCompanion <: SGeneratedMessageCompanion[ScalaPB]
    def parser: Parser[M]
    def defaultInstance: M
    def descriptor: Descriptor
    def scalaCompanion: ScalaCompanion
    def javaProtoSupport: JavaProtoSupport[ScalaPB, M]
  }

  trait BuilderSupport[M <: Message] extends Any {
    type Builder <: Message.Builder

    def newBuilder: Builder
    def newBuilder(prototype: M): Builder
  }

  object Experiment {
    // In order for Scala to understand the type member we need to
    // keep the [[support.Builder] in the method signature
    // Sadly this means AnyVal can't be used
    def messageToBuilderOps[M <: Message](message: M)(implicit support: BuilderSupport[M]): BuilderOps[M, support.Builder] =
    new BuilderOps[M, support.Builder](message, support)

    // Some ugly but probably safe casts required..
    class BuilderOps[M <: Message, B <: Message.Builder](val m: M, support: BuilderSupport[M]) {
      def updateGeneric(ms: BuilderOps.Updater[B]*): M =
        BuilderOps.update[M](m, support)(ms.asInstanceOf[Seq[Nothing]]).build().asInstanceOf[M]
    }
  }

  object BuilderOps {
    type Updater[B] = Lens[B, B] => Mutation[B]
    def update[M <: Message](m: M, support: BuilderSupport[M])(ms: Seq[Updater[support.Builder]]): support.Builder = {
      val builder = support.newBuilder(m)
      ms.foreach(_(Lens.unit[support.Builder])(builder))
      builder
    }

    def update[B <: Message.Builder](builder: B)(ms: Seq[Updater[B]]): B = {
      ms.foreach(_(Lens.unit[B])(builder))
      builder
    }
  }

  object ScalaConverters {
    implicit class MessageScalaConversions[M](val m: M) extends AnyVal {
      def toScala(implicit companion: JavaProtoConverters[M]): companion.ScalaPB = companion.toScalaProto(m)
    }

    implicit class MessageOptionScalaConversions[M](val m: Option[M]) extends AnyVal {
      def toScala(implicit companion: JavaProtoConverters[M]): Option[companion.ScalaPB] = m.map(companion.toScalaProto(_))
    }

    implicit class MessageSeqScalaConversions[M](val ms: Seq[M]) extends AnyVal {
      def toScala(implicit converter: JavaProtoConverters[M]): Seq[converter.ScalaPB] = ms.map(converter.toScalaProto(_))
    }
  }

  trait ObjectLens[U, Container] extends Any with Lens[U, Container] {
    def self: Lens[U, Container]
    def field[A](lens: Lens[Container, A]): Lens[U, A] = self.compose(lens)
    def field[A](getter: Container => A)(setter: (Container, A) => Container): Lens[U, A] = field(Lens(getter)(setter))
    override def get(u: U): Container = self.get(u)
    override def set(c: Container): Mutation[U] = self.set(c)
    def update(ms: (Lens[Container, Container] => Mutation[Container])*): Mutation[U] =
      u => set(ms.foldLeft[Container](get(u))((p, m) => m(Lens.unit[Container])(p)))(u)
  }
}

import Lib._

object Utils {
  def default[M <: Message : MessageCompanion]: M = implicitly[MessageCompanion[M]].defaultInstance

  def scalaDefault[M <: Message](implicit companion: MessageCompanion[M]): companion.ScalaPB = {
    companion.toScalaProto(companion.defaultInstance)
  }

  def scalaCompanion[M <: Message](implicit companion: MessageCompanion[M]): companion.ScalaCompanion = companion.scalaCompanion

  def enumValue[E <: ProtocolMessageEnum : EnumCompanion](str: String): Option[E] =
    implicitly[EnumCompanion[E]].valueOf(str)
}



/**
  * Created by ekroth on 30/06/18.
  */
object Test2 {

  // Gender

  implicit class GenderRich(val underlying: Gender) extends AnyVal with RichEnum[Gender] {
    override def companion: EnumCompanion[Gender] = GenderCompanion
  }

  object GenderCompanion extends EnumCompanion[Gender] {
    override type ScalaPB = SGender
    override type ScalaCompanion = SGender.type

    override def descriptor: EnumDescriptor = Gender.getDescriptor
    override def forNumber(i: Int): Option[Gender] = Option(Gender.forNumber(i))
    override def valueOf(str: String): Option[Gender] = Try(Gender.valueOf(str)).toOption
    override def valueOf(enumValueDescriptor: EnumValueDescriptor): Option[Gender] = Try(Gender.valueOf(enumValueDescriptor)).toOption
    override def values: Seq[Gender] = Gender.values()
    override def scalaCompanion: ScalaCompanion = SGender
    private object Support extends JavaProtoSupport[ScalaPB, Gender] {
      override def fromJavaProto(javaProto: Gender): SGender = SGender.fromJavaValue(javaProto)
      override def toJavaProto(scalaProto: SGender): Gender = SGender.toJavaValue(scalaProto)
    }
    override def javaProtoSupport: JavaProtoSupport[ScalaPB, Gender] = Support
  }

  implicit def genderCompanion: GenderCompanion.type = GenderCompanion


  // ADDRESS

  implicit class AddressBuilderLens[UpperPB](val self: Lens[UpperPB, Address.Builder]) extends AnyVal with ObjectLens[UpperPB, Address.Builder] {
    def country: Lens[UpperPB, String] = field(_.getCountry)((c_, f_) => c_.setCountry(f_))
    def street: Lens[UpperPB, Seq[String]] = field[Seq[String]]((v: Address.Builder) => v.getStreetList.asScala)((c_, f_) => c_.clearStreet().addAllStreet(f_.asJava))
  }

  implicit class AddressOrBuilderRich(val underlying: AddressOrBuilder) extends AnyVal with RichMessageOrBuilder[Address, AddressOrBuilder] {
    override def companion: AddressCompanion.type = AddressCompanion
    def street: Seq[String] = underlying.getStreetList.asScala
  }

  implicit class AddressRich(val underlying: Address) extends AnyVal with RichMessage[Address] {
    type B = Address.Builder
    override def companion: AddressCompanion.type = AddressCompanion
    def update(ms: BuilderOps.Updater[Address.Builder]*): Address = BuilderOps.update(underlying, companion)(ms).build()
  }

  implicit class AddressBuilderRich(val underlying: Address.Builder) extends AnyVal with RichMessageBuilder[Address, Address.Builder] {
    override def companion: AddressCompanion.type = AddressCompanion
    def update(ms: BuilderOps.Updater[Address.Builder]*): Address.Builder = BuilderOps.update(underlying)(ms)
  }

  object AddressCompanion extends MessageCompanion[Address]
    with BuilderSupport[Address]
  {
    override type Builder = Address.Builder
    override type ScalaPB = SAddress
    override type ScalaCompanion = SGeneratedMessageCompanion[ScalaPB]

    override def parser: Parser[Address] = Address.parser()
    override def defaultInstance: Address = Address.getDefaultInstance
    override def descriptor: Descriptor = Address.getDescriptor
    override def javaProtoSupport: JavaProtoSupport[SAddress, Address] = SAddress
    override def scalaCompanion: ScalaCompanion = SAddress
    override def newBuilder: Address.Builder = Address.newBuilder()
    override def newBuilder(prototype: Address): Address.Builder = Address.newBuilder(prototype)
  }

  implicit def addressCompanion: AddressCompanion.type = AddressCompanion


  // PERSON

  implicit class PersonRich(val underlying: Person) extends AnyVal with RichMessage[Person] {
    type B = Person.Builder
    override def companion: PersonCompanion.type = PersonCompanion
    def update(ms: BuilderOps.Updater[Person.Builder]*): Person = BuilderOps.update(underlying, companion)(ms).build()
    def primaryAddress = if (underlying.hasPrimaryAddress) Some(underlying.getPrimaryAddress) else None
    def gender = if (underlying.hasGender) Some(underlying.getGender) else None
  }

  implicit class PersonBuilderRich(val underlying: Person.Builder) extends AnyVal with RichMessageBuilder[Person, Person.Builder] {
    override def companion: PersonCompanion.type = PersonCompanion
    def update(ms: BuilderOps.Updater[Person.Builder]*): Person.Builder = BuilderOps.update(underlying)(ms)
    def primaryAddressBuilder = if (underlying.hasPrimaryAddress) Some(underlying.getPrimaryAddressBuilder) else None
  }

  implicit class PersonBuilderLens[UpperPB](val self: Lens[UpperPB, Person.Builder]) extends AnyVal with ObjectLens[UpperPB, Person.Builder] {
    def primaryAddress = field(_.getPrimaryAddressBuilder)((c_, f_) => c_.setPrimaryAddress(f_))
    def optionalPrimaryAddress = field(_.primaryAddressBuilder)((c_, f_) => if (f_.isDefined) c_.setPrimaryAddress(f_.get) else c_.clearPrimaryAddress())
    def age = field(_.getAge)((c_, f_) => c_.setAge(f_))
    def gender = field(_.getGender)((c_, f_) => c_.setGender(f_))
  }

  object PersonCompanion extends MessageCompanion[Person]
    with BuilderSupport[Person]
  {
    override type Builder = Person.Builder
    override type ScalaPB = SPerson
    override type ScalaCompanion = SGeneratedMessageCompanion[ScalaPB]

    override def parser: Parser[Person] = Person.parser()
    override def defaultInstance: Person = Person.getDefaultInstance
    override def descriptor: Descriptor = Person.getDescriptor
    override def javaProtoSupport: JavaProtoSupport[SPerson, Person] = SPerson
    override def scalaCompanion: ScalaCompanion = SPerson
    override def newBuilder: Person.Builder = Person.newBuilder()
    override def newBuilder(prototype: Person): Person.Builder = Person.newBuilder(prototype)
  }

  implicit def personCompanion: PersonCompanion.type = PersonCompanion


  import Utils._

  val x = Utils.default[Address].update(
    _.country := "country",
    _.street := Seq("hello", "and-welcome"),
    _.street :+= "again",
    _.street := Seq.empty
  )

  val y = Experiment.messageToBuilderOps(x).updateGeneric(
    _.country := "country"
  )

  val z: SAddress = scalaCompanion[Address].defaultInstance

  val wat = Person.getDefaultInstance.update(
    _.primaryAddress.country := "innerCountry",
    _.age := 1337,
    _.optionalPrimaryAddress := None,
    _.gender := Gender.Female
  )


  def main(args: Array[String]): Unit = {
    import ScalaConverters._
    import scala.collection.JavaConverters._

    println(x)
    println(x.toScala)
    println(x.street)
    println(wat.gender.toScala)
    println(x.getStreetList.asScala)
    println(Utils.default[Address])
    println(Utils.scalaDefault[Address].copy(country = Some("ctuy")))
    println("wooop")
    println(enumValue[Gender]("Male"))
    val genderScala: SGender = Gender.Male.toScala
    val addressScala: SAddress = x.toScala
    println(y)
    println(wat.toScala)
  }
}
