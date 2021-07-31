# pylint: disable=C0103  # Disable snake case warning
# pylint: disable=C0202  # Disable cls requirement for setUpClass / tearDownClass

from unittest import TestCase
from utils import Server, Client


class TestCommandLineInterface(TestCase):

    @classmethod
    def setUpClass(self) -> None:
        self.server = Server()
        self.client = Client()

    @classmethod
    def tearDown(self) -> None:
        self.server.stop_server()
        self.client.disconnect()

    def test_invalid_port_low_long_option(self) -> None:
        port = 22
        self.server.start_server('--port={}'.format(port))
        with self.assertRaises(OSError):
            self.client.connect(port=port)

    def test_invalid_port_high_long_option(self) -> None:
        self.server.start_server('--port=99999')
        self.assertIsNone(self.server.process.pid)

    def test_valid_port_long_option(self) -> None:
        self.server.start_server('--port=8080')
        self.assertTrue(self.server.process.pid > 0)

    def test_invalid_port_low_short_option(self) -> None:
        self.server.start_server('-p', '22')
        self.assertIsNone(self.server.process.pid)

    def test_invalid_port_high_short_option(self) -> None:
        self.server.start_server('-p', '99999')
        self.assertIsNone(self.server.process.pid)

    def test_valid_port_short_option(self) -> None:
        self.server.start_server('-p', '8080')
        self.assertTrue(self.server.process.pid > 0)
