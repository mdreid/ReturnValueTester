sudo apt-get update
sudo apt-get upgrade

while read -r line; do
    # skip comment lines
    case "$line" in \#*) continue ;; esac

    IFS='|' read -ra OPTS <<< "$line"
    app=${OPTS[0]}
    exists=which app
    # this line adapted from Seb answer on http://stackoverflow.com/questions/1298066/check-if-a-package-is-installed-and-then-install-it-if-its-not
    if [ $(dpkg-query -W -f='${Status}' nano 2>/dev/null | grep -c "ok installed") -eq 0 ];
    then
        echo $app
        sudo apt-get install $app;
    fi
done < $1
