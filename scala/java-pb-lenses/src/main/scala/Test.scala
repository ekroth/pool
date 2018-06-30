import javaapi.Test.{Address => SAddress}
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
    def companion: GeneratedMessageCompanion[M] with BuilderSupport[M, _]
  }

  abstract class RichMessageWithBuilder[M <: Message, B <: Message.Builder](underlying: M) extends RichMessage[M](underlying) {

    def update(ms: (Lens[B, B] => Mutation[B])*): M =
      ms.foldLeft[B](companion.newBuilder(underlying))((p, m) => m(Lens.unit[B])(p)).build().asInstanceOf[M]

    def companion: GeneratedMessageCompanion[M] with BuilderSupport[M, B]
  }

  trait ScalaProtoSupport[JavaPB] {
    type ScalaPB <: SGeneratedMessage with SMessage[ScalaPB]

    def javaProtoSupport: JavaProtoSupport[ScalaPB, JavaPB]

    def fromScalaProto(scalaProto: ScalaPB): JavaPB = javaProtoSupport.toJavaProto(scalaProto)

    def toScalaProto(javaProto: JavaPB): ScalaPB = javaProtoSupport.fromJavaProto(javaProto)
  }

  abstract class GeneratedMessageCompanion[M <: Message] {
    type ScalaPB <: SGeneratedMessage with SMessage[ScalaPB]
    def parser: Parser[M]
    def defaultInstance: M
    def descriptor: Descriptor
    def scalaCompanion: SGeneratedMessageCompanion[ScalaPB]
  }

  trait BuilderSupport[-M <: Message, +B <: Message.Builder] {
    self: GeneratedMessageCompanion[_ <: Message] =>

    def newBuilder: B
    def newBuilder(prototype: M): B
  }

  implicit def RichMessage[M <: Message](underlying: M)(implicit support: ScalaProtoSupport[M]): support.ScalaPB = support.toScalaProto(underlying)
}

import Lib._

object Utils {
  def default[M <: Message : GeneratedMessageCompanion]: M = implicitly[GeneratedMessageCompanion[M]].defaultInstance
  //def defaultAsScala[M <: Message](implicit scalaProtoSupport: GeneratedMessageCompanion[M] with ScalaProtoSupport[M, GeneratedMessageCompanion[M]#ScalaPB]): GeneratedMessageCompanion[M]#ScalaPB = {
  //  scalaProtoSupport.toScalaProto(scalaProtoSupport.defaultInstance)
 //
  //
  // def}

  def scalaDefault[M <: Message : GeneratedMessageCompanion](implicit support: ScalaProtoSupport[M]): support.ScalaPB = {
    val companion = implicitly[GeneratedMessageCompanion[M]]
    support.toScalaProto(companion.defaultInstance)
  }
}



/**
  * Created by ekroth on 30/06/18.
  */
object Test2 {

  // ADDRESS
  
  implicit class AddressBuilderLens[UpperPB](_l: Lens[UpperPB, Address.Builder]) extends ObjectLens[UpperPB, Address.Builder](_l) {
    def country: Lens[UpperPB, String] = field(_.getCountry)((c_, f_) => c_.setCountry(f_))
  }
  
  implicit class AddressRich(underlying: Address) extends RichMessageWithBuilder[Address, Address.Builder](underlying) {
    override def companion: GeneratedMessageCompanion[Address] with BuilderSupport[Address, Address.Builder] = AddressCompanion
  }

  object AddressCompanion extends GeneratedMessageCompanion[Address]
    with BuilderSupport[Address, Address.Builder]
    with ScalaProtoSupport[Address]
  {
    override type ScalaPB = SAddress

    implicit def messageCompanion: this.type = this

    override def parser: Parser[Address] = Address.parser()

    override def defaultInstance: Address = Address.getDefaultInstance

    override def descriptor: Descriptor = Address.getDescriptor

    override def javaProtoSupport: JavaProtoSupport[SAddress, Address] = SAddress

    override def scalaCompanion: SGeneratedMessageCompanion[SAddress] = SAddress

    override def newBuilder: Address.Builder = Address.newBuilder()

    override def newBuilder(prototype: Address): Address.Builder = Address.newBuilder(prototype)
  }


  // PERSON

  
/*  implicit class PersonBuilderLens[UpperPB](_l: Lens[UpperPB, Person.Builder]) extends ObjectLens[UpperPB, Person.Builder](_l) {
    def primaryAddress = field(_.getPrimaryAddressBuilder)((c_, f_) => c_.setPrimaryAddress(f_))
    def age = field(_.getAge)((c_, f_) => c_.setAge(f_))
  }
  
  implicit class PersonRich(underlying: Person) extends RichMessage[Person, Person.Builder](underlying)
*/

  import AddressCompanion.messageCompanion

  val x = Utils.default[Address].update(
    _.country := "country"
  )

 /* val y = Person.getDefaultInstance.update(
    _.primaryAddress.country := "innerCountry",
    _.age := 1337
  )*/


  def main(args: Array[String]): Unit = {
    import AddressCompanion.messageCompanion

    println(Utils.default[Address])
    println(Utils.scalaDefault[Address])
    println("wooop")
    //println(y)
  }
}
