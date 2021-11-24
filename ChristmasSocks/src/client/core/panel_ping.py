from core.panel_base import ControlPanelBase

class PanelPing(ControlPanelBase):

    def core(self) -> None:
        self.stdscr.getch()

# See https://docs.python.org/3/howto/curses.html#starting-and-ending-a-curses-application
def panel_ping(stdscr) -> None:
    PanelPing(stdscr).main()
