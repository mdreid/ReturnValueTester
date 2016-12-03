CALLS=()
for w in ../wrappers/*.so
do
	call=$(basename "$w")
	call="${call%.*}"
	call="${call%_*}"
	CALLS+=($call)
done

while read -r line; do
#line="base64|rm -f tmp.txt|echo 'testing base64' >> tmp.txt|tmp.txt"
	#echo $line
	export PRE=
	export AFTER=
	for prob in {0..50..10}; do
		percent=$(echo print $prob / 100. | python)
		echo [$line $percent]
		IFS='|' read -ra OPTS <<< "$line"
		IFS=' ' read -ra ARGS <<< "${OPTS[3]}"
		for call in "${CALLS[@]}"; do
			export PRE=${OPTS[1]}
			export AFTER=${OPTS[2]}
			#echo $PRE
			.././log_gen 20 $call ${OPTS[0]} ${ARGS[@]} $percent
			#echo $AFTER
			#echo $PRE
		done
	done
done < $1
export PRE=
export AFTER=
