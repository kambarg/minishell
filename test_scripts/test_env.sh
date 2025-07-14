#!/bin/bash

# test environment variables
echo $
echo $?
echo $0
echo $x
echo $USER

# test environment variables expanded
export $
export 123
export $?
export $0
export $x
export $USER