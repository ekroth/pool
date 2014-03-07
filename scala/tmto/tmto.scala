import java.security._

package object tmto {
  val ascii = (97.toChar, 122.toChar)
  lazy val MD = MessageDigest.getInstance("SHA-1")
  val Height = 15625
  val Width = 1

  case class Hash(hash: String) {
    def reduced = {
      val groups = (hash grouped (hash.length / 3)).toList match {
        case x if (x.length == 3) => x
        case x :: y :: z :: w :: Nil => x :: y :: (z + w) :: Nil
      }

      val chars = groups map {
        x => (ascii._1 + (x.sum % (ascii._2 - ascii._1))).toChar
      }

      Pass(chars.mkString)
    }
  }

  case class Pass(pass: String) {
    def hashed = Hash(MD.digest(pass.getBytes("UTF-8")).mkString)
  }

  def row(start: Pass, size: Int) = {
    val init = (List(start), start.hashed)
    val end = (0 until size).foldLeft(init) { 
      case ((ps, h), _) => {
        val p = h.reduced
        (p :: ps, p.hashed)
      }
    }

    (start, end._2, end._1)
  }

  def matrix = {
    val chars = ascii._1 until ascii._2
    val combs = for {
      x <- chars
      y <- chars
      z <- chars
    } yield s"$x$y$z"


    val indices = (0 until Height) map { _ * (combs.length / Height) }
    (indices map {
      x => {
        val r = row(Pass(combs(x)), Width)
        r._2 -> r._1
      }
    }).toMap
  }

  def crack(table: Map[Hash, Pass], hash: Hash) = {
    val combs = Math.pow(ascii._2 - ascii._1, 3)
    println(s"Combinations: $combs, table: ${Width * Height}")

    def crack(h: Hash, i: Int, row: Boolean): Pass = {
      val pass = if (row) h.reduced else table.getOrElse(h, h.reduced)
      val hazh = pass.hashed

      if (i > Width * Height) Pass("Unknown")
      else if (hazh == hash) pass
      else crack(hazh, i + 1, row || table.contains(h))
    }

    crack(hash, 0, false)
  }
}
