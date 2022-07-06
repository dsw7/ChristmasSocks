import platform
from re import compile

def test_sleep(socks_server_background, socks_client) -> None:
    assert socks_client.send('sleep') == 'Success'

def test_help(socks_server_background, socks_client) -> None:
    assert '> List of commands:' in socks_client.send('help')

def test_uptime(socks_server_background, socks_client) -> None: # Ensure that a string in HH:MM:SS form is returned
    pattern = compile(r'[0-9]{2}:[0-9]{2}:[0-9]{2}')
    assert pattern.match(socks_client.send('uptime'))

def test_get_kernel_info(socks_server_background, socks_client) -> None:
    entries = {}

    for entry in socks_client.send('sysinfo').split('\n'):
        if not entry:
            continue

        key, value = entry.split(': ')
        entries[key] = value

    assert entries['System'] == platform.system()
    assert entries['Node'] == platform.node()
    assert entries['Release'] == platform.release()
    assert entries['Version'] == platform.version()
    assert entries['Machine'] == platform.machine()
