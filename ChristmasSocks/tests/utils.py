from configparser import ConfigParser
from os import path
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
        pass

    


class Client:

    def __init__(self) -> None:
        self.ini_configs = ConfigParser()
        self.ini_configs.read(path.join(path.dirname(__file__), 'client.ini'))

        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.settimeout(self.ini_configs['client'].getfloat('sock_timeout'))

    def connect(self) -> None:
        self.socket.connect((
            self.ini_configs['client']['ipv4_address_server'],
            self.ini_configs['client'].getint('tcp_port')
        ))

    def disconnect(self) -> None:
        self.socket.close()

    def send(self, command: str) -> str:
        self.socket.sendall(command.encode())
        buffer_size = self.ini_configs['client'].getint('tcp_buffer_size')
        bytes_recv = self.socket.recv(buffer_size)
        return bytes_recv.decode()
