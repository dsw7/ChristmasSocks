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
class TestHostnameResolution:

    def setup_method(self) -> None:

        self.process = None
        self.log_handle = None

        self.client = Client()
        self.test_string = 'foobar'

        log_filepath = utils.get_log_file_path(utils.get_current_test_method())
        logger.debug('Will log to: %s', log_filepath)

        self.log_handle = open(log_filepath, 'w')

    def teardown_method(self) -> None:

        self.client.disconnect()
        sleep(0.05)

        self.process.send_signal(SIGINT)
        self.log_handle.close()

    def test_hostname_resolution(self) -> None:
        self.process = Popen([consts.PATH_SOCKS_BIN, f'--master=localhost'], stdout=self.log_handle)

        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)
