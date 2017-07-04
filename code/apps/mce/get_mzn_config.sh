#!/bin/bash
printf "\n"

# get stations as arguments
st_http_args="?"
output_file="config.json"

stations_provided=false
# check if another output file is desired
while getopts ":a:o:" option; do
    case "$option" in
        o)
            output_file="$OPTARG"
            ;;

        a)
            stations+=("$OPTARG")
            stations_provided=true
            ;;

        \?)
            echo "!!! error !!! Invalid option. Example usage: -o config.tmp -a ST1 -a ST2" >&2
            exit 64
            ;;

        :)
            echo "!!! error !!! Option -$OPTARG requires an argument." >&2
            exit 65
            ;;
    esac
done
shift $((OPTIND -1))

# make sure at least one station is provided
if test "$stations_provided" = false; then
    echo "provide station arguments, example: -a ST1 -a ST2"
    exit 66
fi

printf "+++ output file +++\n"

echo $output_file

printf "+++ stations +++\n"

for station in "${stations[@]}"; do
    echo " - $station"
done

# create url using station information
n=${#stations[@]}

for ((i=0; i<$n; i++)); do

    stations[$i]="st=${stations[$i]}"

    if [ $i -ne 0 ]; then
        st_http_args+="&"
    fi;

    st_http_args+="${stations[$i]}"

done

# ip_port="devwb_server"
# url="${ip_port}/mcew/rest_mzn/"
# usr="usr"
# pw="pw"
# ---------------------------------------------------------------------------- #
source ${HOME}/.config/manzano/get_mzn_config.conf

url_with_args="${url}${st_http_args}"

printf "+++ url +++\n"
echo "$url_with_args"

curl -H 'Accept: application/json; indent=4' \
    -u "$usr":"$pw" \
    -o "$output_file" \
    "$url_with_args"

printf "\n"
exit 0
