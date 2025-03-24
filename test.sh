#!/bin/bash

GREEN="\033[32m"
RED="\033[31m"
YELLOW="\033[33m"
BLUE="\033[34m"
RESET="\033[0m"
TEST_COUNT=0
PASS_COUNT=0
FAIL_COUNT=0
MINISHELL="./minishell"

make all
rm log/*.txt

run_test() {
    local test_cmd="$1"
    local expected_exit_code=${2:-0}
    TEST_COUNT=$((TEST_COUNT + 1))
    MINISHELL_OUTPUT=$(mktemp)
    BASH_OUTPUT=$(mktemp)
    MINISHELL_ERROR=$(mktemp)
    BASH_ERROR=$(mktemp)
    echo "$test_cmd" | $MINISHELL > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERROR"
    MINISHELL_EXIT=$?
    echo "$test_cmd" | bash > "$BASH_OUTPUT" 2> "$BASH_ERROR"
    BASH_EXIT=$?
    diff -u "$BASH_OUTPUT" "$MINISHELL_OUTPUT" > /dev/null
    STDOUT_DIFF=$?
    printf "%3s %-42s " "$TEST_COUNT" "\"$test_cmd\""
    if [ "$MINISHELL_EXIT" -eq "$expected_exit_code" ] && [ "$STDOUT_DIFF" -eq 0 ]; then
        echo -e "${GREEN}GOOD${RESET}  ($MINISHELL_EXIT/$expected_exit_code)"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo -e -n "${RED}ERROR${RESET} "
        if [ "$MINISHELL_EXIT" -eq "$BASH_EXIT" ]; then
            echo -e "($MINISHELL_EXIT/$BASH_EXIT)"
        else
            echo -e "${RED}($MINISHELL_EXIT not $BASH_EXIT)${RESET}"
        fi
        FAIL_COUNT=$((FAIL_COUNT + 1))
        LOG_FILE="test_${TEST_COUNT}.txt"
        {
            echo "Command: $test_cmd"
            echo "------------------------------------------(bash):"
            cat "$BASH_OUTPUT"
            
            echo -e "\n------------------------------------------(minishell):"
            cat "$MINISHELL_OUTPUT"
            echo -e "\n------------------------------------------"
            echo -e "\nExit status: \nbash      = $BASH_EXIT\nminishell = $MINISHELL_EXIT"
            
            echo -e "\nStderr bash:"
            cat "$BASH_ERROR"
            echo -e "\nStderr minishell:"
            cat "$MINISHELL_ERROR"
        } > "log/$LOG_FILE"
    fi
    rm -f "$MINISHELL_OUTPUT" "$BASH_OUTPUT" "$MINISHELL_ERROR" "$BASH_ERROR"
}

run_signal_test() {
    local signal_name="$1"
    local signal_code="$2"
    local test_cmd="$3"
    local expected_exit_code="$4"
    TEST_COUNT=$((TEST_COUNT + 1))
    MINISHELL_OUTPUT=$(mktemp)
    MINISHELL_ERROR=$(mktemp)
    printf "%3s %-42s " "$TEST_COUNT" "$test_cmd ($signal_name)"
    # CTRL+D (EOF)
    if [ "$signal_code" = "EOF" ]; then
        $MINISHELL < /dev/null > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERROR"
        MINISHELL_EXIT=$?
        if [ "$MINISHELL_EXIT" -eq "$expected_exit_code" ]; then
            echo -e "${GREEN}GOOD${RESET}  ($expected_exit_code/$MINISHELL_EXIT)"
            PASS_COUNT=$((PASS_COUNT + 1))
        else
            echo -e "${RED}ERROR ($MINISHELL_EXIT not $expected_exit_code)${RESET}"
            FAIL_COUNT=$((FAIL_COUNT + 1))
        fi
    # CTRL+C (SIGINT)
    elif [ "$signal_code" = "SIGINT" ]; then
        $MINISHELL < /dev/null > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERROR" &
        MINISHELL_PID=$!
        kill -SIGINT $MINISHELL_PID
        wait $MINISHELL_PID
        MINISHELL_EXIT=$?
        if [ "$MINISHELL_EXIT" -eq "$expected_exit_code" ]; then
            echo -e "${GREEN}GOOD${RESET}  ($MINISHELL_EXIT/$expected_exit_code)"
            PASS_COUNT=$((PASS_COUNT + 1))
        else
            echo -e "${RED}ERROR ($MINISHELL_EXIT not $expected_exit_code)${RESET}"
            FAIL_COUNT=$((FAIL_COUNT + 1))
        fi
    # CTRL+\ (SIGQUIT)
    elif [ "$signal_code" = "SIGQUIT" ]; then
        $MINISHELL < /dev/null > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERROR" &
        MINISHELL_PID=$!
        kill -SIGQUIT $MINISHELL_PID
        wait $MINISHELL_PID
        MINISHELL_EXIT=$?
        
        if [ "$MINISHELL_EXIT" -eq "$expected_exit_code" ]; then
            echo -e "${GREEN}GOOD${RESET}  ($expected_exit_code/$MINISHELL_EXIT)"
            PASS_COUNT=$((PASS_COUNT + 1))
        else
            echo -e "${RED}ERROR ($MINISHELL_EXIT not $expected_exit_code)${RESET}"
            FAIL_COUNT=$((FAIL_COUNT + 1))
        fi
    fi
    rm -f "$MINISHELL_OUTPUT" "$MINISHELL_ERROR"
}

printf "%-3s  %-42s%s\n" "NUM" "COMMAND" "RESULT RETURN "
# echo "Signal testing"
run_signal_test "Ctrl+D (EOF)" "EOF" "" 0
run_signal_test "Ctrl+C (SIGINT)" "SIGINT" "" 130
run_signal_test "Ctrl+\\ (SIGQUIT)" "SIGQUIT" "" 131
# echo "Invalid Commands"
run_test "Invalid \"singular double quote"
# echo "Basic command tests"
run_test "echo Hello World"
run_test "ls -la"
run_test "pwd"
# echo "Built-ins"
run_test "cd .."
run_test "pwd"
run_test "export TEST_VAR=42"
run_test "echo \$TEST_VAR"
run_test "unset TEST_VAR"
run_test "echo \$TEST_VAR"
# echo "Redirections"
run_test "echo Hello > test_file.txt"
run_test "cat < test_file.txt"
run_test "echo Append >> test_file.txt"
run_test "cat test_file.txt"
run_test "rm test_file.txt"
# echo "Pipes"
run_test "echo Hello | grep H"
run_test "ls -la | grep .c | wc -l"
# echo "Quotes and expansion"
run_test "echo \"Double quoted plus \$HOME\""
run_test "echo 'Single quoted plus \$HOME'"
# echo "Command not found"
run_test "nonexistentcommand" 127
# echo "Exit"
run_test "exit 42" 42

echo -e "    Total: $TEST_COUNT | ${GREEN}Passed: $PASS_COUNT${RESET} | ${RED}Failed: $FAIL_COUNT${RESET}"
rm -f test_file.txt

exit $FAIL_COUNT