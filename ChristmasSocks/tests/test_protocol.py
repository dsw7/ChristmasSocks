from unittest import TestCase
from utils import (
    Connection,
    generate_random_string
)


class TestProtocolRandomStrings(TestCase):

    def test_echo_5_byte_string(self):
        socket = Connection()
        socket.connect()

        for string in generate_random_string(num_strings=5, len_strings=5):
            with self.subTest():
                self.assertEqual(string, socket.send(string))

        socket.disconnect()

    def test_echo_10_byte_string(self):
        socket = Connection()
        socket.connect()

        for string in generate_random_string(num_strings=10, len_strings=10):
            with self.subTest():
                self.assertEqual(string, socket.send(string))

        socket.disconnect()

    def test_echo_15_byte_string(self):
        socket = Connection()
        socket.connect()

        for string in generate_random_string(num_strings=15, len_strings=15):
            with self.subTest():
                self.assertEqual(string, socket.send(string))

        socket.disconnect()
