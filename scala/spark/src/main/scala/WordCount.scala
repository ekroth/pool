object WordCount {
  import org.apache.spark.rdd.RDD

  def usingRDD(lines: RDD[String]): RDD[(String, Int)] = {
    lines
      .flatMap(_.split(" "))
      .map(_.toLowerCase)
      .filter(_.nonEmpty)
      .map((_, 1))
      .reduceByKey(_ + _)
  }

}
