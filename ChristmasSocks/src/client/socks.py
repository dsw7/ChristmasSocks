#!/usr/bin/env python3

import sys
from os import path, get_terminal_size
from uuid import uuid4
from functools import lru_cache
from configparser import ConfigParser
from click import (
    group,
    pass_context,
    pass_obj,
    secho
)
from client import Client

TERMINAL_SIZE = get_terminal_size().columns
PATH_THIS = path.dirname(__file__)

@lru_cache
def get_separator(size: int) -> str:
    return u'\u2500' * size

def read_socks_config_file() -> ConfigParser:
    ini_file = path.join(PATH_THIS, 'configs', 'socks.ini')
    if not path.exists(ini_file):
        sys.exit('Could not open {}'.format(ini_file))

    parser = ConfigParser()
    parser.read(ini_file)
    return parser

@group()
@pass_context
def main(context) -> None:
    context.ensure_object(dict)
    configs = read_socks_config_file()

    for server in configs['servers'].values():
        context.obj[server] = Client(
            client_configs=configs['client-configs'], host=server
        )

@main.command(help='Ping all machines')
@pass_obj
def ping(obj) -> None:
    statuses = {}

    for server, client_handle in obj.items():
        status = {}

        if not client_handle.connect():
            status['status'] = 'DEAD'
            status['send'] = 'X'
            status['recv'] = 'X'
            statuses[server] = status
            continue

        msg = uuid4().__str__()[0:13]
        status['status'] = 'ALIVE'
        status['send'] = msg
        status['recv'] = client_handle.send(msg)

        client_handle.disconnect()
        statuses[server] = status

    secho(get_separator(TERMINAL_SIZE))
    secho(' {:<20} {:<20} {:<20} {:<20}'.format('HOST', 'STATUS', 'SEND', 'RECV'))
    secho(get_separator(TERMINAL_SIZE))

    for server, status in statuses.items():
        secho(' {:<20} {:<20} {:<20} {:<20}'.format(server, status['status'], status['send'], status['recv']))

    secho(get_separator(TERMINAL_SIZE))

if __name__ == '__main__':
    main()
