#!/usr/bin/env python3
from os import path
from unittest import (
    TestLoader,
    TextTestRunner
)

EXIT_SUCCESS = 0
EXIT_FAILURE = 1
TEST_FILENAMES_PATTERN = 'test_*'

def main():
    test_directory = path.dirname(__file__)
    realpath = path.realpath(test_directory)

    print(f'Running tests in directory: {realpath}')

    suite = TestLoader().discover(
        test_directory, pattern=TEST_FILENAMES_PATTERN
    )
    runner = TextTestRunner(verbosity=2)
    runner.run(suite)

if __name__ == "__main__":
    main()
