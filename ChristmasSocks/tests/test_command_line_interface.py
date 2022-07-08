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
class TestCommandLineInterface:

    def setup_class(self) -> None:

        self.process = None
        self.log_handle = None

        self.client = Client()
        self.test_string = 'foobar'

    def setup_method(self) -> None:

        log_filepath = utils.get_log_file_path(utils.get_current_test_method())
        logger.debug('Will log to: %s', log_filepath)

        self.log_handle = open(log_filepath, 'w')

    def teardown_method(self) -> None:

        self.client.disconnect()
        sleep(0.05)

        self.process.send_signal(SIGINT)
        self.log_handle.close()

    def test_valid_port_long_option(self) -> None:

        self.process = Popen([consts.PATH_SOCKS_BIN, f'--port={consts.TCP_PORT}'], stdout=self.log_handle)
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_valid_port_short_option(self) -> None:

        self.process = Popen([consts.PATH_SOCKS_BIN, '-p', f'{consts.TCP_PORT}'], stdout=self.log_handle)
        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_buffer_size_long_option(self) -> None:
        self.process = Popen([consts.PATH_SOCKS_BIN, '--buffer-size=1024'], stdout=self.log_handle)

        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_buffer_size_short_option(self) -> None:
        self.process = Popen([consts.PATH_SOCKS_BIN, '-b', '1024'], stdout=self.log_handle)

        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_bind_ip_long_option(self) -> None:
        self.process = Popen([consts.PATH_SOCKS_BIN, '--bind-ip=127.0.0.1'], stdout=self.log_handle)

        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_bind_ip_short_option(self) -> None:
        self.process = Popen([consts.PATH_SOCKS_BIN, '-i', '127.0.0.1'], stdout=self.log_handle)

        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_master_long_option(self) -> None:
        self.process = Popen([consts.PATH_SOCKS_BIN, '--master=127.0.0.1'], stdout=self.log_handle)

        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)

    def test_master_short_option(self) -> None:
        self.process = Popen([consts.PATH_SOCKS_BIN, '-w', '127.0.0.1'], stdout=self.log_handle)

        self.client.connect()
        assert self.test_string == self.client.send(self.test_string)
