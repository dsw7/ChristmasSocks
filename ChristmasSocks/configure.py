#!/usr/bin/env python3

import sys
from os import get_terminal_size, path
from subprocess import call
from time import time
from click import secho

EXIT_SUCCESS = 0
EXIT_FAILURE = 1
TERMINAL_WIDTH = get_terminal_size().columns

def render_separator() -> None:
    secho('=' * TERMINAL_WIDTH, fg='cyan')


class CompileSocks:
    def __init__(self) -> None:
        self.path_this = path.dirname(__file__)
        self.start_time = time()

    def generate_makefiles(self) -> int:
        render_separator()
        command = 'cmake -S {} -B {}/bin'.format(self.path_this, self.path_this)
        return call(command.split())

    def compile_binary_from_makefiles(self) -> int:
        render_separator()
        command = 'make --jobs=12 -C {}/bin'.format(self.path_this)
        return call(command.split())

    def execute_main(self) -> None:
        render_separator()
        secho('-- Compiling project: ChristmasSocks')
        secho('-- Project maintainer: David Weber')

        if self.generate_makefiles() != EXIT_SUCCESS:
            return EXIT_FAILURE

        if self.compile_binary_from_makefiles() != EXIT_SUCCESS:
            return EXIT_FAILURE

        render_separator()
        secho('-- Success!')
        secho('-- Total compile time: {} s'.format(round(time() - self.start_time, 3)))
        return EXIT_SUCCESS


def main():
    sys.exit(CompileSocks().execute_main())

if __name__ == '__main__':
    main()
