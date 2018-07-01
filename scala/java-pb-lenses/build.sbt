PB.targets in Compile := Seq(
  PB.gens.java("2.6.1") -> (sourceManaged in Compile).value,
  scalapb.gen(javaConversions = true) -> (sourceManaged in Compile).value
)

scalaVersion := "2.11.5"