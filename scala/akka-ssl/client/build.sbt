
fork := true

javaOptions ++= Seq("-DKILL_CLIENT_FORK", "-Djava.security.egd=file:/dev/./urandom")

