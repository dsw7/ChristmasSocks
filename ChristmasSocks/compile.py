#!/usr/bin/env python3

import sys
from os import get_terminal_size, path
from subprocess import call

EXIT_SUCCESS = 0
EXIT_FAILURE = 1


class CompileSocks:
    def __init__(self) -> None:
        self.hbar = '=' * get_terminal_size().columns
        self.path_this = path.dirname(__file__)

    def generate_makefiles(self) -> int:
        command = 'cmake -S {} -B {}/bin'.format(self.path_this, self.path_this)
        return call(command.split())

    def compile_binary_from_makefiles(self) -> int:
        command = 'make --jobs=12 -C {}/bin'.format(self.path_this)
        return call(command.split())

    def execute_main(self) -> None:
        print(self.hbar)
        print('-- Compiling project: ChristmasSocks')
        print('-- Project maintainer: David Weber')

        print(self.hbar)
        if self.generate_makefiles() != EXIT_SUCCESS:
            return EXIT_FAILURE

        print(self.hbar)
        if self.compile_binary_from_makefiles() != EXIT_SUCCESS:
            return EXIT_FAILURE

        print(self.hbar)
        return EXIT_SUCCESS


def main():
    sys.exit(CompileSocks().execute_main())

if __name__ == '__main__':
    main()
