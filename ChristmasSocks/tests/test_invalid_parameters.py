import subprocess
from pytest import mark, raises
import consts

@mark.release_test
def test_invalid_tcp_port_negative() -> None:
    with raises(subprocess.CalledProcessError):
        subprocess.run([consts.PATH_SOCKS_BIN, '--port=-100'], capture_output=True, check=True)

@mark.release_test
def test_invalid_tcp_port_low() -> None:
    with raises(subprocess.CalledProcessError):
        subprocess.run([consts.PATH_SOCKS_BIN, '--port=22'], capture_output=True, check=True)

@mark.release_test
def test_invalid_tcp_port_high() -> None:
    with raises(subprocess.CalledProcessError):
        subprocess.run([consts.PATH_SOCKS_BIN, '--port=99999'], capture_output=True, check=True)

@mark.release_test
def test_invalid_tcp_buffer_size() -> None:
    with raises(subprocess.CalledProcessError):
        subprocess.run([consts.PATH_SOCKS_BIN, '--buffer-size=4'], capture_output=True, check=True)

@mark.release_test
def test_invalid_tcp_buffer_size_negative() -> None:
    with raises(subprocess.CalledProcessError):
        subprocess.run([consts.PATH_SOCKS_BIN, '--buffer-size=-100'], capture_output=True, check=True)

@mark.release_test
def test_invalid_bind_ip() -> None:
    with raises(subprocess.CalledProcessError):
        subprocess.run([consts.PATH_SOCKS_BIN, '--bind-ip=1.2.3'], capture_output=True, check=True)
