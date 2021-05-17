from unittest import TestCase
from utils import (
    Connection,
    generate_random_string
)


class TestProtocolRandomStrings(TestCase):
    def setUp(self) -> None:
        self.socket = Connection()
        self.socket.start_server()
        self.socket.connect()

    def tearDown(self) -> None:
        self.socket.disconnect()
        self.socket.stop_server()

    def test_echo_5_byte_string(self) -> None:
        for string in generate_random_string(num_strings=5, len_strings=5):
            with self.subTest():
                self.assertEqual(string, self.socket.send(string))

    def test_echo_10_byte_string(self) -> None:
        for string in generate_random_string(num_strings=5, len_strings=10):
            with self.subTest():
                self.assertEqual(string, self.socket.send(string))

    def test_echo_15_byte_string(self) -> None:
        for string in generate_random_string(num_strings=5, len_strings=15):
            with self.subTest():
                self.assertEqual(string, self.socket.send(string))
