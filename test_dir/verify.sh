#!/bin/bash
rm -f verify_output.txt
touch verify_output.txt
while read -r line; do
	# skip comment lines
	case "$line" in \#*) continue ;; esac
	export PRE=
	export AFTER=
	echo [$line]
	IFS='|' read -ra OPTS <<< "$line"
	IFS=' ' read -ra ARGS <<< "${OPTS[3]}"
	export PRE=${OPTS[1]}
	export AFTER=${OPTS[2]}
	eval "$PRE"
	echo $line >> verify_output.txt
	${OPTS[0]} ${ARGS[@]} 2>> verify_output.txt
	eval "$AFTER"
	echo "" >> verify_output.txt 
done < $1
export PRE=
export AFTER=
