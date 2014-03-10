import java.security._

/* Results 2014-03-10
 scala> (tmto.passwords.take(512) flatMap { x => tmto.crack(tmto.Pass(x).hashed) }).length
 res13: Int = 16
 */

package object tmto {
  lazy val MD = MessageDigest.getInstance("SHA-256")
  val ascii = (97.toChar, 122.toChar)
  val Height = 406
  val Width = 512

  case class Hash(hash: String) {
    /** Reduced hash */
    def reduced = {
      val (start, end) = ascii
      val range = BigInt(end - start)
      val groups = hash split '-' filter { _ != "" }
      val chars = groups map { 
        x => (BigInt(start) + BigInt(x) % range).toChar 
      }

      Pass(chars.take(3).mkString)
    }
  }

  case class Pass(pass: String) {
    /** Hashed password */
    def hashed = Hash(MD.digest(pass.getBytes("UTF-8")).mkString)
  }

  def chain(pass: Pass) = Stream.iterate(pass.hashed) { _.reduced.hashed }

  lazy val passwords = {
    val chars = ascii._1 until ascii._2
    for {
      x <- chars
      y <- chars
      z <- chars
    } yield s"$x$y$z"
  }

  /** TMTO-table */
  lazy val table = {

    assert {
      val hs = passwords map { x => Pass(x) }
      hs.length == hs.distinct.length
    }

    assert { passwords.length >= Height }

    val indices = (0 until Height)
    val vs = indices map {
      x => {
        val pass = Pass(passwords(x))
        chain(pass)(Width - 1) -> pass
      }
    }

    println(vs.mkString("\n"))

    println(vs.length)
    val map = vs.toMap
    println(map.mkString("\n"))
    println(map.size)
    map
  }

  /** Try to crack hash */
  def crack(hash: Hash) = {
    /** Search table row for Hash */
    def crackrow(pass: Pass, end: Hash): Option[Pass] = pass.hashed match {
      case `hash` => Some(pass)
      case `end` => None
      case h => crackrow(h.reduced, end)
    }

    /** Crack */
    def crack(curr: Hash): Option[Pass] = table.get(curr) match {
      case Some(pass) => crackrow(pass, curr)
      case _ => crack(curr.reduced.hashed)
    }

    crack(hash)
  }
}
