#!/usr/bin/env python3
import sys
from os import (
    get_terminal_size,
    path,
    devnull
)
from signal import SIGINT
from configparser import ConfigParser
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
DEVNULL = open(devnull, 'wb')


class ConfigBase(ABC):
    def __init__(self) -> None:
        self.path_this = path.dirname(__file__)
        self.start_time = time()
        path_ini_file = path.join(self.path_this, 'configure.ini')

        if not path.exists(path_ini_file):
            self.echo_error('Could not access {}'.format(path_ini_file))
            sys.exit(EXIT_FAILURE)

        self.configs = ConfigParser()
        self.configs.read(path_ini_file)

    def __del__(self) -> None:
        self.echo_separator()
        self.echo_message('Total processing time: {} s'.format(round(time() - self.start_time, 3)))

    @staticmethod
    def echo_separator() -> None:
        secho(SEPARATOR, fg='yellow')

    @staticmethod
    def echo_message(msg: str) -> None:
        secho('{}'.format(msg), fg='yellow')

    @staticmethod
    def echo_error(msg: str) -> None:
        secho('{}'.format(msg), fg='red')

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

        command = 'cmake -S {root} -B {root}/bin'.format(root=self.path_this)
        return self.run_shell_command(command)[0]

    def compile_binary_from_makefiles(self) -> int:
        self.echo_separator()
        self.echo_message('Compiling project using Makefiles')

        command = 'make --jobs={} -C {}/bin'.format(self.configs['compile']['num-make-jobs'], self.path_this)
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

        command = 'cppcheck {root}/src/ -I {root}/include/ --template={template} --enable=all'.format(
            root=self.path_this, template=self.configs['static-analysis']['cppcheck-template']
        )
        exit_code, _, _ = self.run_shell_command(command)
        return exit_code

    def main(self) -> int:
        if self.run_cppcheck() != EXIT_SUCCESS:
            return EXIT_FAILURE

        return EXIT_SUCCESS


class RunTests(ConfigBase):

    def start_server(self) -> Popen:
        # pass command line arguments to binary here
        command = '{}/bin/{}'.format(self.path_this, self.configs['run-tests']['output-name'])
        self.echo_message('Starting up server on localhost with command: {}'.format(command))
        return Popen(command, stdout=DEVNULL)

    def stop_server(self, process: Popen) -> None:
        self.echo_message('Stopping server on localhost')
        process.send_signal(SIGINT)

    def run_unittest(self) -> bool:
        self.echo_separator()

        process = self.start_server()
        sleep(self.configs['run-tests'].getfloat('startup-delay'))

        test_directory = path.join(self.path_this, 'tests')
        realpath = path.realpath(test_directory)
        self.echo_message('Running tests in directory: {}'.format(realpath))

        suite = TestLoader().discover(
            test_directory, pattern=TEST_FILENAMES_PATTERN
        )

        runner = TextTestRunner(
            verbosity=self.configs['run-tests'].getint('test-verbosity'),
            failfast=self.configs['run-tests'].getboolean('blind-run')
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
