package server

import akka.actor._

import common._

class Hospital extends Actor {
  def receive = {
    case msg: String => {
      println(s"Hospital got: '$msg'")
      sender() ! s"Welcome, ${sender().path}!"
    }

    case Ping(x) => println(s"Got ping'd nr $x")
  }
}

object ServerApp extends App {
  val system = ActorSystem("eit060s")
  val srv = system.actorOf(Props[Hospital], name = "hospital")
}
