# pylint: disable=C0103  # Disable snake case warning
# pylint: disable=C0202  # Disable cls requirement for setUpClass / tearDownClass

from unittest import TestCase
from utils import (
    Client,
    Server
)

class TestExitCommand(TestCase):

    @classmethod
    def setUpClass(self) -> None:
        self.server = Server()
        self.server.start_server()
        self.client = Client()
        self.client.connect()

    @classmethod
    def tearDownClass(self) -> None:
        self.client.disconnect()

    def test_process_is_dead_after_sending_exit(self) -> None:
        string = 'foobar'
        self.assertEqual(string, self.client.send(string))
        self.client.send('exit')

        # Cannot simply check if /proc/self.server.process.pid no longer exists because
        # python keeps the process lingering as a zombie
        self.assertEqual('', self.client.send(string))
