#!/usr/bin/env python3
import sys
from os import (
    get_terminal_size,
    path,
    devnull
)
from configparser import ConfigParser
from typing import Tuple
from subprocess import Popen, PIPE
from unittest import (
    TestLoader,
    TextTestRunner
)
from click import (
    secho,
    group,
    option
)

EXIT_SUCCESS = 0
EXIT_FAILURE = 1
SEPARATOR = '-' * get_terminal_size().columns
DEVNULL = open(devnull, 'wb')
PATH_THIS = path.dirname(__file__)

def echo_separator() -> None:
    secho(SEPARATOR, fg='yellow')

def echo_message(msg: str) -> None:
    secho('{}'.format(msg), fg='yellow')

def echo_error(msg: str) -> None:
    secho('{}'.format(msg), fg='red')

def run_shell_command(command: str, capture_output=False) -> Tuple[int, str, str]:
    echo_message('Running command: {}'.format(command))
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

def get_configs() -> dict:
    path_ini_file = path.join(PATH_THIS, 'configure.ini')

    if not path.exists(path_ini_file):
        echo_error('Could not access {}'.format(path_ini_file))
        sys.exit(EXIT_FAILURE)

    configs = ConfigParser()
    configs.read(path_ini_file)
    return configs


class Compile:

    def __init__(self) -> None:
        self.configs = get_configs()

    def generate_makefiles(self) -> int:
        echo_separator()
        echo_message('Generating Makefiles for project')

        command = 'cmake -S {root} -B {root}/{output_dir}'.format(
            root=PATH_THIS, output_dir=self.configs['compile']['output-dir']
        )
        return run_shell_command(command)[0]

    def generate_makefiles_release_with_debug_info(self) -> int:
        echo_separator()
        echo_message('Generating Makefiles for project')

        command = 'cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S {root} -B {root}/{output_dir}'.format(
            root=PATH_THIS, output_dir=self.configs['compile']['output-dir']
        )
        return run_shell_command(command)[0]

    def run_make(self) -> int:
        echo_separator()
        echo_message('Compiling project using Makefiles')

        command = 'make --jobs={} -C {}/{}'.format(
            self.configs['compile']['num-make-jobs'], PATH_THIS, self.configs['compile']['output-dir']
        )
        return run_shell_command(command)[0]

    def compile_release_binary(self) -> int:
        if self.generate_makefiles() != EXIT_SUCCESS:
            return EXIT_FAILURE

        if self.run_make() != EXIT_SUCCESS:
            return EXIT_FAILURE

        return EXIT_SUCCESS

    def compile_debug_binary(self) -> int:
        if self.generate_makefiles_release_with_debug_info() != EXIT_SUCCESS:
            return EXIT_FAILURE

        if self.run_make() != EXIT_SUCCESS:
            return EXIT_FAILURE

        return EXIT_SUCCESS


class StaticAnalysis:

    def __init__(self) -> None:
        self.configs = get_configs()

    def run_cppcheck(self) -> int:
        echo_separator()
        echo_message('Linting project using cppcheck static analysis tool')

        command = 'cppcheck {root}/src/ -I {root}/include/ --template={template} --enable=all'.format(
            root=PATH_THIS, template=self.configs['static-analysis']['cppcheck-template']
        )
        exit_code, _, _ = run_shell_command(command)
        return exit_code

    def main(self) -> int:
        if self.run_cppcheck() != EXIT_SUCCESS:
            return EXIT_FAILURE

        return EXIT_SUCCESS


class RunTests:

    def __init__(self) -> None:
        self.configs = get_configs()
        self.test_directory = path.join(PATH_THIS, 'tests')

        echo_separator()
        echo_message('Running tests in directory: {}'.format(path.realpath(self.test_directory)))

    def run_unittest_release(self) -> bool:
        echo_message('Running tests in files matching pattern: {}'.format(self.configs['run-tests']['test-filename-release']))

        suite = TestLoader().discover(
            self.test_directory, pattern=self.configs['run-tests']['test-filename-release']
        )

        runner = TextTestRunner(
            verbosity=self.configs['run-tests'].getint('test-verbosity'),
            failfast=self.configs['run-tests'].getboolean('blind-run')
        )

        test_run = runner.run(suite)
        return test_run.wasSuccessful()

    def run_unittest_memory(self) -> bool:
        echo_message('Running tests in files matching pattern: {}'.format(self.configs['run-tests']['test-filename-memory']))

        suite = TestLoader().discover(
            self.test_directory, pattern=self.configs['run-tests']['test-filename-memory']
        )

        runner = TextTestRunner(
            verbosity=self.configs['run-tests'].getint('test-verbosity'),
            failfast=self.configs['run-tests'].getboolean('blind-run')
        )

        test_run = runner.run(suite)
        return test_run.wasSuccessful()


@group()
def main():
    pass

@main.command(help='Compile binary')
@option('--debug', 'build_type', flag_value='debug', help='Compile with -DCMAKE_BUILD_TYPE=RelWithDebInfo')
@option('--release', 'build_type', flag_value='release', default=True, help='Compile with -DCMAKE_BUILD_TYPE=Release')
def compile(build_type: str):
    compiler = Compile()
    if build_type == 'debug':
        rv = compiler.compile_debug_binary()
    else:
        rv = compiler.compile_release_binary()
    sys.exit(rv)

@main.command(help='Run static analysis on project')
def lint():
    sys.exit(StaticAnalysis().main())

@main.command(help='Run unit tests on project')
@option('-v', '--valgrind/--no-valgrind', default=False, help='Run unit tests with Valgrind debugging tool')
def test(valgrind):
    test_runner = RunTests()

    if valgrind:
        rv = test_runner.run_unittest_memory()
    else:
        rv = test_runner.run_unittest_release()

    if rv:
        sys.exit(EXIT_SUCCESS)
    sys.exit(EXIT_FAILURE)

if __name__ == '__main__':
    main()
