# pylint: disable=W0201  # Disable defined outside __init__

from inspect import stack
from pytest import mark
from utils import ServerForeground, EXIT_FAILURE

@mark.release_test
class TestInvalidParameters:

    def setup_class(self) -> None:
        self.server = ServerForeground()

    def test_invalid_tcp_port_low(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        assert self.server.start_server('--port=22', logfile=logfile) == EXIT_FAILURE

    def test_invalid_tcp_port_high(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        assert self.server.start_server('--port=99999', logfile=logfile) == EXIT_FAILURE

    def test_invalid_tcp_buffer_size(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        assert self.server.start_server('--buffer-size=4', logfile=logfile) == EXIT_FAILURE

    def test_invalid_tcp_buffer_size_negative(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        assert self.server.start_server('--buffer-size=-100', logfile=logfile) == EXIT_FAILURE
