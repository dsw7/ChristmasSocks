import subprocess
from os import EX_OK
from pytest import mark
import consts

@mark.release_test
def test_help_long_option() -> None:
    completed_process = subprocess.run([consts.PATH_SOCKS_BIN, '--help'], capture_output=True, check=True)

    assert completed_process.returncode == EX_OK
    assert 'Synopsis' in completed_process.stdout.decode()

@mark.release_test
def test_help_short_option() -> None:
    completed_process = subprocess.run([consts.PATH_SOCKS_BIN, '-h'], capture_output=True, check=True)

    assert completed_process.returncode == EX_OK
    assert 'Synopsis' in completed_process.stdout.decode()
