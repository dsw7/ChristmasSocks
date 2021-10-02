# pylint: disable=W0201  # Disable defined outside __init__

from inspect import stack
from pytest import mark
from utils import (
    ServerForeground,
    ServerBackground,
    Client,
    EXIT_SUCCESS
)

@mark.release_test
class TestHelpMenu:

    def setup_class(self) -> None:
        self.server = ServerForeground()

    def test_help_long_option(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        assert self.server.start_server('--help', logfile=logfile) == EXIT_SUCCESS

    def test_help_short_option(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        assert self.server.start_server('-h', logfile=logfile) == EXIT_SUCCESS


@mark.release_test
class TestCommandLineInterface:

    def setup_class(self) -> None:
        self.server = ServerBackground()
        self.client = Client()
        self.test_string = 'foobar'

    def teardown_method(self) -> None:
        self.server.stop_server()
        self.client.disconnect()

    def test_valid_port_long_option(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        port = 8080
        self.server.start_server('--port={}'.format(port), logfile=logfile)
        self.client.connect(port=port)
        assert self.test_string == self.client.send(self.test_string)

    def test_valid_port_short_option(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        port = 8080
        self.server.start_server('-p', str(port), logfile=logfile)
        self.client.connect(port=port)
        assert self.test_string == self.client.send(self.test_string)

    def test_buffer_size_long_option(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        buffer_size = 1024
        self.server.start_server('--buffer-size={}'.format(buffer_size), logfile=logfile)
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_buffer_size_short_option(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        buffer_size = 1024
        self.server.start_server('-b', str(buffer_size), logfile=logfile)
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_bind_ip_long_option(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        bind_ip = '127.0.0.1'
        self.server.start_server('--bind-ip={}'.format(bind_ip), logfile=logfile)
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_bind_ip_short_option(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        bind_ip = '127.0.0.1'
        self.server.start_server('-i', str(bind_ip), logfile=logfile)
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_whitelist_long_option(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        whitelist = '127.0.0.1'
        self.server.start_server('--whitelist={}'.format(whitelist), logfile=logfile)
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_whitelist_short_option(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        whitelist = '127.0.0.1'
        self.server.start_server('-w', str(whitelist), logfile=logfile)
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)
