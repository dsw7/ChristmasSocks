# pylint: disable=C0103  # Disable snake case warning
# pylint: disable=C0202  # Disable cls requirement for setUpClass / tearDownClass

from unittest import TestCase
from utils import (
    Client,
    Server,
    generate_random_string,
    generate_random_punctuation
)


class TestCommandLineInterface(TestCase):

    @classmethod
    def setUpClass(self) -> None:
        #self.client = Client()
        #self.client.connect()
        self.server = Server()

    @classmethod
    def tearDownClass(self) -> None:
        #self.client.disconnect()
        self.server.stop_server()

    def test_invalid_port_low(self) -> None:
        self.server.start_server('--port=22')
        print(self.server.process.pid)
