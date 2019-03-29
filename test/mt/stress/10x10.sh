#!/bin/sh
PIDS=""
for j in `seq 0 9`
do
for i in `seq 0 9`
do
	cat ref|./client -ORBBindAddr inet:localhost:7788 bind#bench$i $@ >/dev/null &
	PIDS="$PIDS $!"
done
done
trap "kill $PIDS > /dev/null 2>&1" 0
wait
