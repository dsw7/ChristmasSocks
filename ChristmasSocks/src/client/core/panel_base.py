import curses
from abc import ABC, abstractmethod
from core.consts import PROJECT_TITLE


class ControlPanelBase(ABC):

    def __init__(self, stdscr: curses.window, clients: dict) -> None:
        self.stdscr = stdscr
        self.rows, self.columns = self.stdscr.getmaxyx()

    def header(self) -> None:
        self.stdscr.addstr(0, 0, u'\u2500' * self.columns)
        self.stdscr.addstr(1, (self.columns // 2) - (len(PROJECT_TITLE) // 2), PROJECT_TITLE, curses.A_BOLD)
        self.stdscr.addstr(2, 0, u'\u2500' * self.columns)

    def footer(self) -> None:
        self.stdscr.addstr(self.rows - 1, 1, ' Press any button to exit ', curses.A_REVERSE)

    @abstractmethod
    def core(self) -> None:
        pass

    def main(self) -> None:
        self.header()
        self.footer()
        self.stdscr.refresh()
        self.core()
