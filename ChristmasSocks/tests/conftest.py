from logging import getLogger
from subprocess import Popen, STDOUT
from signal import SIGINT
from os import path, environ
from tempfile import gettempdir
import pytest

LOGGER = getLogger(__name__)
PATH_SOCKS_BIN = 'ChristmasSocks/bin/socks'

@pytest.fixture
def socks_server_basic() -> None:

    if not path.exists(PATH_SOCKS_BIN):
        pytest.exit(f'Binary {PATH_SOCKS_BIN} does not exist. Exiting!')

    try:
        current_test = environ['PYTEST_CURRENT_TEST']
    except KeyError:
        pytest.exit('Could not find PYTEST_CURRENT_TEST environment variable. Are you using pytest?')

    current_test = current_test.split('::')[0]
    current_test = path.splitext(current_test)[0]

    logfile = path.join(gettempdir(), current_test)
    logfile = f'{logfile}.log'

    LOGGER.debug('Will log to: %s', logfile)

    log_handle = open(logfile, 'w')
    process = Popen(PATH_SOCKS_BIN, stdout=log_handle, stderr=STDOUT)

    yield

    LOGGER.debug('Closing connection')

    process.send_signal(SIGINT)
    log_handle.close()
