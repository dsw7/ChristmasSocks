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

TCP_PORT = 8080
IPV4_ADDRESS_SERVER = '127.0.0.1'
TCP_BUFFER_SIZE = 1024
SOCK_TIMEOUT = 1.5
MAX_CONNECTION_ATTEMPTS = 25
SERVER_SHUTDOWN_DELAY = 0.05


class Client:

    def __init__(self: T) -> T:
        self.socket = None

    def connect(self: T) -> None:

        LOGGER.debug('Connecting to socks server on port %i and host %s', TCP_PORT, IPV4_ADDRESS_SERVER)

        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.settimeout(SOCK_TIMEOUT)

        for _ in range(MAX_CONNECTION_ATTEMPTS):
            try:
                sleep(0.02)
                self.socket.connect((IPV4_ADDRESS_SERVER, TCP_PORT))
            except ConnectionRefusedError:
                continue
            else:
                break
        else:
            raise ConnectionRefusedError

    def disconnect(self: T) -> None:

        LOGGER.debug('Disconnecting from socks server')
        self.socket.close()

    def send(self, command: str) -> str:

        LOGGER.debug('Sending message "%s" to server', command)
        self.socket.sendall(command.encode())

        try:
            bytes_recv = self.socket.recv(TCP_BUFFER_SIZE)
        except ConnectionResetError:
            LOGGER.debug('Could not send data to server. Is server up?')
            return ''

        message = bytes_recv.decode()
        LOGGER.debug('Received message "%s" from server', message)
        return message

    def stop_server(self: T) -> None:

        LOGGER.debug('Sending message "exit" to server')

        command = 'exit'
        self.socket.sendall(command.encode())


@pytest.fixture(scope='module')
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

@pytest.fixture(scope='module')
def socks_client() -> Client:

    client = Client()
    client.connect()

    yield client
    client.disconnect()
