trait TestParserCfg extends ParserConfig with TestScannerCfg {
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
