#!/usr/bin/env python3
import sys
from os import get_terminal_size, path
from abc import ABC, abstractmethod
from subprocess import call
from time import time
from click import (
    secho,
    group
)

EXIT_SUCCESS = 0
EXIT_FAILURE = 1


class ConfigBase(ABC):
    def __init__(self) -> None:
        self.path_this = path.dirname(__file__)
        self.start_time = time()
        self.separator = '=' * get_terminal_size().columns

    def render_separator(self) -> None:
        secho(self.separator, fg='cyan')

    @staticmethod
    def render_message(msg: str) -> None:
        secho('> {}'.format(msg), fg='cyan')

    @abstractmethod
    def execute_main(self) -> int:
        pass


class Compile(ConfigBase):
    def generate_makefiles(self) -> int:
        self.render_separator()
        command = 'cmake -S {p} -B {p}/bin'.format(p=self.path_this)
        self.render_message('Running command: "{}"'.format(command))
        return call(command.split())

    def compile_binary_from_makefiles(self) -> int:
        self.render_separator()
        command = 'make --jobs=12 -C {}/bin'.format(self.path_this)
        self.render_message('Running command: {}'.format(command))
        return call(command.split())

    def execute_main(self) -> None:
        self.render_separator()
        self.render_message('Compiling project...')

        if self.generate_makefiles() != EXIT_SUCCESS:
            return EXIT_FAILURE

        if self.compile_binary_from_makefiles() != EXIT_SUCCESS:
            return EXIT_FAILURE

        self.render_separator()
        self.render_message('Success!')
        self.render_message('Total compile time: {} s'.format(round(time() - self.start_time, 3)))
        return EXIT_SUCCESS


class StaticAnalysis(ConfigBase):
    def run_cppcheck(self) -> int:
        self.render_separator()
        command = 'cppcheck {p}/src/ -I {p}/include/ --template=gcc --enable=all'.format(p=self.path_this)
        self.render_message('Running command: "{}"'.format(command))
        return call(command.split())

    def execute_main(self) -> None:
        self.render_separator()
        self.render_message('Running static analysis tool cppcheck on project...')

        if self.run_cppcheck() != EXIT_SUCCESS:
            return EXIT_FAILURE

        self.render_separator()
        self.render_message('Success!')
        self.render_message('Total processing time: {} s'.format(round(time() - self.start_time, 3)))
        return EXIT_SUCCESS


@group()
def main():
    pass

@main.command(help='Compile binary')
def compile():
    sys.exit(Compile().execute_main())

@main.command(help='Run static analysis on project')
def static_analysis():
    sys.exit(StaticAnalysis().execute_main())

if __name__ == '__main__':
    main()
