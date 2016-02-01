# Gone In November

## Environment variables
Besides the default environment variables that you have to tweak in order to compile GiN (they are ```REPOPATH```, ```GAMEPATH``` and ```ACKPATH``` - more information can be found under the [main README](https://github.com/Florastamine/Gone-in-November/blob/master/README.md) file.), there are a few optional variables which you can use to further enhance the compilation process.

### UPXPATH
Supply the path to your UPX installation directory.

After the compilation process is completed, GiN will use the [UPX](http://upx.sourceforge.net/) executable which is given using this path to compress the final executable.
