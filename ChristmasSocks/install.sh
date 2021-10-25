# ----------------------------------------
# ChristmasSocks Remote Server Management Software
# ----------------------------------------

CONFIGURE_SCRIPT=$(dirname $0)/configure.py
FILEPATH_BINARY=$(dirname $0)/bin/socks
DIR_BIN=/usr/bin/
SERVICE_FILE=socks.service
SERVICE_NAME=socks.service
PATH_SERVICE_FILE=$(dirname $0)/systemd/${SERVICE_FILE}
PATH_SYSTEMCTL=/etc/systemd/system/

echo -e "\e[1m\e[4mChristmasSocks Remote Server Management Software\e[0m"

if [ $(id --user) -ne 0 ];
    then echo "Please run as root!"
    exit 1
fi

echo "Starting installation..."
echo -n "Please enter a valid Linux user: "
read USER

chmod +x ${CONFIGURE_SCRIPT}
if [ $? -ne 0 ];
    then echo "Could not make ${CONFIGURE_SCRIPT} executable!"
    exit 1
fi

echo "Compiling binary..."
su $USER --command "${CONFIGURE_SCRIPT} compile"
if [ $? -ne 0 ];
    then echo "Failed to compile binary!"
    exit 1
fi

echo "Copying binary into system bin directory..."
cp -v ${FILEPATH_BINARY} ${DIR_BIN}
if [ $? -ne 0 ];
    then echo "Failed to copy binary!"
    exit 1
fi

chown -v $USER ${FILEPATH_BINARY}
if [ $? -ne 0 ];
    then echo "Failed to change ownership of binary!"
    exit 1
fi

echo "Preparing ChristmasSocks service..."
cp -v ${PATH_SERVICE_FILE} ${PATH_SYSTEMCTL}
if [ $? -ne 0 ];
    then echo "Failed to copy service file!"
    exit 1
fi

echo "Updating ${SERVICE_FILE} to match specified user..."
sed -i "s/username/${USER}/" ${PATH_SYSTEMCTL}/${SERVICE_FILE}
if [ $? -ne 0 ];
    then echo "Failed to update ${SERVICE_FILE}!"
    exit 1
fi

systemctl daemon-reload

echo "Starting up ${SERVICE_NAME}!"
systemctl start ${SERVICE_NAME}

echo "Enabling ${SERVICE_NAME}!"
systemctl enable ${SERVICE_NAME}
echo "Please run 'sudo systemctl status ${SERVICE_NAME} to ensure the installation succeeded'"
