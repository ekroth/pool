import java.util.regex._

trait ScannerConfig {
  case class SyntaxException(str: String) extends Exception
  case class Status(str: String)
  type TokenResult = Status => Option[Token]
  type Rule = (Pattern, TokenResult)

  type Token
  val config: Seq[(String, TokenResult)]
  val fallback: TokenResult
}

class AbstractScanner {
  cfg: ScannerConfig =>

  lazy val rules: Seq[Rule] = cfg.config map {
    case (s, t) => (Pattern.compile(s), t)
  }

  /** Step through String.
    increments("abc") = Stream("a", "ab", "abc")
    */
  def increments(xs: String): Stream[String] = (Stream.iterate(xs take 1) {
    x => xs take (x.length + 1)
  }) take xs.length

  /** Partial and exact matches */
  def possible(xs: String): Seq[Rule] = rules filter { x =>
    val m = x._1.matcher(xs)
    m.matches || m.hitEnd
  }

  def perfect(xs: String): Seq[Rule] = rules filter {
    _._1.matcher(xs).matches
  }

  /** All possible matches for each part of String */
  def allPossible(str: String): Seq[(Seq[Rule], Status)] = {
    def perfect(xs: String)(r: Rule): Boolean = r._1.matcher(xs).matches

    val ps = increments(str) map { x => (x, possible(x), Status(x)) }
    val range = ps takeWhile { _._2.nonEmpty }
    val perf = range map {
      case (x, xs, st) => (xs filter perfect(x), st)
    }

    perf filter { _._1.nonEmpty }
  }

  /** Longest match for String */
  def longest(xs: String): (Option[(Rule, Status)], String) = {
    val ps = allPossible(xs)

    val rule = for {
      (rs, st) <- ps.lastOption
      r <- rs.headOption
    } yield (r, st)

    val dropped = rule map { _._2.str.length }

    (rule, xs drop (dropped getOrElse 0))
  }

  def tokens(xs: String): Stream[Token] = {
    def tokensOpt(xs: String): Stream[Option[Token]] =
      if (xs.isEmpty) Stream.empty
      else {
        val (scan, rest) = longest(xs)
        val token = for {
          (rule, status) <- scan
          (p, tOpt) = rule
          t <- tOpt(status)
        } yield t

        if (rest.length == xs.length) 
          Stream(fallback(Status(rest)))
        else 
          Stream.cons(token, tokensOpt(rest))
      }

    tokensOpt(xs).flatten
  }

}

trait CoolScannerCfg extends ScannerConfig  {
  object Tokens extends Enumeration {
    val NUM, IF, THEN, ELSE, FI, ID = Value
  }

  type Token = Tokens.Value

  override val fallback = { x: Status => 
    throw new Exception(s"invalid syntax at $x") 
  }

  override val config = Seq[(String, TokenResult)](
    "[[ ] | \t | \f | \n | \r]" -> { _ => None },
    "if" -> { _ => Some(Tokens.IF) },
    "then" -> { _ => Some(Tokens.THEN) },
    "else" -> { _ => Some(Tokens.ELSE) },
    "fi" -> { _ => Some(Tokens.FI) },
    """([a-zA-z]+[0-9]+)|([0-9]+[a-zA-z]+)""" -> { x => throw SyntaxException("invalid id?") },
    "[a-zA-Z]+" -> { _ => Some(Tokens.ID) },
    "[0-9]+" -> { _ => Some(Tokens.NUM) }
  )
}
