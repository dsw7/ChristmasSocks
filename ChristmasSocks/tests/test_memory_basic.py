# pylint: disable=W0201  # Disable defined outside __init__

from pytest import mark
from utils import (
    Client,
    Server
)

@mark.memory
class TestMemoryBasic:

    def setup_class(self) -> None:
        log_file = '{}.log'.format('test_simple_echo')
        self.server = Server()
        self.server.start_server_under_valgrind(log_file=log_file)
        self.client = Client()
        self.client.connect()

    def teardown_class(self) -> None:
        # Exit gracefully instead of using IPC signal...
        # Signals are terrible for debugging with Valgrind
        self.client.stop_server()
        self.client.disconnect()

    def test_simple_echo(self) -> None:
        string = 'foobar'
        assert string == self.client.send(string)
