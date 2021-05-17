# pylint: disable=C0103  # Disable snake case warning

from unittest import TestCase
from utils import (
    Client,
    Server,
    generate_random_string
)


class TestProtocolRandomStrings(TestCase):

#    @classmethod
#    def setUpClass(self) -> None:
#        self.server = Server()
#        self.server.start_server()
#
#    @classmethod
#    def tearDownClass(self) -> None:
#        self.server.stop_server()

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
