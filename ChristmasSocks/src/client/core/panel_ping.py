import curses
from core.panel_base import ControlPanelBase

HEADER = ' {:<20} {:<20} {:<20}'.format('HOST', 'STATUS', 'UPTIME (HH:MM:SS)')


class PanelPing(ControlPanelBase):

    def render_subwin_header(self) -> None:
        self.body.addstr(1, 2, HEADER + ' ' * (self.body.getmaxyx()[1] - len(HEADER) - 4), curses.A_REVERSE)

    def core(self) -> None:
        self.render_subwin_header()
        self.stdscr.getch()


# See https://docs.python.org/3/howto/curses.html#starting-and-ending-a-curses-application
def panel_ping(stdscr: curses.window, clients: dict) -> None:
    PanelPing(stdscr, clients).main()
