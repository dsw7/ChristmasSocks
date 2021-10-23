#!/usr/bin/env python3

from click import (
    group,
    pass_context,
    pass_obj,
    secho
)
from client import Client

@group()
@pass_context
def main(context) -> None:
    context.obj = Client()

@main.command(help='Ping all machines')
@pass_obj
def ping(obj):
    obj.connect()
    secho(obj.send('echo'))
    obj.disconnect()

if __name__ == '__main__':
    main()
