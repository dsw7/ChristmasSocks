# ChristmasSocks Remote Server Management Software
Another one of the many available remote server management utilities. I built this mainly out of
personal interest and for accelerating my workflow. This project essentially consists of several
[epoll](https://linux.die.net/man/4/epoll) driven servers whom receive commands from a master client.
## Table of Contents
  - [Synopsis](#synopsis)
    - [The master node](#the-master-node)
    - [The worker node](#the-worker-node)
  - [Installation](#installation)
    - [Setting up the client](#setting-up-the-client)
    - [Setting up the server](#setting-up-the-server)
      - [Step 1](#step-1)
      - [Step 2](#step-2)
      - [Step 3](#step-3)
      - [Step 4](#step-4)
    - [Uninstalling the server](#uninstalling-the-server)
  - [Client side development](#client-side-development)
  - [Server side development](#server-side-development)
    - [Static analysis](#static-analysis)
    - [Compiling a test binary](#compiling-a-test-binary)
      - [To compile a `CMAKE_BUILD_TYPE=Release` binary](#to-compile-a-cmake_build_typerelease-binary)
      - [To compile a `CMAKE_BUILD_TYPE=RelWithDebInfo` binary](#to-compile-a-cmake_build_typerelwithdebinfo-binary)
    - [Testing](#testing)
      - [Testing a `CMAKE_BUILD_TYPE=Release` binary](#testing-a-cmake_build_typerelease-binary)
      - [Running memory tests](#running-memory-tests)
      - [Testing with Docker](#testing-with-docker)
      - [Manual testing](#manual-testing)
    - [Shortcuts](#shortcuts)

## Synopsis
The following block diagram summarizes the distributed architecture underpinning this software:
<p align="center">
  <img src="./docs/socks_architecture.svg"
</p>

The architecture consists of the following components:
| Component | Description |
| --------- | ----------- |
| **Master** | This node queries all the worker nodes. Information returned from the nodes is then neatly displayed on the master node for analysis |
| **Worker** | These nodes accept commands from the master node. Commands can then return information about each node, for example whether the node is up, the CPU temperature, among others |
### The master node
_Section to be completed soon!_
### The worker node
Each worker node consists of the following components:
| Component | Description |
| --------- | ----------- |
| `/usr/bin/socks` | This is the server that accepts commands from the master |
| `/etc/socks.ini` | This is where the server looks for system configurations |
| `/etc/systemd/system/socks.service` | This is `systemd` unit file that drives the `socks` service |
## Installation
### Setting up the client
_Section to be completed soon!_
### Setting up the server
#### Step 1
Change directories into `/tmp` and download the product:
```bash
cd /tmp && git clone https://github.com/dsw7/ChristmasSocks.git
```
#### Step 2
Change directories into the project:
```bash
cd ChristmasSocks/ChristmasSocks
```
Then render the installation script executable:
```bash
chmod +x install.sh
```
#### Step 3
Run the installer as root:
```bash
sudo ./install.sh
```
Specify to install product:
```
Select setup type:
[1] -> Install product
[2] -> Uninstall product
> 1
```
Next specify under which Linux user this program will run:
```
Please enter a valid Linux user:
```
When prompted, enter the IPv4 address of the master host:
```
The following parameters must be entered by a human:

1. Specify the master host (enter a valid hostname or IPv4 address) [default: 127.0.0.1]:
```
The server **will only accept** connections from this host. Next set the bind IP:
```
2. Specify the bind IP [default: 127.0.0.1]: 0.0.0.0
```
Note that `0.0.0.0` typically translates to "accept connections from all interfaces," however this software
will block any hosts other than the master host.
#### Step 4
Ensure that the installation succeeded. A successful installation will be indicated by:
```
● socks.service - ChristmasSocks Remote Server Management Software
   Loaded: loaded (/etc/systemd/system/socks.service; enabled; vendor preset: enabled)
   Active: active (running) since <Day> <YYYY-MM-DD HH:MM:SS> PDT; <ss>ms ago
 Main PID: <pid> (socks)
   CGroup: /system.slice/socks.service
           └─<pid> /usr/bin/socks --config /etc/socks.ini
```
### Uninstalling the server
To uninstall the server, first follows steps [1](#step-1) and [2](#step-2) then run:
```bash
sudo ./install.sh
```
When prompted, specify to uninstall product:
```
Select setup type:
[1] -> Install product
[2] -> Uninstall product
> 2
```
The server should now be uninstalled.
## Client side development
_Section to be completed soon!_
## Server side development
All instructions are to be carried out relative to the following directory:
```
/path/to/ChristmasSocks
```
### Static analysis
To lint the C++ source, run:
```bash
make lint
```
As of right now, this project uses [cppcheck](http://cppcheck.sourceforge.net/) for static analysis.
### Compiling a test binary
#### To compile a `CMAKE_BUILD_TYPE=Release` binary:
Run the following:
```bash
make release
```
This will compile a binary under:
```
./ChristmasSocks/bin/socks
```
#### To compile a `CMAKE_BUILD_TYPE=RelWithDebInfo` binary:
Run the following:
```bash
make debug
```
This will, again, compile a binary under:
```
./ChristmasSocks/bin/socks
```
### Testing
#### Testing a `CMAKE_BUILD_TYPE=Release` binary:
To test a binary that was compiled following the instructions under [Compiling a test
binary](#compiling-a-test-binary), run:
```bash
make test-release
```
#### Running memory tests:
```bash
make test-memory
```
This project uses [Valgrind](https://valgrind.org/) for all dynamic analysis.
#### Testing with Docker
To run tests with Docker, first make sure that Docker is installed then change directories to the project
root:
```
cd /path/to/ChristmasSocks
```
Then run:
```bash
docker build -t socks .
```
This will generate a Debian based local Docker image. To actually test the product, run the `socks` image:
```bash
docker run -it --rm socks
```
The Dockerfile will simply run the steps:
- [Compiling a test binary](#compiling-a-test-binary)
- [Testing](#testing)

But within the container itself.
#### Manual testing
Manual testing can be done using [netcat](https://linux.die.net/man/1/nc) (`nc`). First, start the server:
```bash
./bin/socks --port 1234 # Or whatever port TCP port you wish to use
```
Then start an `nc` interactive session:
```
$ nc localhost 1234
echo
echo  # echoed back from the server
exit
```
The `exit` command will shut down the server. The server accepts EOL line endings, and therefore can
technically accept incoming Windows client connections, however support for this is poorly tested. First start
the server as follows:
```bash
./bin/socks --bind-ip 0.0.0.0 --port 1234 # Or whatever port TCP port you wish to use
```
Then from a Windows machine:
```
curl telnet://<ipv4-addr-server>:1234
```
### Shortcuts
_Compile and test release binary_:
```bash
./configure.py compile --release && ./configure.py test --release
```
_Compile and test debug binary_:
```bash
./configure.py compile --debug && ./configure.py test --memory
```
_Run end to end Docker test_:
```bash
docker build -t socks . && docker run -it --rm socks
```
