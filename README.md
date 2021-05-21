# ChristmasSocks
C++ socket utilities for my various other projects

## Testing with Docker
I am testing this project with Docker. To run tests with Docker, first make sure that Docker is installed. Then run:
```
cd /path/to/ChristmasSocks
docker build -t socks .
```
This will generate a Debian based local Docker image. To actually test the product, run the `socks` image:
```
docker run -it --rm socks
```
