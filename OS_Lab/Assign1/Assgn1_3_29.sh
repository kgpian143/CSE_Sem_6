if ! [ -d $2 ]; then
    mkdir $2
fi
file_num=1
for file in $(find $1 -name "*.jsonl");
 do
    echo "$3,$4,$5" >> $2/d_$file_num.csv;
    jq -r "[.$3,.$4,.$5]|@csv" $file >> $2/d_$file_num.csv;
    file_num=$(($file_num+1))
 done