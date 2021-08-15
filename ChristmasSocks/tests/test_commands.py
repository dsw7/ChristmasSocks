# pylint: disable=W0201  # Disable defined outside __init__

from pytest import mark
from utils import (
    Client,
    ServerBackground
)

@mark.release_test
class TestExitCommand:

    def setup_class(self) -> None:
        self.server = ServerBackground()
        self.server.start_server(logfile='{}.log'.format(self.__name__))
        self.client = Client()
        self.client.connect()

    def teardown_class(self) -> None:
        self.client.disconnect()

    def test_process_is_dead_after_sending_exit(self) -> None:
        string = 'foobar'
        assert string == self.client.send(string)
        self.client.send('exit')

        # Cannot simply check if /proc/self.server.process.pid no longer exists because
        # python keeps the process lingering as a zombie
        assert self.client.send(string) == ''

@mark.release_test
class TestCommands:

    def setup_class(self) -> None:
        self.server = ServerBackground()
        self.server.start_server(logfile='{}.log'.format(self.__name__))
        self.client = Client()
        self.client.connect()

    def teardown_class(self) -> None:
        self.client.disconnect()
        self.server.stop_server()

    def test_sleep(self) -> None:
        assert self.client.send('sleep') == 'Success'

    def test_help(self) -> None:
        assert '> List of commands:' in self.client.send('help')
