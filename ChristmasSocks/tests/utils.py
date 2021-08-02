import sys
from configparser import ConfigParser
from os import path, makedirs
from typing import Optional
from time import sleep
from subprocess import (
    Popen,
    DEVNULL,
    call
)
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


class Server:

    def __init__(self) -> None:
        self.configs = read_test_config_file()
        parent = path.dirname(PATH_THIS)
        self.binary = path.join(parent, self.configs['server']['output-dir'], self.configs['server']['output-name'])
        self.process = None

    def start_server_in_foreground(self, *args) -> int:
        command = ()
        command += (self.binary,)
        command += args
        return call(command)

    def start_server_in_background(self, *args) -> None:
        if len(args) > 0:
            command = ()
            command += (self.binary,)
            command += args
        else:
            command = self.binary

        self.process = Popen(command, stdout=DEVNULL)
        sleep(self.configs['server'].getfloat('startup-delay'))

    def start_server_under_valgrind(self, log_file: str) -> None:
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
