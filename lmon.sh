#!/bin/sh

#
#   Lmon install/update/remove script
#
#   Install agent:
#   curl -s https://get.oceancraft.ru/lmon.sh | sh -s agent
#
#   Install server:
#   curl -s https://get.oceancraft.ru/lmon.sh | sh -s server
#
#   Update lmon:
#   curl -s https://get.oceancraft.ru/lmon.sh | sh -s update
#
#   Remove lmon:
#   curl -s https://get.oceancraft.ru/lmon.sh | sh -s remove
#

# Some variables
readonly URL="https://get.oceancraft.ru/lmon"
readonly BIN_PATH="/usr/bin/lmon"
readonly SERVICE_PATH="/etc/systemd/system/lmon.service"

download_bin() {
    curl ${URL} -o ${BIN_PATH}
}

install_service() {
    # Create systemd service
    cat > ${SERVICE_PATH}-${1} << EOF
[Unit]
Description=LMon
After=network.target

[Service]
Type=exec
ExecStart=/usr/bin/lmon ${MODE}
KillMode=process
Delegate=yes

[Install]
WantedBy=multi-user.target
EOF
    # Enable and start service
    systemctl daemon-reload
    systemctl enable lmon
    systemctl start lmon
}

remove_service() {
    systemctl stop lmon
    systemctl disable lmon
    rm ${SERVICE_PATH}
    systemctl daemon-reload
}

remove_binary() {
    rm ${BIN_PATH}
}

install() {
    download_bin
    install_service
}

update() {
    remove_binary
    download_bin
}

remove() {
    remove_service
    remove_binary
}

if [[ $1 == "" ]]; then
    readonly MODE="agent"
elif [[ $1 == "agent" ]]; then
    readonly MODE=$1
elif [[ $1 == "server" ]]; then
    readonly MODE=$1
elif [[ $1 == "update" ]]; then
    update
    exit
elif [[ $1 == "remove" ]]; then
    remove
    exit
else
    echo "Invalid args"
    exit
fi

install