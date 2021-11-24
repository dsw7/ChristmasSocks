import curses


class ControlPanel:

    def __init__(self) -> None:
        self.screen = None

    def execute_main(self) -> None:
        self.screen = curses.initscr()
        self.screen.refresh()

        curses.napms(2000)
        curses.endwin()
