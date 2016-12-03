while read -r line; do
	IFS='|' read -ra OPTS <<< "$line"
	IFS=' ' read -ra ARGS <<< "${OPTS[3]}"
	eval ${OPTS[1]}
	touch tmp.txt
	ltrace -cf -o tmp.txt ${OPTS[0]} ${ARGS[@]}
	cat tmp.txt >> traces/${OPTS[0]}.txt
	eval ${OPTS[2]}
done < "utils.txt"
