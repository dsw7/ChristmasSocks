# pylint: disable=C0103  # Disable snake case warning
# pylint: disable=C0202  # Disable cls requirement for setUpClass / tearDownClass

from unittest import TestCase
from utils import (
    Client,
    Server
)

class TestMemoryBasic(TestCase):

    @classmethod
    def setUpClass(self) -> None:
        print(self.__class__)
        self.server = Server()
        self.server.start_server_under_valgrind()
        self.client = Client()
        self.client.connect()

    @classmethod
    def tearDownClass(self) -> None:
        # Exit gracefully instead of using IPC signal...
        # Signals are terrible for debugging with Valgrind
        self.client.stop_server()
        self.client.disconnect()

    def test_echo_6_bytes(self) -> None:
        string = 'foobar'
        self.assertEqual(string, self.client.send(string))
