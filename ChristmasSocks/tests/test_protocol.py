# pylint: disable=C0103  # Disable snake case warning

from unittest import TestCase
from utils import (
    Client,
    generate_random_string,
    generate_random_punctuation
)


class TestProtocolRandomStrings(TestCase):

    def setUp(self) -> None:
        self.client = Client()
        self.client.connect()

    def tearDown(self) -> None:
        self.client.disconnect()

    def test_echo_5_byte_string(self) -> None:
        for string in generate_random_string(num_strings=5, len_strings=5):
            with self.subTest():
                self.assertEqual(string, self.client.send(string))

    def test_echo_10_byte_string(self) -> None:
        for string in generate_random_string(num_strings=5, len_strings=10):
            with self.subTest():
                self.assertEqual(string, self.client.send(string))

    def test_echo_15_byte_string(self) -> None:
        for string in generate_random_string(num_strings=5, len_strings=15):
            with self.subTest():
                self.assertEqual(string, self.client.send(string))


class TestProtocolRandomPunctuation(TestCase):

    def setUp(self) -> None:
        self.client = Client()
        self.client.connect()

    def tearDown(self) -> None:
        self.client.disconnect()

    def test_echo_5_byte_punctuation(self) -> None:
        for string in generate_random_punctuation(num_strings=5, len_strings=5):
            with self.subTest():
                self.assertEqual(string, self.client.send(string))

    def test_echo_10_byte_punctuation(self) -> None:
        for string in generate_random_punctuation(num_strings=5, len_strings=10):
            with self.subTest():
                self.assertEqual(string, self.client.send(string))

    def test_echo_15_byte_punctuation(self) -> None:
        for string in generate_random_punctuation(num_strings=5, len_strings=15):
            with self.subTest():
                self.assertEqual(string, self.client.send(string))


class TestProtocolLimits(TestCase):

    def setUp(self) -> None:
        self.client = Client()
        self.client.connect()

        # Tell unittest to print large strings (diffs)
        self.maxDiff = None

        self.buffer_size = self.client.ini_configs['server'].getint('tcp_buffer_size') - 1

    def tearDown(self) -> None:
        self.client.disconnect()

    def test_echo_max_size_minus_one_byte_string(self) -> None:
        string = generate_random_string(num_strings=1, len_strings=self.buffer_size)
        self.assertEqual(string[0], self.client.send(string[0]))

    def test_echo_max_size_plus_five_bytes_string(self) -> None:
        string = generate_random_string(num_strings=1, len_strings=self.buffer_size + 5)
        first_chunk = string[0][0:1024]
        # second_chunk = string[0][1024:] # <- what happens to second chunk data?? memory leak??
        self.assertEqual(first_chunk, self.client.send(string[0]))

    def test_echo_max_size_plus_five_bytes_string_parametrized(self) -> None:
        for string in generate_random_punctuation(num_strings=5, len_strings=self.buffer_size + 5):
            with self.subTest():
                first_chunk = string[0][0:1024]
                self.assertEqual(first_chunk, self.client.send(string[0]))
