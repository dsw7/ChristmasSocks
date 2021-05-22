# ChristmasSocks
C++ socket utilities for my various other projects. This project also depicts my workflow for working with C++ projects.

## Compiling a test binary
To compile a test binary, run the following:
```bash
$ cd /path/to/ChristmasSocks
$ chmod +x ChristmasSocks/configure.py
$ ./ChristmasSocks/configure.py compile
```

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
