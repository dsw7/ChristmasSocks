from configparser import ConfigParser
from os import path
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

    def connect(self) -> None:
        self.socket.connect((
            self.ini_configs['server']['ipv4_address_server'],
            self.ini_configs['server']['tcp_port']
        ))

    def disconnect(self) -> None:
        self.socket.close()

    def send(self, command: str) -> str:
        self.socket.sendall(command.encode())
        bytes_recv = self.socket.recv(self.ini_configs['server']['tcp_buffer_size'])
        return bytes_recv.decode()
