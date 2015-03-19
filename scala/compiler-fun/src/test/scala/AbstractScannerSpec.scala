import org.scalatest.FlatSpec

class ScannerSpec extends FlatSpec {
  val scanner = new AbstractScanner with TestScannerCfg

  import scanner.Tokens._

  "AbstractScanner" should "create proper increments" in {
    assert(scanner.increments("abc").toList ===
      List("a", "ab", "abc"))
  }

  it should "handle partial matches" in {
    assert(scanner.possible("el").head._1.toString === 
      scanner.config(3)._1)
  }

  it should "handle full matches" in {
    assert(scanner.possible("else").head._1.toString === 
      scanner.config(3)._1)
  }

  it should "find the longest match" in {
    val (Some((_, scanner.Status(str))), _) = scanner.longest("elseif")
    assert(str === "elseif")
  }

  it should "handle one token" in {
    assert(scanner.tokens("else").toList === 
      List(ELSE))
  }

  it should "handle newlines" in {
    assert(scanner.tokens("else \n 1").toList === 
      List(ELSE, NUM))
  }

  it should "handle multiple tokens" in {
    assert(scanner.tokens("if x then 1 else y fi").toList === 
      List(IF, ID, THEN, NUM, ELSE, ID, FI))
  }

  it should "handle newline without space" in {
    assert(scanner.tokens("else\n 1").toList ===
      List(ELSE, NUM))
    assert(scanner.tokens("else\n1").toList ===
      List(ELSE, NUM))
    assert(scanner.tokens("else \n1").toList ===
      List(ELSE, NUM))
  }

  it should "handle a number" in {
    assert(scanner.tokens("1").toList === List(NUM))
  }

  it should "handle complex code" in {
    val code = """
    if x then
      if omg then 1 else
        2 fi
    else
      0
    fi
   """

    assert(scanner.tokens(code).toList ===
      List(IF, ID, THEN, IF, ID, THEN, NUM, ELSE, NUM, FI,
        ELSE, NUM, FI))
  }

  it should "handle invalid code" in {
      intercept[scanner.SyntaxException] { 
        scanner.tokens("1a") 
      }
  }

}
