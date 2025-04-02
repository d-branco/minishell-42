#!/bin/bash

echo "PS1='\$(echo \$?)% '" > ~/.bash_minishell_test
make minishell
#make clean

echo -e "=========================================="\
			"                             MINISHELL"
./playground/exp_minishell.exp "$@" > out_minishell.txt
sed -i 's/\x1B\[[0-9;?]*[a-zA-Z]//g' out_minishell.txt
cat out_minishell.txt
sleep 0.2
echo -e "=========================================="\
			"                                  BASH"
./playground/exp_bash.exp "$@" > out_bash.txt
sed -i 's/\x1B\[[0-9;?]*[a-zA-Z]//g' out_bash.txt
cat out_bash.txt
echo -e "=========================================="\
			"                                  diff"
diff -B -c out_minishell.txt out_bash.txt
DIFF_EXIT_CODE=$?

rm -f out_minishell.txt out_bash.txt
exit $DIFF_EXIT_CODE
