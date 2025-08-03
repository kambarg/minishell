#!/bin/bash

echo "=== Testing Signal Handling: Consecutive Ctrl+C ==="
echo "This script will test how bash and minishell handle consecutive Ctrl+C signals"
echo ""

# Function to test bash behavior
test_bash_signal() {
    echo "=== Testing BASH Signal Handling ==="
    echo "Starting bash subprocess..."
    
    # Create a temporary script that will send SIGINT to bash
    cat > test_bash_sigint.sh << 'EOF'
#!/bin/bash
# Start bash in background and send SIGINT signals
bash -c '
echo "Bash ready for testing (PID: $$)"
echo "Sending consecutive SIGINT signals..."
kill -SIGINT $$
sleep 0.1
kill -SIGINT $$
sleep 0.1
kill -SIGINT $$
echo "Exit status after signals: $?"
exit $?
' &
wait $!
echo "Bash exit status: $?"
EOF
    
    chmod +x test_bash_sigint.sh
    ./test_bash_sigint.sh
    bash_exit_status=$?
    echo "Final bash exit status: $bash_exit_status"
    echo ""
}

# Function to test minishell behavior
test_minishell_signal() {
    echo "=== Testing MINISHELL Signal Handling ==="
    echo "Starting minishell subprocess..."
    
    # Create a temporary script that will send SIGINT to minishell
    cat > test_minishell_sigint.sh << 'EOF'
#!/bin/bash
# Start minishell in background and send SIGINT signals
./minishell &
MINISHELL_PID=$!
echo "Minishell started (PID: $MINISHELL_PID)"
sleep 1

echo "Sending consecutive SIGINT signals to minishell..."
kill -SIGINT $MINISHELL_PID
sleep 0.5
kill -SIGINT $MINISHELL_PID
sleep 0.5
kill -SIGINT $MINISHELL_PID
sleep 0.5

# Send EOF to terminate minishell gracefully
kill -TERM $MINISHELL_PID 2>/dev/null
wait $MINISHELL_PID 2>/dev/null
minishell_exit_status=$?
echo "Minishell exit status: $minishell_exit_status"
EOF
    
    chmod +x test_minishell_sigint.sh
    ./test_minishell_sigint.sh
    echo ""
}

# Function to test interactive behavior manually
test_interactive_comparison() {
    echo "=== Interactive Comparison Test ==="
    echo "This will help you manually test the interactive behavior"
    echo ""
    
    echo "1. First, test BASH:"
    echo "   - Run: bash"
    echo "   - Press Ctrl+C multiple times consecutively"
    echo "   - Note the behavior (new lines, prompt redisplay)"
    echo "   - Type 'echo \$?' to check exit status"
    echo "   - Type 'exit' to quit"
    echo ""
    
    echo "2. Then, test MINISHELL:"
    echo "   - Run: ./minishell"
    echo "   - Press Ctrl+C multiple times consecutively"
    echo "   - Note the behavior (new lines, prompt redisplay)"
    echo "   - Type 'echo \$?' to check exit status"
    echo "   - Press Ctrl+D or type 'exit' to quit"
    echo ""
    
    echo "Expected behavior in bash:"
    echo "- Ctrl+C should display '^C' and move to new line with fresh prompt"
    echo "- Exit status should remain 0 for interactive mode"
    echo "- Multiple Ctrl+C should each create new prompt lines"
    echo ""
    
    read -p "Press Enter to start interactive bash test..."
    echo "Starting bash (press Ctrl+C multiple times, then type 'exit'):"
    bash
    
    read -p "Press Enter to start interactive minishell test..."
    echo "Starting minishell (press Ctrl+C multiple times, then press Ctrl+D):"
    ./minishell
    
    echo "Test completed!"
}

# Function to analyze the minishell code
analyze_code() {
    echo "=== Code Analysis ==="
    echo "Looking at your minishell signal handling code:"
    echo ""
    
    echo "Current signal handling logic:"
    grep -n -A 10 -B 2 "handle_signals" src/utils/signals.c
    echo ""
    
    echo "Key observations:"
    echo "1. g_exec_status controls signal behavior"
    echo "2. When g_exec_status == 0 (interactive mode):"
    echo "   - Writes newline"
    echo "   - Clears readline line" 
    echo "   - Moves to new line"
    echo "   - Redisplays prompt"
    echo ""
    
    echo "3. There's a commented line: //g_exec_status = 3;"
    echo "4. In run_shell.c, when g_exec_status == 3, exit status is set to 130"
    echo ""
    
    echo "Potential issues:"
    echo "- The commented g_exec_status = 3 line might be needed"
    echo "- Debug printf statements should be removed for production"
    echo "- Exit status 130 is correct for SIGINT (128 + 2)"
}

# Main execution
echo "Choose test option:"
echo "1) Automated signal tests"
echo "2) Interactive comparison (recommended)"
echo "3) Code analysis"
echo "4) All tests"
read -p "Enter choice (1-4): " choice

case $choice in
    1)
        test_bash_signal
        test_minishell_signal
        ;;
    2)
        test_interactive_comparison
        ;;
    3)
        analyze_code
        ;;
    4)
        analyze_code
        echo ""
        test_bash_signal
        test_minishell_signal
        echo ""
        test_interactive_comparison
        ;;
    *)
        echo "Invalid choice"
        exit 1
        ;;
esac

# Cleanup
rm -f test_bash_sigint.sh test_minishell_sigint.sh

echo "=== Test Summary ==="
echo "Compare the following between bash and minishell:"
echo "1. Visual behavior when pressing Ctrl+C multiple times"
echo "2. Exit status after Ctrl+C (should be 130 or 0 depending on context)"
echo "3. Prompt redisplay behavior"
echo "4. Response time to signals" 