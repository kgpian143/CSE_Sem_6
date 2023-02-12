if [ -d $2 ]; then
   for ((i=1; i<=26; i++));
    do
      > $2/"my_file_$i.txt"
    done 
else
    mkdir $2
    for ((i=1; i<=26; i++)); 
    do
        touch $2/"my_file_$i.txt"
    done
fi

for file in $1/*.txt; do
    while read -r line; do
        str="$line"
        ch=${str:0:1}
        lower_ch=${ch,,}
        int_value=$(printf "%d" "'$lower_ch")

        int_value=$((int_value-96))
        echo "$line" >> $2/my_file_$int_value.txt
    done < "$file"
done
for ((i=1; i<=26; i++)); 
 do
    sort $2/my_file_$i.txt -o $2/my_file_$i.txt
 done