from logging import getLogger
from time import sleep
from subprocess import Popen, STDOUT
from signal import SIGINT
from os import path, environ
from tempfile import gettempdir
from typing import TypeVar, Tuple
import pytest
import consts
from client import Client

T = TypeVar('T')
LOGGER = getLogger(__name__)

@pytest.fixture(scope='module')
def socks_server_background() -> None:

    if not path.exists(consts.PATH_SOCKS_BIN):
        pytest.exit(f'Binary {consts.PATH_SOCKS_BIN} does not exist. Exiting!')

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
    process = Popen(consts.PATH_SOCKS_BIN, stdout=log_handle, stderr=STDOUT)

    yield

    LOGGER.debug('Closing connection')
    sleep(consts.SERVER_SHUTDOWN_DELAY)

    process.send_signal(SIGINT)
    log_handle.close()

@pytest.fixture(scope='module')
def socks_client() -> Client:

    client = Client()
    client.connect()

    yield client
    client.disconnect()

@pytest.fixture(scope='module')
def socks_three_clients() -> Tuple[Client]:

    client_a = Client()
    client_b = Client()
    client_c = Client()

    client_a.connect()
    client_b.connect()
    client_c.connect()

    yield client_a, client_b, client_c

    client_a.disconnect()
    client_b.disconnect()
    client_c.disconnect()
