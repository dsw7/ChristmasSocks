# pylint: disable=W0201  # Disable defined outside __init__

from inspect import stack
from pytest import mark, raises
from utils import (
    ServerBackground,
    Client
)

@mark.release_test
class TestWhitelist:

    def setup_class(self) -> None:
        self.server = ServerBackground()
        self.client = Client()
        self.test_string = 'foobar'

    def teardown_method(self) -> None:
        self.server.stop_server()
        self.client.disconnect()

    def test_whitelist(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        whitelist = '127.0.0.1'
        self.server.start_server('-w', str(whitelist), logfile=logfile)
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_blacklist(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        whitelist = '127.0.0.2'
        self.server.start_server('-w', str(whitelist), logfile=logfile)
        self.client.connect()
        assert not self.client.send(self.test_string)
