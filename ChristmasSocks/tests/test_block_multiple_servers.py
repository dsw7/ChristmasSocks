# pylint: disable=W0201  # Disable defined outside __init__

from pytest import mark
from utils import ServerBackground, ServerForeground, EXIT_FAILURE

@mark.release_test
class TestBlockSecondServer:

    def setup_class(self) -> None:
        self.port = 8000
        self.primary_server = ServerBackground(logfile='test_block_second_server_primary.log')
        self.primary_server.start_server('--port={}'.format(self.port))

    def teardown_class(self) -> None:
        self.primary_server.stop_server()

    def test_start_second_server(self) -> None:
        secondary_server = ServerForeground(logfile='test_block_second_server_secondary.log')
        assert secondary_server.start_server('--port={}'.format(self.port)) == EXIT_FAILURE
