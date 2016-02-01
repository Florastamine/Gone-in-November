# Gone In November

## Environment variables
Besides the default environment variables that you have to tweak in order to compile GiN (they are ```REPOPATH```, ```GAMEPATH``` and ```ACKPATH``` - more information can be found under the [main README](https://github.com/Florastamine/Gone-in-November/blob/master/README.md) file.), there are a few optional variables which you can use to further enhance the compilation process.

### UPXPATH
Supply the path to your UPX installation directory.

After the compilation process is completed, GiN will use the [UPX](http://upx.sourceforge.net/) executable which is given using this path to compress the final executable.

### RUSTPATH
Supply the path to your [Rust](http://rust-lang.org/) installation directory.

In order to compile the game launcher (which is written in [Rust](http://rust-lang.org/)), a Rust compiler must be provided. Additionally you must have an existing (mingw-w64)[http://sourceforge.net/projects/mingw-w64/] and [OpenSSL](http://slproweb.com/products/Win32OpenSSL.html) installation. If you don't want to go through the burden of compiling the launcher yourself, just use the existing binaries from your copy of GiN.
