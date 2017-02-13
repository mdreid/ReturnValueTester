#!/bin/bash
# Usage: ./update_apps.sh test_dir/combined_apps.txt
sudo apt-get update > /dev/null
sudo apt-get upgrade > /dev/null

all_packages_present=0
while read -r line; do
    # skip comment lines
    case "$line" in \#*) continue ;; esac

    IFS='|' read -ra OPTS <<< "$line"
    app=${OPTS[0]}
    which $app > /dev/null
    rc=$?
    if [ $rc != 0 ]; then
	all_packages_present=1
    	echo "Installing app "$app
	sudo apt-get install $app > /dev/null
    fi
done < $1
if [ $all_packages_present == 0 ]; then
	echo "All packages present"
fi
