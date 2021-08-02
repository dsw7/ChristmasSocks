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
        self.server.stop_server()
        self.client.disconnect()

    def test_strip_line_breaks(self) -> None:
        self.client.connect()
        self.server.start_server_in_background('--strip-line-breaks')
        self.client.send('sleep\n')

    def test_no_strip_line_breaks(self) -> None:
        self.client.connect()
        self.server.start_server_in_background()
        self.client.send('sleep\n')
