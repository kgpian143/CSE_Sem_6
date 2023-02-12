> $3
while read -r line;
do
    str="$line"
    n=${#str}
    if [ $n -gt 25 ] || [ $n -lt 5 ]
    then
        echo "NO" >> $3
        continue
    fi
    ch=${str:0:1}
    if [[ "$ch" =~ [^a-zA-Z] || -z "$ch" ]] 
    then 
        echo "NO" >> $3
        continue
    fi


    if [[ "$str" =~ [^a-zA-Z0-9] || -z "$str" ]] 
    then 
        echo "NO" >> $3
        continue
    fi
    
    if ! [[ $str =~ [0-9] ]]
    then 
        echo "NO" >> $3
        continue
    fi
    flag=0
    # tr '[:upper:]' '[:lower:]' <<< "$str"
    while read -r line2;
    do
        if echo "$str" | grep -iq "$line2"; 
        then
            echo "NO" >> $3
            flag=1
            break
        fi
    done <$2
    if [ $flag == 0 ]
    then
        echo "YES" >> $3
    fi

done <$1
