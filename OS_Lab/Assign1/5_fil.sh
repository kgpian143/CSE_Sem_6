dir="data1c";
IFS=$'\n';
s=" ";
ln="arr=(\$(find${s}${dir}/${s}-type${s}f|sed${s}-En${s}'s|.*/[^/]+\.([^/.]+)\$|\1|p'|sort${s}-u))";
eval $ln;
brr=(${arr[@]/#/$*.});
brr+=("");arr+=("Nil");
for((i=0;i<${#arr[@]};i++));
do mkdir -p 1cout/${arr[i]};ln="find${s}${dir}/${s}-type${s}f${s}-name${s}\"*${brr[i]}\"${s}-exec${s}mv${s}--target-directory=1cout/${arr[i]}${s}{}${s}+";eval $ln;done;ln="rm${s}-rf${s}${dir};mv${s}1cout${s}${dir}";eval $ln;