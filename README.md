# libsodiumjni

Minimal JNI bindings for libsodium

This project provides JNI bindings for some [libsodium](https://github.com/jedisct1/libsodium) methods.

It's used by [Scala CLI](https://github.com/VirtusLab/scala-cli) to encode repository secrets about to be uploaded to GitHub via the GitHub API. For now, it only exposes libsodium methods that Scala CLI needs.

The motivation for writing libsodiumjni, rather than using any of the existing JNA-based bindings, is mainly using JNI rather than JNA to interface with libsodium, as JNI is supported from GraalVM native images, while JNA is not.

## Build

libsodiumjni is built with Mill.

Compile everything with
```text
$ ./mill __.compile
```

The Mill build automatically compile the C bindings, and generates a `.so` / `.dylib` / `.dll`. These files are also built on the CI, and pushed to Maven Central (see below).

## Artifacts

libsodiumjni is pushed to [Maven Central](https://repo1.maven.org/maven2/io/github/alexarchambault/tmp/libsodiumjni/libsodiumjni). Add it in a Mill module with
```scala
def ivyDeps = super.ivyDeps() ++ Seq(
  ivy"io.github.alexarchambault.tmp.libsodiumjni:libsodiumjni:0.0.2"
)
```

Both static and dynamic libraries are also pushed as is to Maven Central (see the `.so` / `.dylib` / `.dll` and `.a` / `.lib` files [here](https://repo1.maven.org/maven2/io/github/alexarchambault/tmp/libsodiumjni/libsodiumjni/0.0.2/) for example). From the command line, you can fetch those files with commands such as
```text
$ cs fetch --intransitive io.github.alexarchambault.tmp.libsodiumjni:libsodiumjni:0.0.2,classifier=x86_64-pc-win32,ext=dll,type=dll -A dll
```

## Adding support for new libsodium new methods

Typical workflow:
- add a `native` method in [`libsodiumjni/src/main/java/libsodiumjni/internal/SodiumApi.java`](https://github.com/scala-cli/libsodiumjni/blob/main/libsodiumjni/src/main/java/libsodiumjni/internal/SodiumApi.java)
- run `./mill __.compile`, this should add a corresponding declaration in [`libsodiumjni/src/main/c/libsodiumjni_internal_SodiumApi.h`](https://github.com/scala-cli/libsodiumjni/blob/main/libsodiumjni/src/main/c/libsodiumjni_internal_SodiumApi.h)
- add an implementation for that declaration in `libsodiumjni_internal_SodiumApi.c` in the same directory
- run `./mill __.compile` to ensure that the new C code compiles fine
- use the newly added `native` method in [`Sodium.java`](https://github.com/scala-cli/libsodiumjni/blob/main/libsodiumjni/src/main/java/libsodiumjni/Sodium.java) say
- optionally, add tests for it in [`SodiumTests.java`](https://github.com/scala-cli/libsodiumjni/blob/main/libsodiumjni/src/test/java/libsodiumjni/tests/SodiumTests.java)
- ensure those tests pass when running `./mill __.test`
