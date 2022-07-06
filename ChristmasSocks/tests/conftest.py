from logging import getLogger
from time import sleep
from subprocess import Popen, STDOUT
from signal import SIGINT
from os import path, environ
from tempfile import gettempdir
from socket import socket, AF_INET, SOCK_STREAM
from typing import TypeVar
import pytest

T = TypeVar('T')
LOGGER = getLogger(__name__)
PATH_SOCKS_BIN = 'ChristmasSocks/bin/socks'
SERVER_SHUTDOWN_DELAY = 0.05


class Client:

    def __init__(self: T) -> T:
        self.socket = None

    def connect(self: T) -> None:

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

    def disconnect(self: T) -> None:
        self.socket.close()

    def send(self, command: str) -> str:

        self.socket.sendall(command.encode())
        buffer_size = self.configs['client'].getint('tcp_buffer_size')

        try:
            bytes_recv = self.socket.recv(buffer_size)
        except ConnectionResetError:
            return ''
        return bytes_recv.decode()

    def stop_server(self: T) -> None:

        command = 'exit'
        self.socket.sendall(command.encode())


@pytest.fixture
def socks_server_background() -> None:

    if not path.exists(PATH_SOCKS_BIN):
        pytest.exit(f'Binary {PATH_SOCKS_BIN} does not exist. Exiting!')

    try:
        current_test = environ['PYTEST_CURRENT_TEST']
    except KeyError:
        pytest.exit('Could not find PYTEST_CURRENT_TEST environment variable. Are you using pytest?')

    current_test = current_test.split('::')[0]
    current_test = path.splitext(current_test)[0]

    logfile = path.join(gettempdir(), current_test)
    logfile = f'{logfile}.log'

    LOGGER.debug('Will log to: %s', logfile)

    log_handle = open(logfile, 'w')
    process = Popen(PATH_SOCKS_BIN, stdout=log_handle, stderr=STDOUT)

    yield

    LOGGER.debug('Closing connection')
    sleep(SERVER_SHUTDOWN_DELAY)

    process.send_signal(SIGINT)
    log_handle.close()
