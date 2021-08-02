# pylint: disable=W0201  # Disable defined outside __init__

from pytest import mark
from utils import Server, EXIT_FAILURE

@mark.release_test
class TestInvalidParameters:

    def setup_class(self) -> None:
        self.server = Server()

    def test_invalid_tcp_port_low(self) -> None:
        assert self.server.start_server_in_foreground('--port=22') == EXIT_FAILURE

    def test_invalid_tcp_port_high(self) -> None:
        assert self.server.start_server_in_foreground('--port=99999') == EXIT_FAILURE

    def test_invalid_tcp_buffer_size(self) -> None:
        assert self.server.start_server_in_foreground('--buffer-size=4') == EXIT_FAILURE
