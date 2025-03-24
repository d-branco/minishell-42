#!/bin/bash

echo "PS1='\$(echo \$?)% '" > ~/.bash_minishell_test

# Pass all the arguments (commands) to expect
#./expect_script.exp "$@"

#./expect_script.exp "ls" "echo hello" "echo \"hello world\"" 
#./expect_script.exp "ls" "echo hello" "\x04" > out.txt

echo -e "==========================================                             MINISHELL" > out.txt
./exp_minishell.exp "" "" >> out.txt
sleep 0.2
echo -e "==========================================                                  BASH" >> out.txt
./exp_bash.exp "" "" >> out.txt

cat out.txt