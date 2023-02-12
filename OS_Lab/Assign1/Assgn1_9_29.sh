> $2

cat "$1" | awk -v var=2 '{print $var}' \
    | sort -f | uniq -ic | sort -nr -k1,1 \
        | awk '{print $2,$1}' >> $2

echo >> $2
num=0
total=0
cat "$1" | awk '{ print $1 }' $1 | egrep -o "\b[[:alpha:]]+\b" |
awk '{ count[$0]++ }
END {
    for(ind in count)
    { 
        total=total+1;
        if(count[ind] >= 2)
        {
            print ind >> "'$2'"
            num=num+1;
        }        
    }

    print total-num >> "'$2'"
}'
