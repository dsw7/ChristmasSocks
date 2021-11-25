import curses
from core.panel_base import ControlPanelBase

HEADER = ' {:<20} {:<20} {:<20}'.format('HOST', 'STATUS', 'UPTIME (HH:MM:SS)')
OFFSET = 21


class PanelPing(ControlPanelBase):

    def render_subwin_header(self) -> None:
        self.body.addstr(1, 2, HEADER + ' ' * (self.body.getmaxyx()[1] - len(HEADER) - 4), curses.A_REVERSE)

    def ping_servers(self) -> None:

        for server, handle in self.clients.items():
            status = {}

            if not handle.connect():
                status['status'] = 'DEAD'
                status['uptime'] = '-'
                self.results[server] = status
                continue

            status['status'] = 'ALIVE'
            status['uptime'] = handle.send('uptime')

            handle.disconnect()
            self.results[server] = status

    def render_body(self) -> None:
        for index, (server, status) in enumerate(self.results.items(), 2):  # Offset to account for header position
            self.body.addstr(index, 3 + 0 * OFFSET, server)
            self.body.addstr(index, 3 + 1 * OFFSET, status['status'])
            self.body.addstr(index, 3 + 2 * OFFSET, status['uptime'])

    def core(self) -> None:
        self.render_subwin_header()
        self.ping_servers()
        self.render_body()
        self.stdscr.getch()


# See https://docs.python.org/3/howto/curses.html#starting-and-ending-a-curses-application
def panel_ping(stdscr: curses.window, clients: dict) -> None:
    PanelPing(stdscr, clients).main()
