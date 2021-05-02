from configparser import ConfigParser
from os import oath
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

BUFFER_SIZE = 1024
SERVER_HOST = '10.0.0.98'
SERVER_PORT = 8080
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

    def connect(self) -> None:
        self.socket.connect((SERVER_HOST, SERVER_PORT))

    def disconnect(self) -> None:
        self.socket.close()

    def send(self, command: str) -> str:
        self.socket.sendall(command.encode())
        bytes_recv = self.socket.recv(BUFFER_SIZE)
        return bytes_recv.decode()
