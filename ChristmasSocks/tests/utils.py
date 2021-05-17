from configparser import ConfigParser
from os import path, chdir
from subprocess import Popen
from signal import SIGINT
from string import (
    ascii_letters,
    digits
)
from socket import (
    socket,
    AF_INET,
    SOCK_STREAM
)
from random import choice

ALPHANUMERIC = ascii_letters + digits

def generate_random_string(num_strings: int, len_strings: int) -> list:
    result = []
    for _ in range(num_strings):
        result.append(''.join(choice(ALPHANUMERIC) for _ in range(len_strings)))
    return result


class Connection:
    def __init__(self) -> None:
        self.ini_configs = ConfigParser()
        self.ini_configs.read(path.join(path.dirname(__file__), 'tests.ini'))

        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.settimeout(self.ini_configs['server'].getfloat('sock_timeout'))
        self.process = None

    def start_server(self) -> None:
        chdir(path.dirname(__file__))
        command = '../bin/test'
        self.process = Popen(command)

    def stop_server(self) -> None:
        self.process.send_signal(SIGINT)

    def connect(self) -> None:
        self.socket.connect((
            self.ini_configs['server']['ipv4_address_server'],
            self.ini_configs['server'].getint('tcp_port')
        ))

    def disconnect(self) -> None:
        self.socket.close()

    def send(self, command: str) -> str:
        self.socket.sendall(command.encode())
        buffer_size = self.ini_configs['server'].getint('tcp_buffer_size')
        bytes_recv = self.socket.recv(buffer_size)
        return bytes_recv.decode()
