# pylint: disable=W0201  # Disable defined outside __init__
# pylint: disable=E1101  # Disable has no '__name__' member

from pytest import mark
from utils import (
    Client,
    ServerBackground,
    generate_random_string,
    generate_random_punctuation
)

@mark.release_test
class TestProtocolRandomStrings:

    def setup_class(self) -> None:
        self.server = ServerBackground()
        self.server.start_server(logfile='{}.log'.format(self.__name__))
        self.client = Client()
        self.client.connect()
        self.buffer_size = self.client.configs['client'].getint('tcp_buffer_size') - 1

    def teardown_class(self) -> None:
        self.client.disconnect()
        self.server.stop_server()

    @mark.parametrize('string', generate_random_string(num_strings=10, len_strings=15))
    def test_echo_15_byte_string(self, string) -> None:
        assert string == self.client.send(string)

    @mark.parametrize('string', generate_random_punctuation(num_strings=10, len_strings=15))
    def test_echo_15_byte_punctuation(self, string) -> None:
        assert string == self.client.send(string)

    def test_echo_max_size_minus_one_byte_string(self) -> None:
        string = generate_random_string(num_strings=1, len_strings=self.buffer_size)
        assert string[0] == self.client.send(string[0])

    def test_echo_max_size_plus_five_bytes_string(self) -> None:
        string = generate_random_string(num_strings=1, len_strings=self.buffer_size + 5)
        first_chunk = string[0][0:1024]
        # XXX What happens to string[0][1024:] data?? looks like this could be a memory leak
        assert first_chunk == self.client.send(string[0])