import javaapi.Test.{Address => SAddress, Person => SPerson}
import javaapi.JavaAPI._

import com.google.protobuf.AbstractMessage.Builder
import com.google.protobuf.Descriptors.Descriptor
import com.google.protobuf._

import scalapb.{JavaProtoSupport, GeneratedMessage => SGeneratedMessage, GeneratedMessageCompanion => SGeneratedMessageCompanion, Message => SMessage}
import scalapb.lenses._

object Lib {
  implicit class UpdatableBuilder[M <: Message.Builder](val self: M) {
    def update(ms: (Lens[M, M] => Mutation[M])*): M = ms.foldLeft[M](self)((p, m) => m(Lens.unit[M])(p))
  }

  abstract class RichMessage[M <: Message](underlying: M) {
    def companion: GeneratedMessageCompanion[M]
  }

  abstract class RichMessageWithBuilder[M <: Message](underlying: M) extends RichMessage[M](underlying) {
    def companion: GeneratedMessageCompanion[M]
  }

  abstract class GeneratedMessageCompanion[M <: Message] {
    type ScalaPB <: SGeneratedMessage with SMessage[ScalaPB]
    type ScalaCompanion <: SGeneratedMessageCompanion[ScalaPB]
    def parser: Parser[M]
    def defaultInstance: M
    def descriptor: Descriptor
    def scalaCompanion: ScalaCompanion
    def javaProtoSupport: JavaProtoSupport[ScalaPB, M]
    def fromScalaProto(scalaProto: ScalaPB): M = javaProtoSupport.toJavaProto(scalaProto)
    def toScalaProto(javaProto: M): ScalaPB = javaProtoSupport.fromJavaProto(javaProto)
  }

  trait BaseBuilderSupport[-M <: Message] {
    type Builder <: Message.Builder

    def newBuilder: Builder
    def newBuilder(prototype: M): Builder
  }

  trait BuilderSupport[-M <: Message, B <: Message.Builder] extends BaseBuilderSupport[M] {
    override type Builder = B
  }

  // In order for Scala to understand the type member we need to
  // keep the [[support.Builder] in the method signature
  // Sadly this means AnyVal can't be used
  def messageToBuilderOps[M <: Message](message: M)(implicit support: BaseBuilderSupport[M]): BuilderOps[M, support.Builder] =
    new BuilderOps[M, support.Builder](message, support)

  // Some ugly but probably safe casts required..
  class BuilderOps[M <: Message, B <: Message.Builder](val m: M, support: BaseBuilderSupport[M]) {
    def updateGeneric(ms: (BuilderOps.Updater[B])*): M =
      BuilderOps.update[M](m, support)(ms.asInstanceOf[Seq[Nothing]]).build().asInstanceOf[M]
  }

  object BuilderOps {
    type Updater[B] = Lens[B, B] => Mutation[B]
    def update[M <: Message](m: M, support: BaseBuilderSupport[M])
                            (ms: Seq[Updater[support.Builder]]): support.Builder = {
      val builder = support.newBuilder(m)
      ms.foreach(_(Lens.unit[support.Builder])(builder))
      builder
    }
  }

  implicit class ScalaConversionOps[M <: Message](val m: M) extends AnyVal {
    def asScala(implicit companion: GeneratedMessageCompanion[M]): companion.ScalaPB = companion.toScalaProto(m)
  }
}

import Lib._

object Utils {
  def default[M <: Message : GeneratedMessageCompanion]: M = implicitly[GeneratedMessageCompanion[M]].defaultInstance
  //def defaultAsScala[M <: Message](implicit scalaProtoSupport: GeneratedMessageCompanion[M] with ScalaProtoSupport[M, GeneratedMessageCompanion[M]#ScalaPB]): GeneratedMessageCompanion[M]#ScalaPB = {
  //  scalaProtoSupport.toScalaProto(scalaProtoSupport.defaultInstance)
 //
  //
  // def}

  def scalaDefault[M <: Message](implicit companion: GeneratedMessageCompanion[M]): companion.ScalaPB = {
    companion.toScalaProto(companion.defaultInstance)
  }

  def scalaCompanion[M <: Message](implicit companion: GeneratedMessageCompanion[M]): companion.ScalaCompanion = companion.scalaCompanion
}



/**
  * Created by ekroth on 30/06/18.
  */
object Test2 {

  // ADDRESS
  
  implicit class AddressBuilderLens[UpperPB](_l: Lens[UpperPB, Address.Builder]) extends ObjectLens[UpperPB, Address.Builder](_l) {
    def country: Lens[UpperPB, String] = field(_.getCountry)((c_, f_) => c_.setCountry(f_))
  }
  
  implicit class AddressRich(underlying: Address) extends RichMessageWithBuilder[Address](underlying) {
    override val companion: AddressCompanion.type = AddressCompanion
    def update(ms: (BuilderOps.Updater[Address.Builder])*): Address =
      BuilderOps.update[Address](underlying, companion)(ms).build()
  }

  object AddressCompanion extends GeneratedMessageCompanion[Address]
    with BuilderSupport[Address, Address.Builder]
  {
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

  
  implicit class PersonBuilderLens[UpperPB](_l: Lens[UpperPB, Person.Builder]) extends ObjectLens[UpperPB, Person.Builder](_l) {
    def primaryAddress = field(_.getPrimaryAddressBuilder)((c_, f_) => c_.setPrimaryAddress(f_))
    def age = field(_.getAge)((c_, f_) => c_.setAge(f_))
  }
  
  implicit class PersonRich(underlying: Person) extends RichMessage[Person](underlying) {
    override val companion: PersonCompanion.type = PersonCompanion
    def update(ms: (BuilderOps.Updater[Person.Builder])*): Person =
      BuilderOps.update[Person](underlying, companion)(ms).build()
  }

  object PersonCompanion extends GeneratedMessageCompanion[Person]
    with BuilderSupport[Person, Person.Builder]
  {
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
    _.country := "country"
  )

  val y = messageToBuilderOps(x).updateGeneric(
    _.country := "country"
  )

  val z: SAddress = scalaCompanion[Address].defaultInstance

  val wat = Person.getDefaultInstance.update(
    _.primaryAddress.country := "innerCountry",
    _.age := 1337
  )


  def main(args: Array[String]): Unit = {
    println(x)
    println(x.asScala)
    println(Utils.default[Address])
    println(Utils.scalaDefault[Address].copy(country = Some("ctuy")))
    println("wooop")
    println(y)
    println(wat.asScala)
  }
}
