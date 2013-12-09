object smp {
  import scala.collection.immutable.Queue

  type Name = String

  case class Man(name: Name, ws: Queue[Name], wife: Option[Name] = None) {
    def next(): (Name, Man) = {
      val (w,nws) = ws.dequeue
      (w, copy(ws = nws))
    }
  }

  case class Woman(name: Name, ms: Map[Name, Int], husband: Option[Man] = None) {
    def propose(m: Man): (Woman, Option[Man]) = {
      val mlevel = ms(m.name)
      val hlevel = husband map { x => ms(x.name) } getOrElse { mlevel + 1 }

      if (mlevel < hlevel) {
        val h = m.copy(wife = Some(name))
        val w = copy(husband = Some(h))
        (w, husband map { _.copy(wife = None) })
      } else (this, Some(m))
    }
  }

  def smp(ms: Queue[Man], wm: Map[Name, Woman]): List[Woman] = {
    if (ms isEmpty) wm.values.toList
    else {
      val (man,nms) = ms.dequeue
      val (wname,tmpman) = man.next
      val (nwom,nman) = wm(wname).propose(tmpman)
      smp(nms ++ nman.toList, wm.updated(nwom.name, nwom))
    }
  }

  def pretty(ws: List[Woman]): String = {
    ws map { w => s"${w.name} -> ${w.husband.get.name}" } mkString ("\n")
  }

  /**
    * Something like this.
    * {{{
    * scala> smp.pretty(smp.smp(smp.Friends.ms, smp.Friends.wm))
    * res6: String = 
    * Monica -> Chandler
    * Phoebe -> Joey
    * Rachel -> Ross
    * }}}
    */
  object Friends {
    val ms = Queue(
      Man("Ross", Queue("Rachel", "Phoebe", "Monica")),
      Man("Chandler", Queue("Monica", "Rachel", "Phoebe")),
      Man("Joey", Queue("Rachel", "Phoebe", "Monica")))

    val wm = Map(
      "Monica" -> Woman("Monica", Map("Chandler" -> 0, "Joey" -> 1, "Ross" -> 2)),
      "Phoebe" -> Woman("Phoebe", Map("Joey" -> 0, "Ross" -> 1, "Chandler" -> 2)),
      "Rachel" -> Woman("Rachel", Map("Ross" -> 0, "Joey" -> 1, "Chandler" -> 2)))
  }

}
