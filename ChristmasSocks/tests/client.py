from logging import getLogger
from time import sleep
from socket import socket, AF_INET, SOCK_STREAM
from typing import TypeVar
import consts

T = TypeVar('T')
LOGGER = getLogger(__name__)


class Client:

    def __init__(self: T) -> T:
        self.socket = None

    def connect(self: T) -> None:

        LOGGER.debug('Connecting to socks server on port %i and host %s', consts.TCP_PORT, consts.IPV4_ADDRESS_SERVER)

        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.settimeout(consts.SOCK_TIMEOUT)

        for _ in range(consts.MAX_CONNECTION_ATTEMPTS):
            try:
                sleep(0.02)
                self.socket.connect((consts.IPV4_ADDRESS_SERVER, consts.TCP_PORT))
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
            bytes_recv = self.socket.recv(consts.TCP_BUFFER_SIZE)
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
