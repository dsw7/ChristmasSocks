# pylint: disable=C0103  # Disable snake case warning

from unittest import TestCase
from concurrent import futures
from utils import (
    Client,
    generate_random_string
)


class TestMultipleConnections(TestCase):

    def setUp(self) -> None:
        self.client_a = Client()
        self.client_b = Client()
        self.client_c = Client()
        self.client_a.connect()
        self.client_b.connect()
        self.client_c.connect()

    def tearDown(self) -> None:
        self.client_a.disconnect()
        self.client_b.disconnect()
        self.client_c.disconnect()

    @staticmethod
    def wrap_send(test_string: str, client: Client) -> dict:
        return {
            'outgoing_message': test_string,
            'returning_message': client.send(test_string)
        }

    def test_echo_3_connections(self) -> None:
        strings = generate_random_string(num_strings=3, len_strings=5)

        workers, results = [], []
        with futures.ThreadPoolExecutor() as executor:
            workers.append(executor.submit(self.wrap_send, strings[0], self.client_a))
            workers.append(executor.submit(self.wrap_send, strings[1], self.client_b))
            workers.append(executor.submit(self.wrap_send, strings[2], self.client_c))

            if futures.wait(workers, return_when=futures.FIRST_COMPLETED):
                for worker in workers:
                    results.append(worker.result())

        for result in results:
            with self.subTest():
                self.assertEqual(result['outgoing_message'], result['returning_message'])
