import com.holdenkarau.spark.testing._
import org.scalatest._
import org.scalatest.prop._

import scala.collection.immutable.Stream
import scala.util.Random

class WordCountTest extends PropSpec with
    PropertyChecks with Matchers with SharedSparkContext {
  import org.scalacheck.Gen

  /* Generator for non-empty String or newline to int */
  val wordMapGen: Gen[Map[String, Int]] =
    for {
      map <- Gen.mapOf(
        Gen.zip(
          Gen.alphaStr
            .suchThat(_.nonEmpty)
            .map(_.toLowerCase),
          Gen.choose(1, 10)))
      ns <- Gen.zip("\n", Gen.choose(1, 10))
    } yield map + ns

  /* Repeat each entry string by its value and flatten */
  def mapToList(map: Map[String, Int]): Seq[String] =
    map.toSeq.flatMap {
      case (w, n) => Stream.continually(w).take(n)
    }

  property("scalachecked string") {
    forAll(wordMapGen) { map =>
      val xs = Random.shuffle(mapToList(map))

      val lines = xs
        .mkString(" ")
        .split("\n")

      val out = WordCount.usingRDD(
        sc.parallelize(lines)
      ).collectAsMap

      out should be(map - "\n")
    }
  }
}
