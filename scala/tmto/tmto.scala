import java.security._

package object tmto {
  lazy val MD = MessageDigest.getInstance("MD5")
  val ascii = (65.toChar, 122.toChar)
  val Height = 128
  val Width = 128

  case class Hash(hash: String) {
    /** Reduced hash */
    def reduced = {
      val groups = (hash grouped (hash.length / 2)).toList match {
        case x if (x.length == 2) => x
        case x :: y :: z :: Nil => x :: (y + z) :: Nil
      }

      val chars = groups map {
        x => (ascii._1 + (x.mkString.hashCode % (ascii._2 - ascii._1))).toChar
      }

      Pass(chars.mkString)
    }
  }

  case class Pass(pass: String) {
    /** Hashed password */
    def hashed = Hash(MD.digest(pass.getBytes("UTF-8")).mkString)
  }

  /** TMTO-table */
  lazy val table = {
    val passwords = {
      val chars = ascii._1 until ascii._2
      for {
        x <- chars
        y <- chars
      } yield s"$x$y"
    }

    assert {
      val hs = passwords map { x => Pass(x) }
      hs.length == hs.distinct.length
    }

    assert { passwords.length >= Height }

    val indices = (0 until Height) map { _ * (passwords.length / Height) }
    val vs = indices map {
      x => {
        val pass = Pass(passwords(x))
        val chain = Stream.iterate(pass.hashed) { _.reduced.hashed }
        chain(Width - 1) -> pass
      }
    }

    println(vs.mkString("\n"))

    println(vs.length)
    val map = vs.toMap
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
