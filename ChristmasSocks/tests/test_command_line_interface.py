# pylint: disable=W0201  # Disable defined outside __init__

from pytest import (
    mark,
    raises
)
from utils import (
    Server,
    Client
)

@mark.release_test
class TestCommandLineInterface:

    def setup_class(self) -> None:
        self.server = Server()
        self.client = Client()

    def teardown_method(self) -> None:
        self.server.stop_server()
        self.client.disconnect()

    def test_invalid_port_low_long_option(self) -> None:
        port = 22
        self.server.start_server('--port={}'.format(port))
        with raises(OSError):
            self.client.connect(port=port)

    def test_invalid_port_high_long_option(self) -> None:
        port = 99999
        self.server.start_server('--port={}'.format(port))
        with raises(OSError):
            self.client.connect(port=port)

    def test_valid_port_long_option(self) -> None:
        port = 8080
        string = 'foobar'
        self.server.start_server('--port={}'.format(port))
        self.client.connect(port=port)
        assert string == self.client.send(string)

    def test_invalid_port_low_short_option(self) -> None:
        port = 22
        self.server.start_server('-p', str(port))
        with raises(OSError):
            self.client.connect(port=port)

    def test_invalid_port_high_short_option(self) -> None:
        port = 99999
        self.server.start_server('-p', str(port))
        with raises(OSError):
            self.client.connect(port=port)

    def test_valid_port_short_option(self) -> None:
        port = 8080
        string = 'foobar'
        self.server.start_server('-p', str(port))
        self.client.connect(port=port)
        assert string == self.client.send(string)
