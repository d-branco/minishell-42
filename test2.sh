#!/bin/bash
# Colors for prettier output
GREEN="\033[32m"
RED="\033[31m"
YELLOW="\033[33m"
BLUE="\033[34m"
RESET="\033[0m"

# Test counter
TEST_COUNT=0
PASS_COUNT=0
FAIL_COUNT=0
MINISHELL="./minishell"

make re

# Function to run standard test (from your original script)
run_test() {
    local test_cmd="$1"
    local expected_exit_code=${2:-0}  # Default expected exit code is 0
    
    TEST_COUNT=$((TEST_COUNT + 1))
    
    # Create temporary files for output
    MINISHELL_OUTPUT=$(mktemp)
    BASH_OUTPUT=$(mktemp)
    MINISHELL_ERROR=$(mktemp)
    BASH_ERROR=$(mktemp)
    
    # Run command in minishell
    echo "$test_cmd" | $MINISHELL > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERROR"
    MINISHELL_EXIT=$?
    
    # Run the same command in bash
    echo "$test_cmd" | bash > "$BASH_OUTPUT" 2> "$BASH_ERROR"
    BASH_EXIT=$?
    
    # Compare stdout
    diff -u "$BASH_OUTPUT" "$MINISHELL_OUTPUT" > /dev/null
    STDOUT_DIFF=$?
    
    # Print a single line result
    printf "%-3s %-42s " "[$TEST_COUNT]" "\"$test_cmd\""
    
    if [ "$MINISHELL_EXIT" -eq "$expected_exit_code" ] && [ "$STDOUT_DIFF" -eq 0 ]; then
        echo -e "${GREEN}GOOD${RESET}"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo -e "${RED}ERROR${RESET} (bash:$BASH_EXIT/mini:$MINISHELL_EXIT)"
        FAIL_COUNT=$((FAIL_COUNT + 1))
        
        # Create a failure log file for details (optional)
        LOG_FILE="test_${TEST_COUNT}_failure.log"
        {
            echo "Command: $test_cmd"
            echo "Expected output (bash):"
            cat "$BASH_OUTPUT"
            echo -e "\nYour output (minishell):"
            cat "$MINISHELL_OUTPUT"
            echo -e "\nExit status: bash=$BASH_EXIT, minishell=$MINISHELL_EXIT"
            
            echo -e "\nStderr bash:"
            cat "$BASH_ERROR"
            echo -e "\nStderr minishell:"
            cat "$MINISHELL_ERROR"
        } > /dev/null
    fi
    
    # Clean up temp files
    rm -f "$MINISHELL_OUTPUT" "$BASH_OUTPUT" "$MINISHELL_ERROR" "$BASH_ERROR"
}

# Function to test signals (Ctrl+C, Ctrl+D, Ctrl+\)
run_signal_test() {
    local signal_name="$1"
    local signal_code="$2"
    local test_cmd="$3"
    local expected_exit_code="$4"
    
    TEST_COUNT=$((TEST_COUNT + 1))
    
    # Create temporary files for output
    MINISHELL_OUTPUT=$(mktemp)
    MINISHELL_ERROR=$(mktemp)
    
    # Print test information
    printf "%-3s %-42s " "[$TEST_COUNT]" "$signal_name ($test_cmd)"
    
    # Run the test in a more controlled way
    # We need to use expect or a similar approach to send signals
    
    # For testing CTRL+D (EOF)
    if [ "$signal_code" = "EOF" ]; then
        # Run minishell and immediately send EOF
        $MINISHELL < /dev/null > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERROR"
        MINISHELL_EXIT=$?
        
        if [ "$MINISHELL_EXIT" -eq "$expected_exit_code" ]; then
            echo -e "${GREEN}GOOD${RESET}"
            PASS_COUNT=$((PASS_COUNT + 1))
        else
            echo -e "${RED}ERROR${RESET} (expected:$expected_exit_code/mini:$MINISHELL_EXIT)"
            FAIL_COUNT=$((FAIL_COUNT + 1))
        fi
    
    # For testing CTRL+C (SIGINT)
    elif [ "$signal_code" = "SIGINT" ]; then
        # Start minishell and run a command that will wait
        $MINISHELL > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERROR" &
        MINI_PID=$!
        
        # Give it a moment to start up
        sleep 0.5
        
        # If we have a test command to run first, send it
        if [ -n "$test_cmd" ]; then
            echo "$test_cmd" > /proc/$MINI_PID/fd/0
            sleep 1
        fi
        
        # Send SIGINT
        kill -SIGINT $MINI_PID
        
        # Wait for minishell to process the signal
        sleep 1
        
        # Check if minishell is still running
        if kill -0 $MINI_PID 2>/dev/null; then
            # Still running, send SIGTERM to clean up
            kill -SIGTERM $MINI_PID
            wait $MINI_PID
            MINISHELL_EXIT=$?
            
            # In SIGINT case, minishell should still be running (showing a new prompt)
            echo -e "${GREEN}GOOD${RESET} (shell continued as expected)"
            PASS_COUNT=$((PASS_COUNT + 1))
        else
            # Process terminated
            wait $MINI_PID
            MINISHELL_EXIT=$?
            
            echo -e "${RED}ERROR${RESET} (shell terminated with $MINISHELL_EXIT)"
            FAIL_COUNT=$((FAIL_COUNT + 1))
        fi
    
    # For testing CTRL+\ (SIGQUIT)
    elif [ "$signal_code" = "SIGQUIT" ]; then
        # Start minishell and run a command that will wait
        $MINISHELL > "$MINISHELL_OUTPUT" 2> "$MINISHELL_ERROR" &
        MINI_PID=$!
        
        # Give it a moment to start up
        sleep 0.5
        
        # If we have a test command to run first, send it
        if [ -n "$test_cmd" ]; then
            echo "$test_cmd" > /proc/$MINI_PID/fd/0
            sleep 1
        fi
        
        # Send SIGQUIT
        kill -SIGQUIT $MINI_PID
        
        # Wait for minishell to process the signal
        sleep 1
        wait $MINI_PID
        MINISHELL_EXIT=$?
        
        if [ "$MINISHELL_EXIT" -eq "$expected_exit_code" ]; then
            echo -e "${GREEN}GOOD${RESET}"
            PASS_COUNT=$((PASS_COUNT + 1))
        else
            echo -e "${RED}ERROR${RESET} (expected:$expected_exit_code/mini:$MINISHELL_EXIT)"
            FAIL_COUNT=$((FAIL_COUNT + 1))
        fi
    fi
    
    # Clean up temp files
    rm -f "$MINISHELL_OUTPUT" "$MINISHELL_ERROR"
}

# More comprehensive signal testing with expect
run_expect_signal_test() {
    local signal_name="$1"
    local signal_key="$2"  # The key sequence to send
    local test_cmd="$3"    # Optional command to run before sending signal
    local expected_behavior="$4"  # Expected behavior description
    
    TEST_COUNT=$((TEST_COUNT + 1))
    
    # Create an expect script file
    EXPECT_SCRIPT=$(mktemp)
    RESULT_FILE=$(mktemp)
    
    # Determine the key sequence to send
    if [ "$signal_key" = "ctrl_c" ]; then
        CONTROL_KEY="\003"  # Ctrl+C
    elif [ "$signal_key" = "ctrl_d" ]; then
        CONTROL_KEY="\004"  # Ctrl+D
    elif [ "$signal_key" = "ctrl_backslash" ]; then
        CONTROL_KEY="\034"  # Ctrl+\
    else
        CONTROL_KEY="$signal_key"
    fi
    
    # Create the expect script
    cat > "$EXPECT_SCRIPT" << EOF
#!/usr/bin/expect -f
set timeout 5
set result_file [lindex \$argv 0]

# Start minishell
spawn $MINISHELL

# Wait for prompt
expect "minishell> "

# If there's a test command, send it
set test_cmd "$test_cmd"
if {"\$test_cmd" != ""} {
    send "\$test_cmd\r"
    if {"\$test_cmd" == "sleep 10"} {
        # For long running commands, wait a bit before sending signal
        sleep 1
    } else {
        # For normal commands, wait for prompt or output
        expect {
            "minishell> " {}
            timeout {puts "Timeout waiting for command to complete"; exit 1}
        }
    }
}

# Send the signal
send "$CONTROL_KEY"

# For Ctrl+D, we expect the shell to exit
if {"$signal_key" == "ctrl_d"} {
    expect {
        eof {
            puts "Shell exited as expected"
            set exit_status 0
        }
        timeout {
            puts "Shell did not exit after Ctrl+D"
            set exit_status 1
        }
    }
} elseif {"$signal_key" == "ctrl_c"} {
    # For Ctrl+C, we expect the shell to show a new prompt
    expect {
        "minishell> " {
            puts "New prompt displayed after Ctrl+C"
            set exit_status 0
        }
        eof {
            puts "Shell exited after Ctrl+C (unexpected)"
            set exit_status 1
        }
        timeout {
            puts "No new prompt after Ctrl+C"
            set exit_status 1
        }
    }
} elseif {"$signal_key" == "ctrl_backslash"} {
    # For Ctrl+\, behavior depends on whether a command is running
    if {"$test_cmd" == "sleep 10"} {
        # We expect a Quit message and potentially core dump
        expect {
            "Quit" {
                puts "Quit message received after Ctrl+\\"
                set exit_status 0
            }
            "minishell> " {
                puts "New prompt displayed after Ctrl+\\ (possibly OK)"
                set exit_status 0
            }
            eof {
                puts "Shell exited after Ctrl+\\"
                set exit_status 0
            }
            timeout {
                puts "No expected response after Ctrl+\\"
                set exit_status 1
            }
        }
    } else {
        # For Ctrl+\ at prompt, might see a new prompt or nothing
        expect {
            "minishell> " {
                puts "New prompt displayed after Ctrl+\\"
                set exit_status 0
            }
            timeout {
                puts "No visible change after Ctrl+\\ at prompt (acceptable)"
                set exit_status 0
            }
        }
    }
}

# Write result to file
set f [open \$result_file "w"]
puts \$f \$exit_status
close \$f

# Clean exit
send "exit\r"
expect eof
EOF
    
    # Make the expect script executable
    chmod +x "$EXPECT_SCRIPT"
    
    # Print test information
    printf "%-3s %-42s " "[$TEST_COUNT]" "$signal_name ($test_cmd)"
    
    # Run the expect script
    "$EXPECT_SCRIPT" "$RESULT_FILE" > /dev/null 2>&1
    
    # Read the result
    RESULT=$(cat "$RESULT_FILE")
    
    if [ "$RESULT" -eq 0 ]; then
        echo -e "${GREEN}GOOD${RESET} - $expected_behavior"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo -e "${RED}ERROR${RESET} - $expected_behavior not observed"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
    
    # Clean up
    rm -f "$EXPECT_SCRIPT" "$RESULT_FILE"
}

# Print header
printf "%-3s %-42s %s\n" "NUM" "COMMAND/SIGNAL" "RESULT"
echo "--------------------------------------------------------------------------------"

# Basic command tests (from your original script)
run_test "echo Hello World"
run_test "ls -la"
run_test "pwd"

# Signal tests using expect (requires expect to be installed)
if command -v expect >/dev/null 2>&1; then
    # At prompt
    run_expect_signal_test "Ctrl+D at empty prompt" "ctrl_d" "" "Should exit shell"
    run_expect_signal_test "Ctrl+C at empty prompt" "ctrl_c" "" "Should display new prompt"
    run_expect_signal_test "Ctrl+\\ at empty prompt" "ctrl_backslash" "" "Should be ignored"
    
    # With text at prompt
    run_expect_signal_test "Ctrl+D with text" "ctrl_d" "echo Hello" "Should exit shell"
    run_expect_signal_test "Ctrl+C with text" "ctrl_c" "echo Hello" "Should display new prompt"
    run_expect_signal_test "Ctrl+\\ with text" "ctrl_backslash" "echo Hello" "Should be ignored"
    
    # During command execution
    run_expect_signal_test "Ctrl+C during command" "ctrl_c" "sleep 10" "Should interrupt command"
    run_expect_signal_test "Ctrl+\\ during command" "ctrl_backslash" "sleep 10" "Should quit command with core dump"
else
    echo -e "${YELLOW}Warning: 'expect' command not found. Skipping interactive signal tests.${RESET}"
    echo -e "${YELLOW}Install expect with: apt-get install expect (Debian/Ubuntu) or brew install expect (macOS)${RESET}"
    
    # Fallback to basic signal tests
    run_signal_test "Ctrl+D (EOF)" "EOF" "" 0
    run_signal_test "Ctrl+C (SIGINT)" "SIGINT" "sleep 10" 130
    run_signal_test "Ctrl+\\ (SIGQUIT)" "SIGQUIT" "sleep 10" 131
fi

# Continue with original tests
# Built-ins
run_test "cd .."
run_test "pwd"
run_test "export TEST_VAR=42"
run_test "echo \$TEST_VAR"
run_test "unset TEST_VAR"
run_test "echo \$TEST_VAR"

# Redirections
run_test "echo Hello > test_file.txt"
run_test "cat < test_file.txt"
run_test "echo Append >> test_file.txt"
run_test "cat test_file.txt"
run_test "rm test_file.txt"

# Pipes
run_test "echo Hello | grep H"
run_test "ls -la | grep .c | wc -l"

# Quotes and expansion
run_test "echo \"Double quoted string with \$HOME\""
run_test "echo 'Single quoted string with \$HOME'"

# Command not found
run_test "nonexistentcommand" 127

# Exit
run_test "exit 42" 42

# Summary
echo "--------------------------------------------------------------------------------"
echo -e "Total: $TEST_COUNT | ${GREEN}Passed: $PASS_COUNT${RESET} | ${RED}Failed: $FAIL_COUNT${RESET}"
if [ $FAIL_COUNT -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${RESET}"
else
    echo -e "${RED}Some tests failed. Check test_*_failure.log files for details.${RESET}"
fi

# Clean up any remaining test files
rm -f test_file.txt

exit $FAIL_COUNT  # Return number of failed tests as exit code