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
        self.server = Server()

    @classmethod
    def tearDown(self) -> None:
        self.server.stop_server()

    def test_invalid_port_low_long_option(self) -> None:
        self.server.start_server('--port=22')
        self.assertIsNone(self.server.process.pid)

    def test_invalid_port_high_long_option(self) -> None:
        self.server.start_server('--port=99999')
        self.assertIsNone(self.server.process.pid)

    def test_valid_port_long_option(self) -> None:
        self.server.start_server('--port=8080')
        self.assertTrue(self.server.process.pid > 0)
