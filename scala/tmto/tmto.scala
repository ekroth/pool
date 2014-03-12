/*
 The MIT License (MIT)

 Copyright (c) 2014 Andrée Ekroth
 */

package object tmto {
  import scala.util.Random
  import scala.collection.{ GenSeq, GenMap }
  import java.security.MessageDigest

  case class Hash(hash: String) {
    /** 
      * Reduce hash to a password.
      * Password length limited by digester pattern.
      * 
      * Digester pattern: NNNN-NNNNNNN-NNNN-NNN
      * - Map groups into BigInt.
      * - Map BigInt to character index.
      * - Take 'length' groups and turn into String.
      */
    def reduced(length: Int, chars: Seq[Char]) = {
      val groups = hash split '-' filter { _.nonEmpty }
      val cs = groups map { 
        x => chars((BigInt(x) % chars.length).toInt)
      }

      Pass(cs.take(length).mkString)
    }
  }

  case class Pass(pass: String) {
    def hashed(md: MessageDigest) = Hash(md.digest(pass.getBytes("UTF-8")).mkString)
  }

  case class TMTO(
    width: Int, 
    height: Int, 
    length: Int = 3,
    chars: Seq[Char] = 'a' until 'z',
    hf: String = "MD2") {

    /** Some (not all) password combinations. */
    val passwords: Seq[String] = {
      val repeated = Stream.continually(chars).take(length)
      val combs = repeated.flatten.combinations(length) map { _.mkString }
      combs.toVector
    }

    /**
      * TMTO-table.
      * 
      * - Chosen start values are random.
      * - Chains are calculated in parallel.
      */
    val table: GenMap[Hash, Pass] = (Random.shuffle(passwords).take(height).par map {
      x => {
        val pass = Pass(x)
        val digest = md()
        chain(pass.hashed(digest), digest)(width - 1) -> pass
      }
    }).toMap

    /** Default digester. The damn thing is not thread safe! */
    def md() = MessageDigest.getInstance(hf)

    /** Infinite hash chain. */
    def chain(hash: Hash, md: MessageDigest) = Stream.iterate(hash) { _.reduced(length, chars).hashed(md) }

    /** Match Hash with Pass. */
    def crack(hash: Hash) = {
      val digest = md()

      /** Search table row for Hash. */
      @annotation.tailrec
      def crackrow(pass: Pass, end: Hash): Option[Pass] = pass.hashed(digest) match {
        case `hash` => Some(pass)
        case `end` => None
        case h => crackrow(h.reduced(length, chars), end)
      }

      /** Search hash chain for table entry.
        * 
        * 1. Look for hash in table, otherwise none.
        *    If found, search row, otherswise none.
        * 2. If iterations are less than chain length, continue.
        *    Otherwise, none.
        */
      @annotation.tailrec
      def crack(curr: Hash, depth: Int): Option[Pass] = {
        val entry = table.get(curr) flatMap { crackrow(_, curr) }

        /* not pretty, but tailrec wanted. */
        if (entry.isDefined) entry
        else if (depth < width) crack(curr.reduced(length, chars).hashed(digest), depth + 1)
        else None
      }

      crack(hash, 0)
    }
  }

  /** Crack hashed passwords. */
  def test(hs: Seq[String], tmto: TMTO): GenSeq[Pass] = hs.par flatMap { x =>
    tmto.crack(Pass(x).hashed(tmto.md()))
  }

  /** Crack n random passwords. */
  def test(n: Int, tmto: TMTO): GenSeq[Pass] = test(Random.shuffle(tmto.passwords).take(n), tmto)
}
