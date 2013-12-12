object smp {
  import scala.collection.immutable.Queue

  type Name = String
  type Id = Int

  case class Man(id: Id, ws: Queue[Id], wife: Option[Id] = None) {
    def next(): (Id, Man) = {
      val (w,nws) = ws.dequeue
      (w, copy(ws = nws))
    }

    def out()(implicit names: Seq[Name]) = s"${names(id)} -- ${names(wife.get)}"
  }

  case class Woman(id: Id, ms: Seq[Int], husband: Option[Man] = None) {
    def propFrom(m: Man): (Option[Man], Woman) = {
      val mlevel = ms.indexOf(m.id)
      val hlevel = husband map { x => ms.indexOf(x.id) } getOrElse { mlevel + 1 }

      if (mlevel < hlevel) {
        val h = m.copy(wife = Some(id))
        val w = copy(husband = Some(h))
        (husband map { _.copy(wife = None) }, w)
      } else (Some(m), this)
    }
  }

  def smp(names: Seq[Name], ms: Queue[Man], wm: Seq[Woman]): Seq[Woman] = {
    if (ms.isEmpty) wm
    else {
      val (man, nms) = ms.dequeue
      val (wid, tmpman) = man.next
      val (nman, nwom) = wm(wid) propFrom tmpman
      smp(names, nms ++ nman, wm.updated(nwom.id, nwom))
    }
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
/*    val ms = Queue(
      Man("Ross", Queue("Rachel", "Phoebe", "Monica")),
      Man("Chandler", Queue("Monica", "Rachel", "Phoebe")),
      Man("Joey", Queue("Rachel", "Phoebe", "Monica")))

    val wm = Map(
      "Monica" -> Woman("Monica", Map("Chandler" -> 0, "Joey" -> 1, "Ross" -> 2)),
      "Phoebe" -> Woman("Phoebe", Map("Joey" -> 0, "Ross" -> 1, "Chandler" -> 2)),
      "Rachel" -> Woman("Rachel", Map("Ross" -> 0, "Joey" -> 1, "Chandler" -> 2)))*/
  }

  def read() {
    import scala.io.Source

    // Use Stream.continually! (Immutable!)

    def clean(it: Iterator[String]) = 
      it dropWhile { x => x(0) == '#' || x(0) == '\n' }

    def names(it: Iterator[String]):  = {

    }

    def people(it: Iterator[String])


  }
}
