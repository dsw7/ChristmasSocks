from time import sleep
from pytest import mark

@mark.release_test
def test_command_exit(socks_server_background, socks_client) -> None:
    string = 'foobar'
    assert socks_client.send(string) == string

    # Cannot simply check if /proc/<pid> no longer exists because
    # python keeps the process lingering as a zombie

    socks_client.send('exit')
    sleep(0.10)

    assert socks_client.send(string) == ''
