trait TestScannerCfg extends ScannerConfig {
  object Tokens extends Enumeration {
    val NUM, IF, THEN, ELSE, FI, ID = Value
  }

  override type Token = Tokens.Value

  override val fallback = { x: Status =>
    throw SyntaxException(s"invalid syntax at $x")
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
