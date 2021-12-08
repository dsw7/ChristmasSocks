import curses
from core.panel_base import ControlPanelBase
from core.consts import PANEL_MARGIN

HEADER = ' {:<20} {:<20} {:<20}'.format('HOST', 'STATUS', 'UPTIME (HH:MM:SS)')
OFFSET = 21


class PanelSysInfo(ControlPanelBase):

    def render_subwin_header(self) -> None:

        self.body.addstr(1, PANEL_MARGIN - 1, ' Panel type:')
        self.body.addstr(1, PANEL_MARGIN + 20, 'SYSINFO', curses.A_UNDERLINE)
        self.body.addstr(4, PANEL_MARGIN - 1, HEADER + ' ' * (self.body.getmaxyx()[1] - len(HEADER) - 4), curses.A_REVERSE)

    def run_sysinfo_on_servers(self) -> None:

        for server, handle in self.cli_params['clients'].items():
            status = {}

            if not handle.connect():
                status['status'] = 'DEAD'
                status['results'] = '-'
                self.results[server] = status
                continue

            status['status'] = 'ALIVE'
            status['results'] = handle.send('sysinfo')

            handle.disconnect()
            self.results[server] = status

    def render_body(self) -> None:
        for index, (server, status) in enumerate(self.results.items(), 5):  # Offset to account for header position
            self.body.addstr(index, PANEL_MARGIN + 0 * OFFSET, server)

            # Clears from cursor to EOL - so covers both the following addstr
            self.body.clrtoeol()
            self.body.addstr(index, PANEL_MARGIN + 1 * OFFSET, status['status'])
            self.body.addstr(index, PANEL_MARGIN + 2 * OFFSET, status['uptime'])

    def wait_for_user_input(self) -> None:
        while self.run_program:
            if self.stdscr.getch() == ord('q'):
                self.run_program = False

    def core(self) -> None:
        self.render_subwin_header()
        self.run_sysinfo_on_servers()
        self.render_body()
        self.body.refresh()
        self.stdscr.refresh()

        self.wait_for_user_input()


# See https://docs.python.org/3/howto/curses.html#starting-and-ending-a-curses-application
def panel_sysinfo(stdscr: curses.window, cli_params: dict) -> None:
    PanelSysInfo(stdscr, cli_params).main()
