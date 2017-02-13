while read -r line; do
        # skip comment lines
        case "$line" in \#*) continue ;; esac

	export PRE=
	export AFTER=
	for prob in {0..100..10}; do
		percent=$(echo print $prob / 100. | python)
		export PROB=$percent
		echo [$line $percent]
		IFS='|' read -ra OPTS <<< "$line"
	    IFS=' ' read -ra ARGS <<< "${OPTS[3]}"
	    export PRE=${OPTS[1]}
        export AFTER=${OPTS[2]}
         .././log_gen 20 "combined" ${OPTS[0]} ${ARGS[@]}
	done
done < $1
