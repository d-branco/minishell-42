#!/bin/bash

make

echo "PS1='$(echo $?) % '" > ~/.bash_minishell_test

# Start a new tmux session in the background
tmux new-session -d -s test3

# Split the window vertically (left-right split)
tmux split-window -h "bash --norc"
# Split the bottom pane horizontally (top-bottom split)
tmux select-pane -D
tmux split-window -v "bash --norc"

# Send the echo command to the bottom two panes simultaneously
tmux select-pane -t 1
tmux send-keys 'clear ; ./minishell' C-m
tmux select-pane -t 2
tmux send-keys 'clear ; bash --noprofile --rcfile ~/.bash_minishell_test' C-m

# Send the echo command to the bottom two panes simultaneously
tmux select-pane -t 1
tmux send-keys 'echo "This is a pane"' C-m 
tmux select-pane -t 2
tmux send-keys 'echo "This is a pane"' C-m 

tmux select-pane -t 1 
#tmux send-keys 'echo $?' C-m
tmux capture-pane -pS - > ./pane1.txt
tmux select-pane -t 2 
#tmux send-keys 'echo $?' C-m
tmux capture-pane -pS - > ./pane2.txt

tmux select-pane -t 0 
tmux send-keys 'cat pane1.txt' C-m
tmux send-keys 'cat pane2.txt' C-m
tmux send-keys 'diff -B -c pane1.txt pane2.txt' C-m

# Attach to the tmux session to see the result
tmux attach -t test3