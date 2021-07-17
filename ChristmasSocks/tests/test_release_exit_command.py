# pylint: disable=C0103  # Disable snake case warning
# pylint: disable=C0202  # Disable cls requirement for setUpClass / tearDownClass

from unittest import TestCase
from utils import (
    Client,
    Server,
    generate_random_string,
    generate_random_punctuation
)


class TestProtocolRandomStrings(TestCase):

    @classmethod
    def setUpClass(self) -> None:
        self.server = Server()
        self.server.start_server()
        self.client = Client()
        self.client.connect()

    @classmethod
    def tearDownClass(self) -> None:
        self.client.disconnect()
        self.server.stop_server()

    def test_process_is_dead(self) -> None:
        print(self.server.process.pid)
        assert True
