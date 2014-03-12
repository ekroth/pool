/*
 The MIT License (MIT)

 Copyright (c) 2014 Andrée Ekroth
 */

/* Results 2014-03-10
 * Size: 4096 x 4096
 * 
 * scala> (tmto.passwords.take(8192).par flatMap { x => tmto.crack(tmto.Pass(x).hashed(md())) }).length
 * res13: Int = 289
 */
package object tmto {
  import scala.util.Random
  import scala.collection.{ GenSeq, GenMap }
  import java.security.MessageDigest

  type Table = GenMap[Hash, Pass]
  type Reducer = Hash => Pass

  /** Valid characters. */
  val ASCII = ('a', 'z')
  val Length = 3
  val Algorithms = Seq("MD2", "MD5", "SHA-1", "SHA-256", "SHA-384", "SHA-512")

  case class Hash(hash: String) {
    /** 
      * Reduce hash to a password.
      * Password length limited by SHA-256 pattern.
      * 
      * SHA-256 pattern: NNNN-NNNNNNN-NNNN-NNN
      * - Map groups into BigInt.
      * - Map BigInt to character by: ASCII-start + BigInt % ASCII-range.
      * - Take X groups and turn into String.
      */
    def reduced = {
      val (start, end) = ASCII
      val range = BigInt(end - start)
      val groups = hash split '-' filter { _ != "" }
      val chars = groups map { 
        x => (BigInt(start) + BigInt(x) % range).toChar 
      }

      Pass(chars.take(Length).mkString)
    }
  }

  case class Pass(pass: String) {
    def hashed(md: MessageDigest) = Hash(md.digest(pass.getBytes("UTF-8")).mkString)
  }

  /** Default digester. */
  def md() = MessageDigest.getInstance("SHA-256")

  /** Infinite hash chain. */
  def chain(pass: Pass, md: MessageDigest) = Stream.iterate(pass.hashed(md)) { _.reduced.hashed(md) }

  /** All password combinations. */ 
  lazy val passwords = {
    val chars = ASCII._1 until ASCII._2
    for {
      x <- chars
      y <- chars
      z <- chars
    } yield s"$x$y$z"
  }

  /**
    * TMTO-table.
    * 
    * - Chosen start values are random.
    * - Chains are calculated in parallel.
    */
  def table(width: Int, height: Int): Table = (Random.shuffle(passwords).take(height).par map {
    x => {
      val pass = Pass(x)
      chain(pass, md())(width - 1) -> pass
    }
  }).toMap

  /** Match Hash with Pass. */
  def crack(table: Table, hash: Hash, width: Int) = {
    val digest = md()

    /** Search table row for Hash. */
    @annotation.tailrec 
    def crackrow(pass: Pass, end: Hash): Option[Pass] = pass.hashed(digest) match {
      case `hash` => Some(pass)
      case `end` => None
      case h => crackrow(h.reduced, end)
    }

    /** Search hash chain for table entry. */
    @annotation.tailrec
    def crack(curr: Hash, depth: Int): Option[Pass] = {
      val entry = table.get(curr) flatMap { crackrow(_, curr) }
      if (entry.isDefined) entry
      else if (depth < width) crack(curr.reduced.hashed(digest), depth + 1)
      else None
    }

    crack(hash, 0)
  }

  /** Crack hashed passwords. */
  def test(hs: Seq[String], table: Table, width: Int): GenSeq[Pass] = hs.par flatMap { x =>
    tmto.crack(table, tmto.Pass(x).hashed(md()), width)
  }

  /** Crack n random passwords. */
  def test(n: Int, table: Table, width: Int): GenSeq[Pass] = test(Random.shuffle(passwords).take(n), table, width)
}
