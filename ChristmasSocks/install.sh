# ----------------------------------------
# ChristmasSocks Remote Server Management Software
# ----------------------------------------

SRC_BINARY=$(dirname $0)/bin
BINARY_NAME=socks
DST_BINARY=/usr/bin
SERVICE_FILE=socks.service
CONFIG_FILE=socks.ini
SERVICE_NAME=socks.service
PATH_SERVICE_FILE=$(dirname $0)/systemd/${SERVICE_FILE}
PATH_CONFIG_FILE=$(dirname $0)/src/server/configs/${CONFIG_FILE}
PATH_SYSTEMCTL=/etc/systemd/system
DIR_CONFIG=/etc

echo -e "\e[1m\e[4mChristmasSocks Remote Server Management Software\e[0m\n"

if [ $(id --user) -ne 0 ];
    then echo "Please run as root!"
    exit 1
fi

echo "Starting installation"
echo -n "Please enter a valid Linux user: "
read USER

echo "Compiling binary"
su ${USER} -c "cmake -S $(dirname $0) -B ${SRC_BINARY} && make --jobs=12 -C ${SRC_BINARY}"
if [ $? -ne 0 ];
    then exit 1
fi

echo "Copying binary into system bin directory"
cp -v ${SRC_BINARY}/${BINARY_NAME} ${DST_BINARY}/
if [ $? -ne 0 ];
    then exit 1
fi

echo "Copying ${CONFIG_FILE} to ${DIR_CONFIG}/"
cp -v ${PATH_CONFIG_FILE} ${DIR_CONFIG}/
if [ $? -ne 0 ];
    then exit 1
fi

echo "Preparing ChristmasSocks service"
cp -v ${PATH_SERVICE_FILE} ${PATH_SYSTEMCTL}/
if [ $? -ne 0 ];
    then exit 1
fi

echo "Updating ${SERVICE_FILE} User field"
sed -i "s/username/${USER}/" ${PATH_SYSTEMCTL}/${SERVICE_FILE}
if [ $? -ne 0 ];
    then exit 1
fi

echo "Updating ${SERVICE_FILE} ExecStart command"
sed -i "s@execstart@${DST_BINARY}\/${BINARY_NAME} --config ${DIR_CONFIG}\/${CONFIG_FILE}@" ${PATH_SYSTEMCTL}/${SERVICE_FILE}
if [ $? -ne 0 ];
    then exit 1
fi

systemctl daemon-reload

echo "Starting up ${SERVICE_NAME}!"
systemctl start ${SERVICE_NAME}

echo "Enabling ${SERVICE_NAME}!"
systemctl enable ${SERVICE_NAME}
echo "Please run 'sudo systemctl status ${SERVICE_NAME}' to ensure the installation succeeded"
