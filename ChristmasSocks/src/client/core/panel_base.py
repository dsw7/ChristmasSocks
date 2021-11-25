import curses
from abc import ABC, abstractmethod
from core.consts import PROJECT_TITLE


class ControlPanelBase(ABC):

    def __init__(self, stdscr: curses.window, clients: dict) -> None:
        self.stdscr = stdscr
        self.rows, self.columns = self.stdscr.getmaxyx()

        # Disable annoying blinking cursor
        curses.curs_set(0)

        # The args follow: lines, columns, y, x)
        self.header = self.stdscr.subwin(3, self.columns, 0, 0)
        self.footer = self.stdscr.subwin(3, self.columns, self.rows - 3, 0)

    def display_header(self) -> None:
        self.header.addstr(1, (self.columns // 2) - (len(PROJECT_TITLE) // 2), PROJECT_TITLE, curses.A_BOLD)
        self.header.box()

    def display_footer(self) -> None:
        self.footer.addstr(1, 2, ' Press any button to exit ', curses.A_REVERSE)
        self.footer.box()

    @abstractmethod
    def core(self) -> None:
        pass

    def main(self) -> None:
        self.display_header()
        self.display_footer()
        self.core()
