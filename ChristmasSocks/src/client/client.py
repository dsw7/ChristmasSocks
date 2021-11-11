import sys
from configparser import ConfigParser
from os import path
from time import sleep
from socket import (
    socket,
    AF_INET,
    SOCK_STREAM
)

PATH_THIS = path.dirname(__file__)

def read_socks_config_file() -> ConfigParser:
    ini_file = path.join(PATH_THIS, 'configs', 'socks.ini')
    if not path.exists(ini_file):
        sys.exit('Could not open {}'.format(ini_file))

    parser = ConfigParser()
    parser.read(ini_file)
    return parser


class Client:

    def __init__(self) -> None:
        self.configs = read_socks_config_file()
        self.socket = None

    def connect(self) -> None:
        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.settimeout(self.configs['client'].getfloat('sock_timeout'))

        port = self.configs['client'].getint('tcp_port')
        host = self.configs['client']['ipv4_address_server']

        for _ in range(self.configs['client'].getint('max_connection_attempts')):
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

        try:
            bytes_recv = self.socket.recv(buffer_size)
        except ConnectionResetError:
            return ''
        return bytes_recv.decode()

    def stop_server(self) -> None:
        command = 'exit'
        self.socket.sendall(command.encode())
