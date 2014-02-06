package client

import com.typesafe.config._
import akka.actor._

import common._

class Nurse extends Actor {
  val hospital = context.actorSelection("akka.ssl.tcp://eit060s@127.0.0.1:2552/user/hospital")
  hospital ! s"Hello hospital! Best regards, ${self.path}"
  hospital ! Ping(2)

  def receive = {
    case x: String  => println(s"Thank you for '$x', ${sender().path}")
  }
}

object NurseApp extends App {
  // we can load override settings. i.e. we can supply custom keystore!
  val sslconfig = ConfigFactory.parseString("""akka.remote.netty.ssl.security.key-store = "clientkeystore"""")
  val config = ConfigFactory.load
  val comb = ConfigFactory.load(sslconfig withFallback config)
  val system = ActorSystem.create("eit060c", comb)
  val nurse = system.actorOf(Props[Nurse], name = "nurse")
}
