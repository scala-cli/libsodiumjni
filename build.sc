import $ivy.`de.tototec::de.tobiasroeser.mill.vcs.version_mill0.10:0.1.4`
import $file.settings, settings.{GenerateHeaders, JniModule, JniPublishModule, JniResourcesModule}

import coursier.cache.ArchiveCache
import coursier.util.Artifact
import de.tobiasroeser.mill.vcs.version.VcsVersion

import mill._
import mill.scalalib._
import mill.scalalib.publish._

import scala.concurrent.duration.Duration

object libsodiumjni extends MavenModule with JniModule with JniPublishModule with JniResourcesModule
    with GenerateHeaders {
  def ivyDeps = super.ivyDeps() ++ Seq(
    ivy"net.java.dev.jna:jna:5.8.0",
    ivy"net.java.dev.jna:jna-platform:5.8.0"
  )

  def windowsDllName = "libsodiumjni"
  def unixLibName    = "libsodiumjni"

  def unixLinkingLibs = T {
    super.unixLinkingLibs() ++ Seq("sodium")
  }

  private def windowsLibSodiumDistDir(): os.Path = {
    val archiveCache = ArchiveCache()
    archiveCache.get(Artifact("https://download.libsodium.org/libsodium/releases/libsodium-1.0.18-stable-msvc.zip")).unsafeRun()(archiveCache.cache.ec) match {
      case Left(e) => throw new Exception(e)
      case Right(dir0) => os.Path(dir0, os.pwd)
    }
  }

  def windowsLinkingLibs = T {
    val dir = windowsLibSodiumDistDir()
    val lib = dir / "libsodium" / "x64" / "Release" / "v143" / "dynamic" / "libsodium"
    super.windowsLinkingLibs() ++ Seq(lib.toString)
  }
  def extraWindowsCOptions = T {
    val dir = windowsLibSodiumDistDir()
    val headerDir = dir / "libsodium" / "include"
    Seq(
      "/D__WIN__",
      "/I", headerDir.toString
    )
  }
  def windowsDllCOptions = T {
    super.windowsDllCOptions() ++
      extraWindowsCOptions()
  }
  def windowsLibCOptions = T {
    super.windowsLibCOptions() ++
      extraWindowsCOptions() ++
      Seq("/DSODIUM_STATIC")
  }
  def windowsBatInit = T {
    import java.util.Locale
    val dir = windowsLibSodiumDistDir()
    val dllDir = dir / "libsodium" / "x64" / "Release" / "v143" / "dynamic"
    val pathEnvVarName = sys.env.find(_._1.toLowerCase(Locale.ROOT) == "path").fold("PATH")(_._1)
    "@echo on" + System.lineSeparator() +
      super.windowsBatInit() + System.lineSeparator() +
      s"""set "$pathEnvVarName=$dllDir;%$pathEnvVarName%""""
  }

  def jniArtifactDir =
    if (System.getenv("CI") == null) None
    else Some(os.Path("artifacts/", os.pwd))

  def publishVersion = T {
    val state = VcsVersion.vcsState()
    if (state.commitsSinceLastTag > 0) {
      val versionOrEmpty = state.lastTag
        .map(_.stripPrefix("v"))
        .map { tag =>
          val idx = tag.lastIndexOf(".")
          if (idx >= 0) tag.take(idx + 1) + (tag.drop(idx + 1).takeWhile(
            _.isDigit
          ).toInt + 1).toString + "-SNAPSHOT"
          else ""
        }
        .getOrElse("0.0.1-SNAPSHOT")
      Some(versionOrEmpty)
        .filter(_.nonEmpty)
        .getOrElse(state.format())
    }
    else
      state
        .lastTag
        .getOrElse(state.format())
        .stripPrefix("v")
  }
  def pomSettings = PomSettings(
    description = artifactName(),
    organization = "org.virtuslab.scala-cli",
    url = "https://github.com/virtusLab/libsodiumjni",
    licenses = Seq(License.`Apache-2.0`),
    versionControl = VersionControl.github("virtusLab", "libsodiumjni"),
    developers = Seq(
      Developer("alexarchambault", "Alex Archambault", "https://github.com/alexarchambault")
    )
  )

  object test extends Tests {
    def ivyDeps = super.ivyDeps() ++ Seq(
      ivy"com.novocode:junit-interface:0.11"
    )
    def testFramework = "com.novocode.junit.JUnitFramework"
  }
}

def publishSonatype(tasks: mill.main.Tasks[PublishModule.PublishData]) =
  T.command {
    import scala.concurrent.duration.DurationInt
    val timeout     = 10.minutes
    val credentials = sys.env("SONATYPE_USERNAME") + ":" + sys.env("SONATYPE_PASSWORD")
    val pgpPassword = sys.env("PGP_PASSWORD")
    val data        = T.sequence(tasks.value)()

    doPublishSonatype(
      credentials = credentials,
      pgpPassword = pgpPassword,
      data = data,
      timeout = timeout,
      log = T.ctx().log
    )
  }

def doPublishSonatype(
  credentials: String,
  pgpPassword: String,
  data: Seq[PublishModule.PublishData],
  timeout: Duration,
  log: mill.api.Logger
): Unit = {

  val artifacts = data.map {
    case PublishModule.PublishData(a, s) =>
      (s.map { case (p, f) => (p.path, f) }, a)
  }

  val isRelease = {
    val versions = artifacts.map(_._2.version).toSet
    val set      = versions.map(!_.endsWith("-SNAPSHOT"))
    assert(
      set.size == 1,
      s"Found both snapshot and non-snapshot versions: ${versions.toVector.sorted.mkString(", ")}"
    )
    set.head
  }
  val publisher = new publish.SonatypePublisher(
    uri = "https://s01.oss.sonatype.org/service/local",
    snapshotUri = "https://s01.oss.sonatype.org/content/repositories/snapshots",
    credentials = credentials,
    signed = true,
    gpgArgs = Seq(
      "--detach-sign",
      "--batch=true",
      "--yes",
      "--pinentry-mode",
      "loopback",
      "--passphrase",
      pgpPassword,
      "--armor",
      "--use-agent"
    ),
    readTimeout = timeout.toMillis.toInt,
    connectTimeout = timeout.toMillis.toInt,
    log = log,
    awaitTimeout = timeout.toMillis.toInt,
    stagingRelease = isRelease
  )

  publisher.publishAll(isRelease, artifacts: _*)
}
