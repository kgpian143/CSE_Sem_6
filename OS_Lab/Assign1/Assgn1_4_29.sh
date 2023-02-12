> $2
substr="kharagpur"
while read -r line 
do 
   str="$line"
   if echo "$str" | grep -iq "$substr"; then
    echo $str | sed 's/\([A-Za-z]\)\([^A-Za-z]*\)\([A-Za-z]\)/\u\1\2\l\3/g'  >> $2
   else
    echo $str >> $2
   fi
done <$1

