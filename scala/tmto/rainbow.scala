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
    length: Int = 3,
    hasher: Hasher.Value = Hasher.MD2,
    chars: Seq[Char] = 'a' until 'z') {

    case class Hash(hash: String) {
      /** Reduce hash to a password.
        * Password length limited by digester pattern.
        * 
        * Digester pattern: NNNN-NNNNNNN-NNNN-NNN
        * - Hash hash with md
        * - Map groups into integer.
        * - Map integer to character index.
        * - Take 'length' groups and turn into String.
        */
      def reduced(md: MD) = {
        val hash2 = md.digest(hash.getBytes("UTF-8")).mkString
        val groups = hash2 split '-' filter { _.nonEmpty }
        val cs = groups map {
          x => chars((BigInt(x) % chars.length).toInt)
        }

        Pass(cs.take(length).mkString)
      }

      /** Hash chain this Hash using `hs` digester as reducers. */
      def chains(hs: List[MD]): List[Hash] = hs.scanLeft(this) { 
        case (h, md) => h.reduced(md).hashed
      }

      /** Last hash using `hs` digester as reducers. */
      def chained(hs: List[MD]): Hash = chains(hs).last
    }

    case class Pass(pass: String) {
      def hashed = Hash(digester().digest(pass.getBytes("UTF-8")).mkString)
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
        pass.hashed.chained(mds()) -> pass
      }
    }).toMap

    /** Default digester. The damn thing is not thread safe! */
    def digester(): MD = MD.getInstance(hasher.toString)

    /** Digesters. The damn thing is not thread safe! */
    def mds() = Stream.continually(MD.getInstance("SHA-1")).take(6).toList

    def crack(hash: Hash): Option[Pass] = {
      val chains = mds().tails.toList.reverse
      val chain = mds()

      /** Search table row for Hash. */
      @annotation.tailrec
      def crackrow(pass: Pass, rs: List[MD]): Option[Pass] = pass.hashed match {
        case `hash` => Some(pass)
        case _ if (rs.isEmpty) => None
        case h => crackrow(h reduced rs.head, rs.tail)
      }

      @annotation.tailrec
      def crack(rss: List[List[MD]]): Option[Pass] = {
        val hlast = hash chained rss.head
        val entry = table get hlast
        val start = entry flatMap { crackrow(_, chain) }

        if (start.isDefined) start
        else if (rss.tail.nonEmpty) crack(rss.tail)
        else None
      }

      crack(chains)
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
