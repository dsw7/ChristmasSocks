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
    cp ${SRC_BINARY}/${BINARY_NAME} ${DST_BINARY}/
    if [ $? -ne 0 ];
        then exit 1
    fi
}

remove_binary()
{
    echo "Removing ${BINARY_NAME} binary from ${DST_BINARY}/"
    rm ${DST_BINARY}/${BINARY_NAME}
    if [ $? -ne 0 ];
        then exit 1
    fi
}

copy_config_file()
{
    echo "Copying ${CONFIG_FILE} to ${DST_CONFIG}/"
    cp ${SRC_CONFIG}/${CONFIG_FILE} ${DST_CONFIG}/
    if [ $? -ne 0 ];
        then exit 1
    fi
}

remove_config_file()
{
    echo "Removing ${CONFIG_FILE} from ${DST_CONFIG}/"
    rm ${DST_CONFIG}/${CONFIG_FILE}
    if [ $? -ne 0 ];
        then exit 1
    fi
}

update_config_file()
{
    echo
    local tcp_port
    read -p "Specify a TCP port [default: 8080]: " tcp_port
    local tcp_port=${tcp_port:-8080}
    sed -i "s/<port>/${tcp_port}/" ${DST_CONFIG}/${CONFIG_FILE}

    if [ $? -ne 0 ];
        then exit 1
    fi
    echo "Service will listen on port ${tcp_port}"

}

copy_service_file()
{
    echo "Preparing ChristmasSocks service"
    cp ${SRC_SERVICE_FILE}/${SERVICE_FILE} ${DST_SERVICE_FILE}/
    if [ $? -ne 0 ];
        then exit 1
    fi
}

remove_service_file()
{
    echo "Removing ${SERVICE_FILE} from ${DST_SERVICE_FILE}"
    rm ${DST_SERVICE_FILE}/${SERVICE_FILE}
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

    echo
    systemctl status ${SERVICE_NAME}
}

teardown_service()
{
    echo "Stopping ${SERVICE_NAME} service"
    systemctl stop ${SERVICE_NAME}

    echo "Disabling ${SERVICE_NAME}!"
    systemctl disable ${SERVICE_NAME}
}

install()
{
    echo -n "Please enter a valid Linux user: "
    read USER
    echo

    compile_binary
    copy_binary
    copy_config_file
    update_config_file
    copy_service_file
    update_service_file
    setup_service
}

uninstall()
{
    teardown_service
    remove_service_file
    systemctl daemon-reload
    remove_config_file
    remove_binary
}

echo "Select setup type:"
echo "[1] -> Install product"
echo "[2] -> Uninstall product"
echo -n "> "
read SETUP_TYPE
echo

if [ $SETUP_TYPE = 1 ]
then
    install
elif [ $SETUP_TYPE = 2 ]
then
    uninstall
else
    echo "Unrecognized setup option!"
    exit 1
fi
