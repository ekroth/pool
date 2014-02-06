version := "1.0"

scalaVersion := "2.10.3"

resolvers += "Typesafe Repository" at "http://repo.typesafe.com/typesafe/releases/"

libraryDependencies ++= Seq(
  "com.typesafe.akka" %% "akka-actor" % "2.3.0-RC2",
  "com.typesafe.akka" %% "akka-remote" % "2.3.0-RC2"
)

fork := true

javaOptions ++= Seq("-Djava.security.egd=file:/dev/./urandom")

lazy val common = project in file(".")

lazy val server = project in file("server") dependsOn common

lazy val client = project in file("client") dependsOn common
