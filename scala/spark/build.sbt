
name := "Spark Experiments"

version := "1.0"

scalaVersion := "2.11.8"

javaOptions ++= Seq("-Xms512M", "-Xmx2048M", "-XX:MaxPermSize=2048M", "-XX:+CMSClassUnloadingEnabled")

//cancelable in Global := true

libraryDependencies += "org.apache.spark" %% "spark-core" % "2.0.0"

libraryDependencies += "com.holdenkarau" %% "spark-testing-base" % "1.6.1_0.3.3"


libraryDependencies += "org.scalacheck" %% "scalacheck" % "1.13.2" % "test"

libraryDependencies += "org.scalatest" %% "scalatest" % "3.0.0" % "test"

