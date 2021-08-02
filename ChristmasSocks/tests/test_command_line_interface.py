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

    def test_valid_port_long_option(self) -> None:
        port = 8080
        string = 'foobar'
        self.server.start_server('--port={}'.format(port))
        self.client.connect(port=port)
        assert string == self.client.send(string)

    def test_valid_port_short_option(self) -> None:
        port = 8080
        string = 'foobar'
        self.server.start_server('-p', str(port))
        self.client.connect(port=port)
        assert string == self.client.send(string)
