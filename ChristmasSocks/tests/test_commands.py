# pylint: disable=W0201  # Disable defined outside __init__

from pytest import mark
from utils import (
    Client,
    Server
)

@mark.release_test
class TestExitCommand:

    def setup_class(self) -> None:
        self.server = Server()
        self.server.start_server_in_background()
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
        self.server = Server()
        self.server.start_server_in_background()
        self.client = Client()
        self.client.connect()

    def teardown_class(self) -> None:
        self.client.disconnect()

    def test_sleep(self) -> None:
        assert 'sleep' == self.client.send('sleep')
