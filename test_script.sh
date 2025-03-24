#!/bin/bash

#bind 'set enable-bracketed-paste on'

# Pass all the arguments (commands) to expect
#./expect_script.exp "$@"

#./expect_script.exp "ls" "echo hello" "echo \"hello world\"" 
#./expect_script.exp "ls" "echo hello" "\x04" > out.txt


./exp_minishell.exp "Oi" "" > out.txt
sleep 0.2
echo -e "=========================================" >> out.txt
./exp_bash.exp "Oi"  "" >> out.txt