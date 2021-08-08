# pylint: disable=W0201  # Disable defined outside __init__

from concurrent import futures
from pytest import mark
from utils import (
    Server,
    Client,
    EXIT_SUCCESS
)

@mark.release_test
class TestClientHangup:

    def setup_class(self) -> None:
        self.server = Server()
        self.client = Client()

    def start_server(self) -> int:
        return self.server.start_server_in_foreground()

    def hangup_client(self) -> int:
        self.client.connect()
        self.client.disconnect()
        return EXIT_SUCCESS

    def test_echo_3_connections(self) -> None:
        workers, results = [], []

        with futures.ThreadPoolExecutor() as executor:
            workers.append(executor.submit(self.start_server))
            import sleep; sleep(2);
            workers.append(executor.submit(self.hangup_client))

            if futures.wait(workers, return_when=futures.ALL_COMPLETED):
                for worker in workers:
                    results.append(worker.result())

        for result in results:
            assert result == EXIT_SUCCESS
