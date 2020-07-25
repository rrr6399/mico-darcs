#!/bin/sh
PIDS=""
for i in `seq 1 10`
do
	cat ref|./client $@ >/dev/null &
	PIDS="$PIDS $!"
done
trap "kill $PIDS > /dev/null 2>&1" 0
wait
