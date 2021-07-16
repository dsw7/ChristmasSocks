from configparser import ConfigParser
from os import path
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

ALPHANUMERIC = ascii_letters + digits
PATH_THIS = path.dirname(__file__)
PATH_INI = path.join(PATH_THIS, 'client.ini')

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
        self.cfgs = ConfigParser()
        self.cfgs.read(PATH_INI)

        parent = path.dirname(PATH_THIS)
        self.binary = path.join(parent, self.cfgs['server']['output-dir'], self.cfgs['server']['output-name'])
        self.process = None

    def start_server(self) -> None:
        self.process = Popen(self.binary, stdout=DEVNULL)
        sleep(self.cfgs['server'].getfloat('startup-delay'))

    def start_server_under_valgrind(self) -> None:
        command = 'valgrind {}'.format(self.binary)
        self.process = Popen(command.split(), stdout=DEVNULL)
        sleep(self.cfgs['server'].getfloat('startup-delay-valgrind'))

    def stop_server(self) -> None:
        sleep(self.cfgs['server'].getfloat('shutdown-delay'))
        self.process.send_signal(SIGINT)


class Client:

    def __init__(self) -> None:
        self.cfgs = ConfigParser()
        self.cfgs.read(PATH_INI)

        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.settimeout(self.cfgs['client'].getfloat('sock_timeout'))

    def connect(self) -> None:
        self.socket.connect((
            self.cfgs['client']['ipv4_address_server'],
            self.cfgs['client'].getint('tcp_port')
        ))

    def disconnect(self) -> None:
        self.socket.close()

    def send(self, command: str) -> str:
        self.socket.sendall(command.encode())
        buffer_size = self.cfgs['client'].getint('tcp_buffer_size')
        bytes_recv = self.socket.recv(buffer_size)
        return bytes_recv.decode()
