from logging import getLogger
from time import sleep
from subprocess import Popen, STDOUT
from signal import SIGINT
from os import path
from typing import TypeVar, Dict
import pytest
import consts
import utils
from client import Client

T = TypeVar('T')
LOGGER = getLogger(__name__)

@pytest.fixture(scope='module')
def socks_server_background() -> None:

    if not path.exists(consts.PATH_SOCKS_BIN):
        pytest.exit(f'Binary {consts.PATH_SOCKS_BIN} does not exist. Exiting!')

    logfile = utils.get_log_file_path(utils.get_current_test_module())
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
def socks_three_clients() -> Dict[str, Client]:

    client_a = Client()
    client_b = Client()
    client_c = Client()

    client_a.connect()
    client_b.connect()
    client_c.connect()

    yield {
        'client_a': client_a,
        'client_b': client_b,
        'client_c': client_c
    }

    client_a.disconnect()
    client_b.disconnect()
    client_c.disconnect()
