import sys
from configparser import ConfigParser
from abc import ABC, abstractmethod
from os import path, makedirs
from typing import Optional, Union
from time import sleep
from subprocess import Popen, DEVNULL
from signal import SIGINT
from string import (
    ascii_letters,
    digits,
    punctuation
)
from socket import (
    socket,
    AF_INET,
    SOCK_STREAM
)
from random import choice
from click import secho

EXIT_SUCCESS = 0
EXIT_FAILURE = 1
ALPHANUMERIC = ascii_letters + digits
PATH_THIS = path.dirname(__file__)

def read_test_config_file() -> ConfigParser:
    ini_file = path.join(PATH_THIS, 'tests.ini')
    if not path.exists(ini_file):
        sys.exit('Could not open {}'.format(ini_file))

    parser = ConfigParser()
    parser.read(ini_file)
    return parser

def echo_message(msg: str) -> None:
    secho('{}'.format(msg), fg='yellow')

def generate_random_string(num_strings: int, len_strings: int) -> list:
    result = []
    for _ in range(num_strings):
        result.append(''.join(choice(ALPHANUMERIC) for _ in range(len_strings)))
    return result

def generate_random_punctuation(num_strings: int, len_strings: int) -> list:
    result = []
    for _ in range(num_strings):
        result.append(''.join(choice(punctuation) for _ in range(len_strings)))
    return result


class Server(ABC):

    def __init__(self) -> None:
        self.configs = read_test_config_file()
        parent = path.dirname(PATH_THIS)
        self.binary = path.join(parent, self.configs['server']['output-dir'], self.configs['server']['output-name'])
        self.process = None

    @abstractmethod
    def start_server(self, *args) -> Union[int, None]:
        pass

    @abstractmethod
    def stop_server(self) -> None:
        pass


class ServerForeground(Server):

    def start_server(self, *args) -> int:
        command = ()
        command += (self.binary,)
        command += args
        return Popen(command, stdout=DEVNULL).wait()


class ServerBackground(Server):

    def start_server(self, *args) -> None:
        command = ()
        command += (self.binary,)
        command += args
        self.process = Popen(command, stdout=DEVNULL)
        sleep(self.configs['server'].getfloat('startup-delay'))

    def stop_server(self) -> None:
        sleep(self.configs['server'].getfloat('shutdown-delay'))
        self.process.send_signal(SIGINT)


class ServerValgrind(Server):

    def start_server(self, log_file: str) -> None:
        log_file_dump = path.join(PATH_THIS, self.configs['server']['valgrind-log-directory'])

        if not path.exists(log_file_dump):
            makedirs(log_file_dump)

        path_log_file = path.join(log_file_dump, log_file)
        echo_message('Valgrind log file: {}'.format(path_log_file))

        command = 'valgrind --leak-check=yes --log-file={} {}'.format(path_log_file, self.binary)
        self.process = Popen(command.split(), stdout=DEVNULL)
        sleep(self.configs['server'].getfloat('startup-delay-valgrind'))

    def stop_server(self) -> None:
        sleep(self.configs['server'].getfloat('shutdown-delay'))
        self.process.send_signal(SIGINT)


class Client:

    def __init__(self) -> None:
        self.configs = read_test_config_file()
        self.socket = None

    def connect(self, port: Optional[int]=None, host: Optional[str]=None) -> None:
        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.settimeout(self.configs['client'].getfloat('sock_timeout'))

        if not port:
            port = self.configs['client'].getint('tcp_port')

        if not host:
            host = self.configs['client']['ipv4_address_server']

        self.socket.connect((host, port))

    def disconnect(self) -> None:
        self.socket.close()

    def send(self, command: str) -> str:
        self.socket.sendall(command.encode())
        buffer_size = self.configs['client'].getint('tcp_buffer_size')
        bytes_recv = self.socket.recv(buffer_size)
        return bytes_recv.decode()

    def stop_server(self) -> None:
        command = 'exit'
        self.socket.sendall(command.encode())
