# pylint: disable=W0201  # Disable defined outside __init__

from pytest import mark
from utils import (
    Server,
    Client,
    EXIT_SUCCESS
)

@mark.release_test
class TestHelpMenu:

    def setup_class(self) -> None:
        self.server = Server()

    def test_help_long_option(self) -> None:
        assert self.server.start_server_in_foreground('--help') == EXIT_SUCCESS

    def test_help_short_option(self) -> None:
        assert self.server.start_server_in_foreground('-h') == EXIT_SUCCESS


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
        self.server.start_server_in_background('--port={}'.format(port))
        self.client.connect(port=port)
        assert self.test_string == self.client.send(self.test_string)

    def test_valid_port_short_option(self) -> None:
        port = 8080
        self.server.start_server_in_background('-p', str(port))
        self.client.connect(port=port)
        assert self.test_string == self.client.send(self.test_string)

    def test_buffer_size_long_option(self) -> None:
        buffer_size = 1024
        self.server.start_server_in_background('--buffer-size={}'.format(buffer_size))
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_buffer_size_short_option(self) -> None:
        buffer_size = 1024
        self.server.start_server_in_background('-b', str(buffer_size))
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_bind_ip_long_option(self) -> None:
        bind_ip = '127.0.0.1'
        self.server.start_server_in_background('--bind-ip={}'.format(bind_ip))
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_bind_ip_short_option(self) -> None:
        bind_ip = '127.0.0.1'
        self.server.start_server_in_background('-i', str(bind_ip))
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)
