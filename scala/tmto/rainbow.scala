/*
 The MIT License (MIT)

 Copyright (c) 2014 Andrée Ekroth
 */

package object rainbow {
  import scala.util.Random
  import scala.collection.{ GenSeq, GenMap }
  import java.security.{ MessageDigest => MD }

  object Hasher extends Enumeration {
    type Hasher = Value
    val MD2 = Value("MD2")
    val MD5 = Value("MD5")
    val SHA1 = Value("SHA-1")
    val SHA256 = Value("SHA-256")
    val SHA384 = Value("SHA-384")
    val SHA512 = Value("SHA-512")
  }

  case class TMTO(
    height: Int,
    width: Int = 32,
    length: Int = 3,
    hasher: Hasher.Value = Hasher.MD2,
    redHasher: Hasher.Value = Hasher.SHA1,
    chars: Seq[Char] = 'a' until 'z') {

    case class Hash(hash: String) {
      /** Default digester. The damn thing is not thread safe! */
      def rh(): MD = MD.getInstance(redHasher.toString)

      /** Reduce hash to a password.
        * Password length limited by digester pattern.
        * 
        * Digester pattern: NNNN-NNNNNNN-NNNN-NNN
        * - Hash hash with md
        * - Map groups into integer.
        * - Map integer to character index.
        * - Take 'length' groups and turn into String.
        */
      def reduced(depth: Int) = {
        val hasher = rh()
        val digests = Stream.iterate(hash.getBytes("UTF-8")) { hasher.digest(_) }
        val cs = digests(depth) map {
          x => chars(Math.abs(x) % chars.length)
        }

        Pass(cs.take(length).mkString)
      }
    }

    case class Pass(pass: String) {
      def hashed = Hash(digester().digest(pass.getBytes("UTF-8")).mkString)

      def chain: Stream[(Pass, Hash)] = Stream.from(0).scanLeft (this, hashed) {
        case ((_, h), d) => {
          val pass = h.reduced(d)
          (pass, pass.hashed)
        }
      }
    }

    /** Some (not all) password combinations. */
    val passwords: Seq[String] = {
      val repeated = Stream.continually(chars).take(length)
      val combs = repeated.flatten.combinations(length) map { _.mkString }
      combs.toVector
    }

    /** TMTO-table.
      * 
      * - Chosen start values are random.
      * - Chains are calculated in parallel.
      */
    val table: GenMap[Hash, Pass] = (Random.shuffle(passwords).take(height).par map {
      x => {
        val pass = Pass(x)
        pass.chain(width - 1)._2 -> pass
      }
    }).toMap

    /** Default digester. The damn thing is not thread safe! */
    def digester(): MD = MD.getInstance(hasher.toString)

    def crack(hash: Hash): Option[Pass] = {
      /** Search table row for Hash. */
/*      @annotation.tailrec
      def crackrow(pass: Pass, depth: Int): Option[Pass] = pass.hashed match {
        case `hash` => Some(pass)
        case _ if (rs.isEmpty) => None
        case h => crackrow(h reduced rs.head, rs.tail)
      }*/

/*      @annotation.tailrec
      def crack(rss: List[List[MD]]): Option[Pass] = {
        val hlast = hash chained rss.head
        val entry = table get hlast
        val start = entry flatMap { crackrow(_, chain) }

        if (start.isDefined) start
        else if (rss.tail.nonEmpty) crack(rss.tail)
        else None
      }*/

      //crack(chains)
      None
    }
  }

  /** Crack hashed passwords. */
  def test(hs: Seq[String], tmto: TMTO): GenSeq[(tmto.Pass, tmto.Pass)] = hs.par flatMap { x =>
    val pass = tmto.Pass(x)
    tmto.crack(pass.hashed) map { (pass -> _) }
  }

  /** Crack n random passwords. */
  def test(n: Int, tmto: TMTO): GenSeq[(tmto.Pass, tmto.Pass)] = test(Random.shuffle(tmto.passwords).take(n), tmto)
}
