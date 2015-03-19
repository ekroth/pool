import org.scalatest.FlatSpec

class ParserSpec extends FlatSpec {
  val parser = new AbstractParser with TestParserCfg

  import parser._

  "A ParserConfig" should "build simple grammar" in {
    assert(parser.grammar(0)._2 ===
      Or(Seq(Block(Seq(Left('stmt))))))
  }

  it should "handle multiple Or" in {
    assert(parser.grammar(1)._2 ===
      Or(Seq(
        Block(Seq(Left('forStmt))),
        Block(Seq(Left('ifStmt))),
        Block(Seq(Left('assignment)))
      )))
  }

  it should "handle if example" in {
    import parser.Tokens._
    assert(parser.grammar(2)._2 ===
      Or(Seq(Block(Seq(
        Right(IF),
        Left('expr),
        Right(THEN),
        Left('stmt),
        Right(FI)
      )))))
  }

  "A Parser" should "verify simple if" in {

  }
}
