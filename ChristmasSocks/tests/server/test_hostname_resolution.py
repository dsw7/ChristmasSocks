# pylint: disable=W0201  # Disable defined outside __init__

from inspect import stack
from pytest import mark
from utils import (
    ServerBackground,
    Client
)


@mark.release_test
class TestHostnameResolution:

    def setup_class(self) -> None:
        self.server = ServerBackground()
        self.client = Client()
        self.test_string = 'foobar'

    def teardown_class(self) -> None:
        self.server.stop_server()
        self.client.disconnect()

    def test_resolve_hostname_long_option(self) -> None:
        logfile = '{}.log'.format(stack()[0][3])
        self.server.start_server('--master=localhost', logfile=logfile)
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)
