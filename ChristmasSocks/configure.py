#!/usr/bin/env python3
import sys
from os import (
    get_terminal_size,
    path,
    devnull
)
from signal import SIGINT
from abc import ABC, abstractmethod
from typing import Tuple
from subprocess import Popen, PIPE
from time import time, sleep
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
TEMPLATE_CPPCHECK = '{severity}-{id}-{file}-{line}-{message}'
LIST_TEMPLATE_CPPCHECK = ['Severity', 'Id', 'File', 'Line', 'Message']
IS_BLIND_TEST = True
DEVNULL = open(devnull, 'wb')


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

    def run_shell_command(self, command: str, capture_output=False) -> Tuple[int, str, str]:
        self.echo_message('Running command: {}'.format(command))
        if capture_output:
            process = Popen(command.split(), stdout=PIPE, stderr=PIPE)
        else:
            process = Popen(command.split())
        stdout, stderr = process.communicate()

        if stdout:
            stdout = stdout.decode()
        else:
            stdout = ''

        if stderr:
            stderr = stderr.decode()
        else:
            stderr = ''

        return process.returncode, stdout, stderr

    @abstractmethod
    def main(self) -> int:
        pass


class Compile(ConfigBase):

    def generate_makefiles(self) -> int:
        self.echo_separator()
        self.echo_message('Generating Makefiles for project')

        command = 'cmake -S {p} -B {p}/bin'.format(p=self.path_this)
        return self.run_shell_command(command)[0]

    def compile_binary_from_makefiles(self) -> int:
        self.echo_separator()
        self.echo_message('Compiling project using Makefiles')

        command = 'make --jobs=12 -C {}/bin'.format(self.path_this)
        return self.run_shell_command(command)[0]

    def main(self) -> int:
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
        exit_code, _, _ = self.run_shell_command(command)
        return exit_code

    def main(self) -> int:
        if self.run_cppcheck() != EXIT_SUCCESS:
            return EXIT_FAILURE

        return EXIT_SUCCESS


class RunTests(ConfigBase):

    def start_server(self) -> Popen:
        # pass command line arguments to binary here
        command = '{}/bin/test'.format(self.path_this)
        self.echo_message('Starting up server on localhost with command: {}'.format(command))
        return Popen(command, stdout=DEVNULL)

    def stop_server(self, process: Popen) -> None:
        self.echo_message('Stopping server on localhost')
        process.send_signal(SIGINT)

    def run_unittest(self) -> bool:
        self.echo_separator()

        process = self.start_server()
        sleep(0.1)

        test_directory = path.join(self.path_this, 'tests')
        realpath = path.realpath(test_directory)
        self.echo_message('Running tests in directory: {}'.format(realpath))

        suite = TestLoader().discover(
            test_directory, pattern=TEST_FILENAMES_PATTERN
        )

        runner = TextTestRunner(
            verbosity=2, failfast=IS_BLIND_TEST
        )

        test_run = runner.run(suite)
        self.stop_server(process)

        return test_run.wasSuccessful()

    def main(self) -> int:
        if self.run_unittest():
            return EXIT_SUCCESS

        return EXIT_FAILURE


@group()
def main():
    pass

@main.command(help='Compile binary')
def compile():
    sys.exit(Compile().main())

@main.command(help='Run static analysis on project')
def lint():
    sys.exit(StaticAnalysis().main())

@main.command(help='Run unit tests on project')
def test():
    sys.exit(RunTests().main())

if __name__ == '__main__':
    main()
