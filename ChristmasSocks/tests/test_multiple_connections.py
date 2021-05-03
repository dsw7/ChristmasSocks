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

    def test_echo_3_connections(self) -> None:
        strings = generate_random_string(num_strings=3, len_strings=5)

        workers, results = [], []
        with futures.ThreadPoolExecutor() as executor:
            workers.append(executor.submit(self.socket_a.send, strings[0]))
            workers.append(executor.submit(self.socket_b.send, strings[1]))
            workers.append(executor.submit(self.socket_c.send, strings[2]))

            if futures.wait(workers, return_when=futures.FIRST_COMPLETED):
                for worker in workers:
                    results.append(worker.result())

        self.assertEqual(strings[0], results[0])
        self.assertEqual(strings[1], results[1])
        self.assertEqual(strings[2], results[2])
