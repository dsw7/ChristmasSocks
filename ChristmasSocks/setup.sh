# ----------------------------------------
# ChristmasSocks Remote Server Management Software
# ----------------------------------------

CONFIGURE_SCRIPT=$(dirname $0)/configure.py

echo -e "\e[1m\e[4mChristmasSocks Remote Server Management Software\e[0m"

if [ $(id --user) -ne 0 ];
    then echo "Please run as root!"
    exit 1
fi

echo -e "Starting installation"
echo -n "Please enter a valid Linux user: "
read USER

chmod +x ${CONFIGURE_SCRIPT}
su $USER --command "${CONFIGURE_SCRIPT} compile"
