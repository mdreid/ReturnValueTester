CALLS=()
for w in ../wrappers/*.so
do
	call=$(basename "$w")
	call="${call%.*}"
	call="${call%_*}"
	CALLS+=($call)
done

while read -r line; do
	export PRE=
	export AFTER=
	for prob in {0..50..10}; do
		percent=$(echo print $prob / 100. | python)
		export PROB=$percent
		echo [$line $percent]
		IFS='|' read -ra OPTS <<< "$line"
		IFS=' ' read -ra ARGS <<< "${OPTS[3]}"
		for call in "${CALLS[@]}"; do
			export PRE=${OPTS[1]}
			export AFTER=${OPTS[2]}
			.././log_gen 20 $call ${OPTS[0]} ${ARGS[@]}
		done
	done
done < $1
export PRE=
export AFTER=
