object collatz {

  def all(n: Int): Set[Long] = {
    val xs = (1 to n).par
    val seqs = xs map { seq(_) }
    seqs reduce { _ ++ _ }
  }

  def seq(n: Long): Set[Long] = {
    @annotation.tailrec def collatzAcc(n: Long, acc: Set[Long]): Set[Long] = {
      n match {
        case 1 => acc + n
        case _ if n % 2 == 0 => collatzAcc(n / 2, acc + n)
        case _ => collatzAcc(n * 3 + 1, acc + n)
      }
    }
    collatzAcc(n, Set.empty)
  }
}
