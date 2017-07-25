#!/bin/bash

ROOT_PATH=$(pwd)
BIN=$ROOT_PATH/httpd
CONF=$ROOT_PATH/conf/server.conf

id=''
proc=$(basename $0)
function usage()
{
	printf "Usage: %s [start(-s) | stop(-t) | restart(-rs) | status(-ss) ]\n" "$proc"
}
function server_status()
{
	server_bin=$(basename $BIN)
	id=$(pidof $server_bin)
	if [ ! -z "$id" ];then
		return 0
	else
		return 1
	fi
}

function service_status()
{
	if server_status;then
		echo "server is running: $id"
	else
		echo "server is not running: $id"
	fi
}

function server_start()
{
	if ! server_status; then
		ip=$(awk -F: '/^IP/{print $NF}' $CONF)
		port=$(awk -F: '/^PORT/{print $NF}' $CONF)
		$BIN $ip $port
	else
		echo "server is already running: $id,please stop first!"
	fi
}

function server_stop()
{
	if server_status; then
		kill -9 $id
	else
		echo "server is not running: $id, Please start!"
	fi
}

function server_restart()
{
	if server_status; then
		server_stop
	fi
	server_start
}

if [ $# -eq 0 ]; then
	usage
	exit1
fi

case $1 in
	start | -s )
	server_start
	;;
	stop | -t )
	server_stop
	;;
	restart | -rs )
	server_restart
	;;
	status | -ss )
	servers_status
	;;
	* )
	exit 1
	;;
esac

