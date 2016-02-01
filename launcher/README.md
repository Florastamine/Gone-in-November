# Gone In November

Welcome to Gone In November's official development page!

Gone In November is a first-person exploration, surreal interactive story about rain, solitude, being forgotten and loss. It is currently in active development by one person and it is written with the help of the Acknex rendering/game library.

## Description
This directory is reserved for the game launcher, which is written in [Go](https://golang.org/). It was initially written in [Rust](https://www.rust-lang.org/), but I chose Go over Rust because of the following reasons:
* The purpose of the game launcher is to act as a shell which launches the "actual" game executable and perform content updates/patches. Go does this far more better than Rust (which is more of a system language, while Go focuses on both application and web development: It has a HTTP interface and an archive ([zip](https://en.wikipedia.org/wiki/ZIP_archive)/[tar](https://en.wikipedia.org/wiki/Tar_archive)) interface built-in. Rust, of course, can do this (with [rust-openssl](https://github.com/sfackler/rust-openssl) and [rust-zip](https://github.com/slackito/zip)), but it would require the user to install a number of tools and dependencies (Rust compiler, OpenSSL, mingw-w64, along with a large number of ["Rust crates"](https://crates.io/)) just to compile a small launcher, which is not worth).
* Go is very fast and easy to use. Rust, on the other hand, is a mostly a functional-based language, and thus has a different learning curve. It's not harder than imperative/structured languages like Go, just different. But I want to keep my programming paradigms coherent across different code bases, so Go is no doubt my choice.
