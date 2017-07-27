#!/bin/zsh
killall ./dummyload
killall procmon
./dummyload c 60 1 > pid.txt &
spawn-fcgi -a 127.0.0.1 -p 8082 -f ./procmon
