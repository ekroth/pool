/*
 The MIT License (MIT)

 Copyright (c) 2014 Andrée Ekroth
 */

import java.security._

/* Results 2014-03-10
 * Size: 4096 x 4096
 * 
 * scala> (tmto.passwords.take(8192).par flatMap { x => tmto.crack(tmto.Pass(x).hashed(md())) }).length
 * res13: Int = 289
 */
package object tmto {

  /** Valid characters. */
  val ASCII = (97.toChar, 122.toChar)
  val Length = 3

  /** Size of table. */
  val Height = 1024
  val Width = 1024

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
  lazy val table = {
    import scala.util.Random

    val vs = Random.shuffle(passwords.take(Width)).par map {
      x => {
        val pass = Pass(x)
        chain(pass, md())(Width - 1) -> pass
      }
    }

    vs.toMap
  }

  /** Match Hash with Pass. */
  def crack(hash: Hash) = {
    val digest = md()

    /** Search table row for Hash. */
    def crackrow(pass: Pass, end: Hash): Option[Pass] = pass.hashed(digest) match {
      case `hash` => Some(pass)
      case `end` => None
      case h => crackrow(h.reduced, end)
    }

    /** Search hash chain for table entry. */
    def crack(curr: Hash): Option[Pass] = table.get(curr) match {
      case Some(pass) => crackrow(pass, curr)
      case _ => crack(curr.reduced.hashed(digest))
    }

    crack(hash)
  }
}
