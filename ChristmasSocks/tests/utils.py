from tempfile import gettempdir
from os import path, environ
from string import ascii_letters, digits, punctuation
from random import choice
import pytest

ALPHANUMERIC = ascii_letters + digits

def get_current_test_method() -> str:

    try:
        test_case = environ['PYTEST_CURRENT_TEST'].split('::')[-1].split()[0]
    except KeyError:
        pytest.exit('Could not find PYTEST_CURRENT_TEST environment variable. Are you using pytest?')

    return test_case

def get_current_test_module() -> str:

    try:
        current_test = environ['PYTEST_CURRENT_TEST']
    except KeyError:
        pytest.exit('Could not find PYTEST_CURRENT_TEST environment variable. Are you using pytest?')

    current_test = current_test.split('::')[0]
    current_test = path.splitext(current_test)[0]

    return current_test

def get_log_file_path(test_case: str) -> str:

    return path.join(gettempdir(), f'{test_case}.log')

def generate_random_string(num_strings: int, len_strings: int) -> list:
    result = []

    for _ in range(num_strings):
        result.append(''.join(choice(ALPHANUMERIC) for _ in range(len_strings)))

    return result

def generate_random_punctuation(num_strings: int, len_strings: int) -> list:
    result = []

    for _ in range(num_strings):
        result.append(''.join(choice(punctuation) for _ in range(len_strings)))

    return result
