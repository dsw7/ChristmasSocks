# pylint: disable=W0201  # Disable defined outside __init__
# pylint: disable=E1101  # Disable has no '__name__' member

from pytest import mark
from utils import (
    Client,
    ServerValgrind
)

@mark.memory_test
class TestMemoryBasic:

    def setup_class(self) -> None:
        self.server = ServerValgrind()
        logfile = '{}.log'.format(self.__name__)
        self.server.start_server(logfile=logfile)
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
