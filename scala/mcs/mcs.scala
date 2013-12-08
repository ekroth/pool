// http://cs.lth.se/english/course/edan40_functional_programming/programming_assignments/mcsp/

object mcsSlow {
  def mcsLength[A](xs: List[A], ys: List[A]): Int = {
    import scala.math.max
      (xs, ys) match {
      case (_, Nil) => 0
      case (Nil, _) => 0
      case (xz@(x :: xs), yz@(y :: ys)) => {
        if (x == y) 1 + mcsLength(xs, ys)
        else max(mcsLength(xs, yz), mcsLength(xz, ys))
      }
    }
  }
}


/** Not fast (tuples and so on), but using lazy and fully immutable.
  * Would have been faster using an Array or similar.
  * 
  * {{{
  * scala> mcs2(10 until 900 toList, (60 to 100 toList) ++ (500 to 1000 toList)).result
  * res5: Int = 542
  * 
  * scala> mcs2("328239439".toList, "132379".toList).result
  * res4: Int = 4
  * }}}
  */
case class mcs2[+A](xs: List[A], ys: List[A]) {
  import scala.math.max
  case class Entry(i: Int, j: Int) {
    def apply() = value
    lazy val value: Int = {
      (i, j) match {
        case (_, 0) => 0
        case (0, _) => 0
        case (i, j) => {
          if (xs(i-1) == ys(j-1)) 1 + mcsMap((i-1) -> (j-1))()
          else max(mcsMap((i-1) -> j)(), mcsMap(i -> (j-1))())
        }
      }
    }
  }

  lazy val mcsMap =
    (for {
      i <- 0 to xs.length
      j <- 0 to ys.length
    } yield {
      ((i,j),Entry(i,j))
    }).toMap

  lazy val result = mcsMap(xs.length -> ys.length)()
}
