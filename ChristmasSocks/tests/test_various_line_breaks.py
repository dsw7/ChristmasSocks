# pylint: disable=W0201  # Disable defined outside __init__

from inspect import stack
from pytest import mark
from utils import (
    ServerBackground,
    Client
)

@mark.release_test
class TestHandleLineBreaks:

    def setup_class(self) -> None:
        self.server = ServerBackground()
        self.client = Client()

    def teardown_method(self) -> None:
        self.client.disconnect()
        self.server.stop_server()

    def test_handle_line_feed(self) -> None:
        self.server.start_server(logfile='{}.log'.format(stack()[0][3]))
        self.client.connect()
        assert self.client.send('foobar\n') == 'foobar\n'

    def test_handle_carriage_return(self) -> None:
        self.server.start_server(logfile='{}.log'.format(stack()[0][3]))
        self.client.connect()
        assert self.client.send('foobar\r') == 'foobar\r'

    def test_handle_end_of_line(self) -> None:
        self.server.start_server(logfile='{}.log'.format(stack()[0][3]))
        self.client.connect()
        assert self.client.send('foobar\r\n') == 'foobar\r\n'

    def test_handle_no_line_break(self) -> None:
        self.server.start_server(logfile='{}.log'.format(stack()[0][3]))
        self.client.connect()
        assert self.client.send('foobar') == 'foobar'
