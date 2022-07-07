from pytest import mark
import utils
import consts

TCP_BUFFER_SIZE = consts.TCP_BUFFER_SIZE - 1

@mark.release_test
@mark.parametrize('string', utils.generate_random_string(num_strings=10, len_strings=15))
def test_echo_15_byte_string(socks_server_background, socks_client, string) -> None:
    assert string == socks_client.send(string)

@mark.release_test
@mark.parametrize('string', utils.generate_random_punctuation(num_strings=10, len_strings=15))
def test_echo_15_byte_punctuation(socks_server_background, socks_client, string) -> None:
    assert string == socks_client.send(string)

@mark.release_test
def test_echo_max_size_minus_one_byte_string(socks_server_background, socks_client) -> None:

    string = utils.generate_random_string(num_strings=1, len_strings=TCP_BUFFER_SIZE)
    assert string[0] == socks_client.send(string[0])

@mark.release_test
def test_echo_max_size_plus_five_bytes_string(socks_server_background, socks_client) -> None:

    string = utils.generate_random_string(num_strings=1, len_strings=TCP_BUFFER_SIZE + 5)
    first_chunk = string[0][0:1024]

    # XXX What happens to string[0][1024:] data?? looks like this could be a memory leak
    assert first_chunk == socks_client.send(string[0])
