# pylint: disable=W0201  # Disable defined outside __init__

from process import Popen
from pytest import mark
from utils import Server, EXIT_FAILURE

@mark.release_test
class TestLineBreakToggler:

    def setup_class(self) -> None:
        self.server = Server()

    def teardown_method(self) -> None:
        self.server.stop_server()

    def test_strip_line_breaks(self) -> None:
        self.server.start_server_in_background('--strip-line-breaks')
        returncode = Popen('echo "sleep" | nc localhost 8080', shell=True).wait()

    def test_no_strip_line_breaks(self) -> None:
        self.server.start_server_in_background()
        returncode = Popen('echo -n "sleep" | nc localhost 8080', shell=True).wait()
