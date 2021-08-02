# pylint: disable=W0201  # Disable defined outside __init__

from pytest import mark
from utils import (
    Server,
    Client
)

@mark.release_test
class TestCommandLineInterface:

    def setup_class(self) -> None:
        self.server = Server()
        self.client = Client()
        self.test_string = 'foobar'

    def teardown_method(self) -> None:
        self.server.stop_server()
        self.client.disconnect()

    def test_valid_port_long_option(self) -> None:
        port = 8080
        self.server.start_server('--port={}'.format(port))
        self.client.connect(port=port)
        assert self.test_string == self.client.send(self.test_string)

    def test_valid_port_short_option(self) -> None:
        port = 8080
        self.server.start_server('-p', str(port))
        self.client.connect(port=port)
        assert self.test_string == self.client.send(self.test_string)

    def test_buffer_size_long_option(self) -> None:
        buffer_size = 1024
        self.server.start_server('--buffer-size={}'.format(buffer_size))
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_buffer_size_short_option(self) -> None:
        buffer_size = 1024
        self.server.start_server('-b', str(buffer_size))
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)
