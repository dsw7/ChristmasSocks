#!/usr/bin/env python3

from click import (
    group,
    pass_context,
    pass_obj
)

@group()
@pass_context
def main(context):
    context.obj = 'Handle to ChristmasSocks adapter'

@main.command(help='Ping all machines')
@pass_obj
def ping(obj):
    # TODO Start filling this out with commands for socks server
    pass

if __name__ == '__main__':
    main()
