from abc import ABC, abstractmethod
import curses


class ControlPanelBase(ABC):

    def __init__(self) -> None:
        self.screen = curses.initscr()
        self.rows, self.columns = self.screen.getmaxyx()

    @abstractmethod
    def execute_main(self) -> None:
        pass


class BasicPanel(ControlPanelBase):

    def execute_main(self) -> None:
        self.screen.refresh()

        curses.napms(2000)
        curses.endwin()
