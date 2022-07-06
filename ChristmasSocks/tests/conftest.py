from os import path
import pytest

PATH_SOCKS_BIN = 'ChristmasSocks/bin/socks'

@pytest.fixture
def socks_server() -> None:

    if not path.exists(PATH_SOCKS_BIN):
        pytest.exit(f'Binary {PATH_SOCKS_BIN} does not exist. Exiting!')

    yield
    print('Tearing down fixture')
