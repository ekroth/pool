
fork := true

javaOptions ++= Seq("-DKILL_SERVER_FORK", "-Djava.security.egd=file:/dev/./urandom")

