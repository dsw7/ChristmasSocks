#!/usr/bin/env python3
import sys
from os import get_terminal_size, path
from abc import ABC, abstractmethod
from subprocess import call
from time import time
from unittest import (
    TestLoader,
    TextTestRunner
)
from click import (
    secho,
    group
)

EXIT_SUCCESS = 0
EXIT_FAILURE = 1
SEPARATOR = '-' * get_terminal_size().columns
TEST_FILENAMES_PATTERN = 'test_*'


class ConfigBase(ABC):
    def __init__(self) -> None:
        self.path_this = path.dirname(__file__)
        self.start_time = time()

    def __del__(self) -> None:
        self.echo_separator()
        self.echo_message('Total processing time: {} s'.format(round(time() - self.start_time, 3)))

    @staticmethod
    def echo_separator() -> None:
        secho(SEPARATOR, fg='yellow')

    @staticmethod
    def echo_message(msg: str) -> None:
        secho('{}'.format(msg), fg='yellow')

    def run_shell_command(self, command: str) -> int:
        self.echo_message('Running command: {}'.format(command))
        return call(command.split())

    @abstractmethod
    def execute_main(self) -> int:
        pass


class Compile(ConfigBase):

    def generate_makefiles(self) -> int:
        self.echo_separator()
        self.echo_message('Generating Makefiles for project')

        command = 'cmake -S {p} -B {p}/bin'.format(p=self.path_this)
        return self.run_shell_command(command)

    def compile_binary_from_makefiles(self) -> int:
        self.echo_separator()
        self.echo_message('Compiling project using Makefiles')

        command = 'make --jobs=12 -C {}/bin'.format(self.path_this)
        return self.run_shell_command(command)

    def execute_main(self) -> int:
        if self.generate_makefiles() != EXIT_SUCCESS:
            return EXIT_FAILURE

        if self.compile_binary_from_makefiles() != EXIT_SUCCESS:
            return EXIT_FAILURE

        return EXIT_SUCCESS


class StaticAnalysis(ConfigBase):

    def run_cppcheck(self) -> int:
        self.echo_separator()
        self.echo_message('Linting project using cppcheck static analysis tool')

        command = 'cppcheck {p}/src/ -I {p}/include/ --template=gcc --enable=all'.format(p=self.path_this)
        return self.run_shell_command(command)

    def execute_main(self) -> int:
        if self.run_cppcheck() != EXIT_SUCCESS:
            return EXIT_FAILURE

        return EXIT_SUCCESS


class RunTests(ConfigBase):

    def run_unittest(self) -> bool:
        self.echo_separator()

        test_directory = path.join(self.path_this, 'tests')
        realpath = path.realpath(test_directory)

        self.echo_message('Running tests in directory: {}'.format(realpath))

        suite = TestLoader().discover(
            test_directory, pattern=TEST_FILENAMES_PATTERN
        )
        runner = TextTestRunner(verbosity=2)
        test_run = runner.run(suite)

        return test_run.wasSuccessful()

    def execute_main(self) -> int:
        if self.run_unittest():
            return EXIT_SUCCESS

        return EXIT_FAILURE


@group()
def main():
    pass

@main.command(help='Compile binary')
def compile():
    sys.exit(Compile().execute_main())

@main.command(help='Run static analysis on project')
def lint():
    sys.exit(StaticAnalysis().execute_main())

@main.command(help='Run unit tests on project')
def test():
    sys.exit(RunTests().execute_main())

if __name__ == '__main__':
    main()
