#!/bin/bash

# test environment variables
echo $
echo $? # returnt value of the last command
echo $0 # name of the shell or script you're in (./minishell or bash)
echo $x
echo $USER

# test environment variables expanded
export # lists env vars
export $ # not a valid identifier
export 123 # not a valid identifier
export $? # expands to the return value that is number - not a valid identifier
export $0 # output depends on $0 expansion
export $0 # output depends on $0 expansion
export nothing # nonexistent env - nothing to export
export $x # expands to empty string <=> export
export $USER # expands to the user name