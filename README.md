# ChristmasSocks
C++ socket utilities for my various other projects. This project also depicts my workflow for working with C++ projects.

## Compiling a test binary
To compile a test binary, run the following:
```bash
$ cd /path/to/ChristmasSocks
$ chmod +x ChristmasSocks/configure.py
$ ./ChristmasSocks/configure.py compile
```
This will compile a binary under:
```
/path/to/ChristmasSocks/ChristmasSocks/bin/socks
```

## Static analysis
To lint the C++ source, run:
```bash
$ cd /path/to/ChristmasSocks
$ ./ChristmasSocks/configure.py lint
```
As of right now, this project uses [cppcheck](http://cppcheck.sourceforge.net/) for static analysis.

## Testing with Docker
I am testing this project with Docker. To run tests with Docker, first make sure that Docker is installed. Then run:
```bash
$ cd /path/to/ChristmasSocks
$ docker build -t socks .
```
This will generate a Debian based local Docker image. To actually test the product, run the `socks` image:
```bash
$ docker run -it --rm socks
```
