object collatz {
  import scala.collection.GenSeq

  def all(n: Int): GenSeq[Long] = {
    val xs = (1 to n).par
    val combs = xs flatMap { seq(_) }
    combs.distinct
  }

  def seq(n: Long): Seq[Long] = {
    @annotation.tailrec def collatzAcc(n: Long, acc: Seq[Long]): Seq[Long] = {
      n match {
        case 1 => acc :+ n
        case _ if n % 2 == 0 => collatzAcc(n / 2, acc :+ n)
        case _ => collatzAcc(n * 3 + 1, acc :+ n)
      }
    }
    collatzAcc(n, Seq.empty)
  }
}
