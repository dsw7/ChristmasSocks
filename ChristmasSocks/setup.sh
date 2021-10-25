# ----------------------------------------
# ChristmasSocks Remote Server Management Software
# ----------------------------------------

echo -e "\e[1m\e[4mChristmasSocks Remote Server Management Software\e[0m"

if [ "$EUID" -ne 0 ]
    then echo "Please run as root!"
    exit 1
fi
