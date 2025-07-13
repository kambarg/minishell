#!/bin/bash

# test quotes

echo $USER
echo "$USER"
echo '$USER'
echo ' "$USER" '
echo ' ' "$USER" ' ' 
echo ' '   ' "$USER" ' ' '
echo ' '  "  ' $USER ' " ' '
echo " " '$USER' " "
echo " " " '$USER' " " "
echo ' '$HOME' '
$ echo ' ' '$HOME' ' '
$ echo " "$HOME" "
$ echo " " "$HOME" " "