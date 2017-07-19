#/bin/zsh
killall procmon

spawn-fcgi -a 127.0.0.1 -p 8082 -f ./procmon
