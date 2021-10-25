# ----------------------------------------
# ChristmasSocks Remote Server Management Software
# ----------------------------------------

FILEPATH_BINARY=/usr/bin/socks
SERVICE_FILE=socks.service
SERVICE_NAME=socks.service
PATH_SERVICE_FILE=/etc/systemd/system/${SERVICE_FILE}

echo -e "\e[1m\e[4mChristmasSocks Remote Server Management Software\e[0m"
echo

if [ $(id --user) -ne 0 ];
    then echo "Please run as root!"
    exit 1
fi

echo -n "Please enter a valid Linux user: "
read USER
echo

echo "Stopping ${SERVICE_NAME} service..."
systemctl stop ${SERVICE_NAME}

echo "Disabling ${SERVICE_NAME} service..."
systemctl disable ${SERVICE_NAME}

echo "Removing binary from system bin directory..."
rm -v ${FILEPATH_BINARY}
if [ $? -ne 0 ];
    then echo "Failed to remove binary!"
    exit 1
fi

echo "Cleaning up service files..."
rm -v ${PATH_SERVICE_FILE} ${PATH_SYSTEMCTL}
if [ $? -ne 0 ];
    then echo "Failed to remove service file!"
    exit 1
fi

systemctl daemon-reload
