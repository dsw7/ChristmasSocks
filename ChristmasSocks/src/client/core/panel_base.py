import curses
from abc import ABC, abstractmethod
from core.consts import PROJECT_TITLE


class ControlPanelBase(ABC):

    def __init__(self, stdscr: curses.window, cli_params: dict) -> None:
        self.stdscr = stdscr
        self.rows, self.columns = self.stdscr.getmaxyx()

        self.cli_params = cli_params
        self.results = {}

        # Disable annoying blinking cursor
        curses.curs_set(0)

        # The args follow: lines, columns, y, x
        self.header = self.stdscr.subwin(3, self.columns, 0, 0)
        self.footer = self.stdscr.subwin(1, self.columns, self.rows - 1, 0)

        header_max_y, _ = self.header.getmaxyx()
        self.body = self.stdscr.subwin(self.footer.getparyx()[0] - header_max_y, self.columns, header_max_y, 0)

        self.run_program = True

    def wait_for_user_input(self) -> None:
        while self.run_program:
            if self.stdscr.getch() == ord('q'):
                self.run_program = False

    def command_does_not_exist(self, command: str) -> bool:
        status = []

        for server, handle in self.cli_params['clients'].items():
            if not handle.connect():
                continue

            status.append(command == handle.send(command))
            handle.disconnect()

        return any(status)

    def display_header(self) -> None:
        self.header.addstr(1, (self.columns // 2) - (len(PROJECT_TITLE) // 2), PROJECT_TITLE, curses.A_BOLD)
        self.header.box()

    def display_footer(self) -> None:
        self.footer.addstr(0, 1, 'Press "q" to quit', curses.A_REVERSE)
        self.footer.bkgd(curses.A_REVERSE)

    @abstractmethod
    def run_server_command(self) -> None:
        pass

    @abstractmethod
    def render_subwin_header(self) -> None:
        pass

    @abstractmethod
    def render_body(self) -> None:
        pass

    @abstractmethod
    def main(self) -> None:
        pass
