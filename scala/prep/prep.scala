object BinarySearch {
  import scala.collection.immutable.Seq
  def findIndex(xs: Seq[Int], x: Int): Option[Int] = {
    var (start, end) = (0, xs.length)
    while (start != end) {
      val length = end - start
      val mid = start + length / 2

      println(s"start: $start end: $end length: $length mid: $mid")

      if (x == xs(mid)) {
        return Some(mid)
      } else if (length == 0) {
        return None
      } else if (x < xs(mid)) {
        end = mid
      } else {
        start = mid
      }
    }

    None
  }
}
