import subprocess
from pytest import mark, raises
import consts

@mark.release_test
def test_block_second_server_from_starting(socks_server_background) -> None:
    with raises(subprocess.CalledProcessError):
        subprocess.run([consts.PATH_SOCKS_BIN], capture_output=True, check=True)
