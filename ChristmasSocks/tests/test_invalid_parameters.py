# pylint: disable=W0201  # Disable defined outside __init__

from pytest import mark
from utils import ServerForeground, EXIT_FAILURE

@mark.release_test
class TestInvalidParameters:

    def setup_class(self) -> None:
        self.server = ServerForeground(logfile='{}.log'.format(self.__name__))

    def test_invalid_tcp_port_low(self) -> None:
        assert self.server.start_server('--port=22') == EXIT_FAILURE

    def test_invalid_tcp_port_high(self) -> None:
        assert self.server.start_server('--port=99999') == EXIT_FAILURE

    def test_invalid_tcp_buffer_size(self) -> None:
        assert self.server.start_server('--buffer-size=4') == EXIT_FAILURE
