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

    def test_handle_line_breaks(self) -> None:
        self.server.start_server_in_background('--handle-line-breaks')
        self.client.connect()
        assert self.client.send('foobar\n') == 'foobar\n'

    def test_no_handle_line_breaks(self) -> None:
        self.server.start_server_in_background()
        self.client.connect()
        assert self.client.send('foobar\n') == 'foobar\n'
