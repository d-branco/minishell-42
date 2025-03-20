#!/bin/bash
# Test script for Minishell
MINISHELL_PATH="./minishell"
echo "" > output.txt
# Test 1: Check if Minishell starts
echo -n "Running test: Start minishell                                "
$MINISHELL_PATH <<< "exit" >> output.txt
if [ $? -eq 0 ]; then
    echo "    GOOD!"
else
    echo -e "\n    Test Failed: Minishell did not start correctly."
fi

echo "" >> output.txt
# Test 2: Run a simple command (e.g., 'ls')
echo -n "Running test: Run 'ls' command                               "
$MINISHELL_PATH <<< "ls" >> output.txt
if [ $? -eq 0 ]; then
    echo "    GOOD!"
else
    echo -e "\n    Test Failed: 'ls' command did not execute correctly."
fi

echo "" >> output.txt
# Test 3: Check for built-in echo command
echo -n "Running test: Run 'echo Hello World'                         "
$MINISHELL_PATH <<< "echo Hello World" >> output.txt
if [ $? -eq 0 ] && grep -q "Hello World" output.txt; then
    echo "    GOOD!"
else
    echo -e "\n    Test Failed: 'echo' command did not work correctly."
fi

echo "" >> output.txt
# Test 4: Check for built-in pwd command
echo -n "Running test: Run 'pwd'                                      "
$MINISHELL_PATH <<< "pwd" >> output.txt
if [ $? -eq 0 ] && [ -n "$(cat output.txt)" ]; then
    echo "    GOOD!"
else
    echo -e "\n    Test Failed: 'pwd' command did not work correctly."
fi

echo "" >> output.txt
# Test 5: Check for external command (e.g., 'cat')
echo -n "Running test: Run 'cat /etc/passwd'                          "
$MINISHELL_PATH <<< "cat /etc/passwd" >> output.txt
if [ $? -eq 0 ] && [ -n "$(cat output.txt)" ]; then
    echo "    GOOD!"
else
    echo "Test Failed: External command 'cat' did not execute correctly."
fi

echo "" >> output.txt
# Test 6: Check if invalid command returns an error
echo -n "Running test: Run invalid syntax                             "
$MINISHELL_PATH <<< "invalid\"command" >> output.txt 2>&1
if [ $? -ne 0 ] && grep -q "syntax error" output.txt; then
    echo "    GOOD!"
else
    echo -e "\n    Test Failed: Invalid command did not produce an error."
fi