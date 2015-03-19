trait ParserConfig {
  scanner: ScannerConfig =>

  implicit def symbolToBlock(s: Symbol) = Block(Seq(Left(s)))
  implicit def tokenToBlock(t: scanner.Token) = Block(Seq(Right(t)))
  implicit def blockToOr(b: Block) = Or(Seq(b))
  implicit def symbolToOr(s: Symbol) = blockToOr(symbolToBlock(s))
  implicit def tokenToOr(t: scanner.Token) = blockToOr(tokenToBlock(t))

  case class Or(items: Seq[Block]) {
    def | (that: Block): Or = Or(items :+ that)
  }

  case class Block(items: Seq[Either[Symbol, scanner.Token]]) {
    def - (that: Block): Block = Block(items ++ that.items)
  }

  val grammar: Seq[(Symbol, Or)]
}

trait CoolParserCfg extends ParserConfig with CoolScannerCfg {
  import Tokens._

  val grammar: Seq[(Symbol, Or)] = Seq(
    ('program,  'stmt),
    ('stmt,     'forStmt | 'ifStmt | 'assignment),
    ('ifStmt,    IF - 'expr - THEN - 'stmt - FI),
    ('expr,     'id | 'numeral),
    ('num,       NUM),
    ('id,        ID)
  )
}

class AbstractParser {
  cfg: ParserConfig with ScannerConfig =>

  def verify(tokens: Stream[cfg.Token]): Boolean = {
    true
  }
}

