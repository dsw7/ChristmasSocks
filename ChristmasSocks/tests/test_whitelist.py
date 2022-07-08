from time import sleep
from logging import getLogger
from subprocess import Popen
from signal import SIGINT
import pytest
from client import Client
import utils
import consts

logger = getLogger(__name__)


@pytest.mark.release_test
class TestWhitelist:

    def setup_class(self) -> None:

        self.process = None
        self.log_handle = None

        self.client = Client()

    def setup_method(self) -> None:

        log_filepath = utils.get_log_file_path(utils.get_current_test_method())
        logger.debug('Will log to: %s', log_filepath)

        self.log_handle = open(log_filepath, 'w')

    def teardown_method(self) -> None:

        self.client.disconnect()
        sleep(0.05)

        self.process.send_signal(SIGINT)
        self.log_handle.close()

    def test_master(self) -> None:
        self.process = Popen([consts.PATH_SOCKS_BIN, '-w', '127.0.0.1'], stdout=self.log_handle)

        self.client.connect()
        assert 'foobar' == self.client.send('foobar')

    def test_blacklist(self) -> None:
        self.process = Popen([consts.PATH_SOCKS_BIN, '-w', '127.0.0.2'], stdout=self.log_handle)

        self.client.connect()
        assert not self.client.send('foobar')
