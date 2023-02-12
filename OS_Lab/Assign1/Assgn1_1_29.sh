file=$1
prev=1
while read -r next;
do
    num_string="$next"
    reverse_string="$(echo "$num_string" | rev)"
    reverse_num="$(echo "$reverse_string" | bc)"
    num1=$prev 
    next=$reverse_num
    while [ $prev -ne $next ];do
    if [ $prev -gt $next ];then
    prev=`expr $prev - $next`
    else
    next=`expr $next - $prev`
    fi
    done
    gcd=$prev
    prev=$((num1 * reverse_num/gcd))
done <$file 
echo $prev
