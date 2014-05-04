object Main {
  /** stream of pascal triangle rows */
  def pascal: Stream[List[Int]] = List(1) #:: Stream.iterate(List(1,1)) { xs =>
    val sums = xs.sliding(2) map { _.sum }
    1 :: (sums.toList :+ 1)
  }
}
