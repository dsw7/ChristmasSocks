from pytest import mark

@mark.release_test
def test_handle_line_feed(socks_server_background, socks_client) -> None:
    assert socks_client.send('foobar\n') == 'foobar\n'

@mark.release_test
def test_handle_carriage_return(socks_server_background, socks_client) -> None:
    assert socks_client.send('foobar\r') == 'foobar\r'

@mark.release_test
def test_handle_end_of_line(socks_server_background, socks_client) -> None:
    assert socks_client.send('foobar\r\n') == 'foobar\r\n'

@mark.release_test
def test_handle_no_line_break(socks_server_background, socks_client) -> None:
    assert socks_client.send('foobar') == 'foobar'
