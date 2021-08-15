import sys
from configparser import ConfigParser
from abc import ABC, abstractmethod
from os import path, makedirs
from typing import Optional, Union
from time import sleep
from subprocess import (
    Popen,
    DEVNULL,
    STDOUT
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


class Server(ABC):

    def __init__(self) -> None:
        self.configs = read_test_config_file()

        self.binary = None
        self.process = None
        self.logdir_valgrind = None
        self.logdir_release = None
        self.log_handle = None

        self.set_binary_path()
        self.set_release_log()
        self.set_valgrind_log()

    def set_binary_path(self) -> None:
        parent = path.dirname(PATH_THIS)
        self.binary = path.join(
            parent, self.configs['server']['output-dir'], self.configs['server']['output-name']
        )

    def set_release_log(self) -> None:
        self.logdir_release = path.join(PATH_THIS, self.configs['server']['release-log-directory'])
        if not path.exists(self.logdir_release):
            makedirs(self.logdir_release)

    def set_valgrind_log(self) -> None:
        self.logdir_valgrind = path.join(PATH_THIS, self.configs['server']['valgrind-log-directory'])
        if not path.exists(self.logdir_valgrind):
            makedirs(self.logdir_valgrind)

    @abstractmethod
    def start_server(self, *args, **kwargs) -> Union[int, None]:
        pass

    @abstractmethod
    def stop_server(self) -> None:
        pass


class ServerForeground(Server):

    def start_server(self, *args, **kwargs) -> int:
        command = ()
        command += (self.binary,)
        command += args

        logfile = path.join(self.logdir_release, kwargs['logfile'])
        with open(logfile, 'w') as log_handle:
            exit_code = Popen(command, stdout=log_handle, stderr=STDOUT).wait()
        return exit_code

    def stop_server(self) -> None:
        pass


class ServerBackground(Server):

    def start_server(self, *args, **kwargs) -> None:
        command = ()
        command += (self.binary,)
        command += args

        logfile = path.join(self.logdir_release, kwargs['logfile'])
        self.log_handle = open(logfile, 'w')
        self.process = Popen(command, stdout=self.log_handle, stderr=STDOUT)

    def stop_server(self) -> None:
        sleep(self.configs['server'].getfloat('shutdown-delay'))
        self.process.send_signal(SIGINT)
        self.log_handle.close()


class ServerValgrind(Server):

    def start_server(self, *args, **kwargs) -> None:
        logfile = path.join(self.logdir_valgrind, kwargs['logfile'])
        command = 'valgrind --leak-check=yes --log-file={} {}'.format(logfile, self.binary)
        self.process = Popen(command.split(), stdout=DEVNULL, stderr=STDOUT)
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

        for _ in self.configs['client'].getint('max_connection_attempts'):
            try:
                sleep(0.02)
                self.socket.connect((host, port))
            except ConnectionRefusedError:
                continue
            else:
                break
        else:
            raise ConnectionRefusedError

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
