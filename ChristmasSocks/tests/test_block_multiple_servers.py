# pylint: disable=W0201  # Disable defined outside __init__

from pytest import mark
from utils import Server, EXIT_FAILURE

@mark.release_test
class TestBlockSecondServer:

    def setup_class(self) -> None:
        self.port = 8000
        self.server = Server()
        self.server.start_server_in_background('--port={}'.format(self.port))

    def teardown_class(self) -> None:
        self.server.stop_server()

    def test_start_second_server(self) -> None:
        assert self.server.start_server_in_foreground('--port={}'.format(self.port)) == EXIT_FAILURE
