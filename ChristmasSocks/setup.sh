# ----------------------------------------
# ChristmasSocks Remote Server Management Software
# ----------------------------------------

CONFIGURE_SCRIPT=$(dirname $0)/configure.py

echo -e "\e[1m\e[4mChristmasSocks Remote Server Management Software\e[0m"

if [ "$EUID" -ne 0 ]
    then echo "Please run as root!"
    exit 1
fi

chmod +x ${CONFIGURE_SCRIPT}
${CONFIGURE_SCRIPT} compile
