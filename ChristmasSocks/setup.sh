# ----------------------------------------
# ChristmasSocks Remote Server Management Software
# ----------------------------------------

CONFIGURE_SCRIPT=$(dirname $0)/configure.py
FILEPATH_BINARY=$(dirname $0)/bin/socks
DIR_BIN=/usr/bin/

echo -e "\e[1m\e[4mChristmasSocks Remote Server Management Software\e[0m"

if [ $(id --user) -ne 0 ];
    then echo "Please run as root!"
    exit 1
fi

echo "Starting installation"
echo -n "Please enter a valid Linux user: "
read USER
echo

chmod +x ${CONFIGURE_SCRIPT}

echo "Compiling binary..."
su $USER --command "${CONFIGURE_SCRIPT} compile"

if [ $? -ne 0 ];
    then echo "Failed to compile binary!"
    exit 1
fi

echo "Copying binary into system bin directory"
echo "${FILEPATH_BINARY} -> ${DIR_BIN}"
cp ${FILEPATH_BINARY} ${DIR_BIN}
if [ $? -ne 0 ];
    then echo "Failed to copy binary!"
    exit 1
fi

echo
chown $USER ${FILEPATH_BINARY}
