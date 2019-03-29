#!/bin/sh
PIDS=""
for i in `seq 1 100`
do
	cat ref|./client -ORBBindAddr inet:localhost:7788 $@ >/dev/null &
	PIDS="$PIDS $!"
done
trap "kill $PIDS > /dev/null 2>&1" 0
wait
