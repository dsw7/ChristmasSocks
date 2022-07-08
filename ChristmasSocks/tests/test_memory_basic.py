from time import sleep
from logging import getLogger
from os import path
from subprocess import Popen, DEVNULL, STDOUT
import pytest
import consts
import utils
from client import Client

LOGGER = getLogger(__name__)


@pytest.mark.memory_test
class TestMemoryBasic:

    def setup_class(self) -> None:

        if not path.exists(consts.PATH_SOCKS_BIN):
            pytest.exit(f'Binary {consts.PATH_SOCKS_BIN} does not exist. Exiting!')

        logfile = utils.get_log_file_path(utils.get_current_test_module())
        LOGGER.debug('Will log to: %s', logfile)

        command = ['valgrind', '--leak-check=yes', f'--log-file={logfile}', consts.PATH_SOCKS_BIN]
        LOGGER.debug('Running command "%s"', ' '.join(command))

        Popen(command, stdout=DEVNULL, stderr=STDOUT)
        sleep(consts.STARTUP_DELAY_VALGRIND)

        self.client = Client()
        self.client.connect()

    def teardown_class(self) -> None:

        # Exit gracefully instead of using IPC signal...
        # Signals are terrible for debugging with Valgrind

        self.client.stop_server()
        self.client.disconnect()

    def test_simple_echo(self) -> None:
        assert 'foobar' == self.client.send('foobar')
