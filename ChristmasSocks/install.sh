# ----------------------------------------
# ChristmasSocks Remote Server Management Software
# ----------------------------------------

SRC_BINARY=$(dirname $0)/bin
BINARY_NAME=socks
DST_BINARY=/usr/bin

CONFIG_FILE=socks.ini
SRC_CONFIG=$(dirname $0)/src/server/configs
DST_CONFIG=/etc

SERVICE_FILE=socks.service
SERVICE_NAME=socks.service
SRC_SERVICE_FILE=$(dirname $0)/systemd
DST_SERVICE_FILE=/etc/systemd/system

echo -e "\e[1m\e[4mChristmasSocks Remote Server Management Software\e[0m\n"

if [ $(id --user) -ne 0 ];
    then echo "Please run as root!"
    exit 1
fi

echo "Starting installation"
echo -n "Please enter a valid Linux user: "
read USER

compile_binary()
{
    echo "Compiling binary"
    su ${USER} -c "cmake -S $(dirname $0) -B ${SRC_BINARY} && make --jobs=12 -C ${SRC_BINARY}"
    if [ $? -ne 0 ];
        then exit 1
    fi
}

copy_binary()
{
    echo "Copying ${BINARY_NAME} binary to ${DST_BINARY}/"
    cp -v ${SRC_BINARY}/${BINARY_NAME} ${DST_BINARY}/
    if [ $? -ne 0 ];
        then exit 1
    fi
}

copy_config_file()
{
    echo "Copying ${CONFIG_FILE} to ${DST_CONFIG}/"
    cp -v ${SRC_CONFIG}/${CONFIG_FILE} ${DST_CONFIG}/
    if [ $? -ne 0 ];
        then exit 1
    fi
}

copy_service_file()
{
    echo "Preparing ChristmasSocks service"
    cp -v ${SRC_SERVICE_FILE}/${SERVICE_FILE} ${DST_SERVICE_FILE}/
    if [ $? -ne 0 ];
        then exit 1
    fi
}

update_service_file()
{
    echo "Updating ${SERVICE_FILE} User field"
    sed -i "s/username/${USER}/" ${DST_SERVICE_FILE}/${SERVICE_FILE}
    if [ $? -ne 0 ];
        then exit 1
    fi

    echo "Updating ${SERVICE_FILE} ExecStart command"
    sed -i "s@execstart@${DST_BINARY}\/${BINARY_NAME} --config ${DST_CONFIG}\/${CONFIG_FILE}@" ${DST_SERVICE_FILE}/${SERVICE_FILE}
    if [ $? -ne 0 ];
        then exit 1
    fi
}

setup_service()
{
    systemctl daemon-reload

    echo "Starting up ${SERVICE_NAME}!"
    systemctl start ${SERVICE_NAME}

    echo "Enabling ${SERVICE_NAME}!"
    systemctl enable ${SERVICE_NAME}
    echo "Please run 'sudo systemctl status ${SERVICE_NAME}' to ensure the installation succeeded"
}

install()
{
    compile_binary
    copy_binary
    copy_config_file
    copy_service_file
    update_service_file
    setup_service
}

install
