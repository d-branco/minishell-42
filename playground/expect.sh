#!/usr/bin/expect -f

set timeout 2

spawn -noecho ./minishell
sleep 0.01
send "\004"
expect eof

#sleep 1
#
#spawn -noecho bash --noprofile --rcfile ~/.bash_minishell_test
#sleep 0.01
#send "\004"
#expect eof