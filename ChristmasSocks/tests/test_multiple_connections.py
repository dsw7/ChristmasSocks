from unittest import TestCase
from concurrent import futures
from utils import (
    Connection,
    generate_random_string
)


class TestMultipleConnections(TestCase):
    def setUp(self) -> None:
        self.socket_a = Connection()
        self.socket_b = Connection()
        self.socket_c = Connection()
        self.socket_a.connect()
        self.socket_b.connect()
        self.socket_c.connect()

    def tearDown(self) -> None:
        self.socket_a.disconnect()
        self.socket_b.disconnect()
        self.socket_c.disconnect()

    @staticmethod
    def wrap_send(test_string: str, socket: Connection) -> dict:
        return {
            'outgoing_message': test_string,
            'returning_message': socket.send(test_string)
        }

    def test_echo_3_connections(self) -> None:
        strings = generate_random_string(num_strings=3, len_strings=5)

        workers, results = [], []
        with futures.ThreadPoolExecutor() as executor:
            workers.append(executor.submit(self.wrap_send, strings[0], self.socket_a))
            workers.append(executor.submit(self.wrap_send, strings[1], self.socket_b))
            workers.append(executor.submit(self.wrap_send, strings[2], self.socket_c))

            if futures.wait(workers, return_when=futures.FIRST_COMPLETED):
                for worker in workers:
                    results.append(worker.result())

        for result in results:
            with self.subTest():
                self.assertEqual(result['outgoing_message'], result['returning_message'])
