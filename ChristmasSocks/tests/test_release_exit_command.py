# pylint: disable=C0103  # Disable snake case warning
# pylint: disable=C0202  # Disable cls requirement for setUpClass / tearDownClass

from unittest import TestCase
from utils import (
    Client,
    Server,
    is_process_running
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

    def test_process_is_dead_after_sending_exit(self) -> None:
        self.client.send('exit')
        self.assertFalse(is_process_running(self.server.process.pid))
