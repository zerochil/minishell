#!/bin/bash

COMMANDS_FILE="commands.txt"

MY_SHELL="./minishell"

MY_OUTPUT="/tmp/mini_output.txt"
BASH_OUTPUT="/tmp/bash_output.txt"

IGNORE_PATTERN="^(bash:|myshell:)"

line=0
while read -r command; do
	echo "->> Testing: $command"
	echo "->> Line: $line"

	echo "$command" | $MY_SHELL | sed -E "s/$IGNORE_PATTERN//" > $MY_OUTPUT 2>&1
	MY_EXIT=$?

	echo "$command" | bash | sed -E "s/$IGNORE_PATTERN//" > $BASH_OUTPUT 2>&1
	BASH_EXIT=$?

	if ! diff -u $BASH_OUTPUT $MY_OUTPUT > /dev/null; then
		echo "-> Output differs for: $command <-"
		echo "=> Minishell output:"
		cat $MY_OUTPUT
		echo "=> Bash output:"
		cat $BASH_OUTPUT
	fi

	if [ $MY_EXIT -ne $BASH_EXIT ]; then
		echo "-> Exit status differs for: $command <-"
		echo "=> Minishell: $MY_EXIT, => Bash: $BASH_EXIT"
	fi

	let line++
done < "$COMMANDS_FILE"
