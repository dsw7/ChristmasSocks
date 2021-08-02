# pylint: disable=W0201  # Disable defined outside __init__

from pytest import mark
from utils import (
    Server,
    Client
)

@mark.release_test
class TestLineBreakToggler:

    def setup_class(self) -> None:
        self.server = Server()
        self.client = Client()

    def teardown_method(self) -> None:
        self.client.disconnect()
        self.server.stop_server()

    def test_strip_line_breaks(self) -> None:
        self.server.start_server_in_background('--strip-line-breaks')
        self.client.connect()
        self.client.send('foobar\n')

    def test_no_strip_line_breaks(self) -> None:
        self.server.start_server_in_background()
        self.client.connect()
        self.client.send('foobar\n')