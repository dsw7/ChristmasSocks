#!/usr/bin/env python3

import sys
from os import path
from configparser import ConfigParser
from click import (
    group,
    pass_context,
    pass_obj,
    secho
)
from client import Client

PATH_THIS = path.dirname(__file__)

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
    secho('Pinging all workers', fg='green')

    for server, client_handle in obj.items():
        if not client_handle.connect():
            secho('Host {} is dead'.format(server), fg='red')
            continue

        if client_handle.send('echo') == 'echo':
            secho('Host {} is alive'.format(server), fg='green')
        else:
            secho('Host {} returned unknown reply'.format(server), fg='yellow')

        client_handle.disconnect()

if __name__ == '__main__':
    main()
