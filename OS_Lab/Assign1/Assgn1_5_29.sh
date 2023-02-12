> $2
find $1 -name "*.py" -type f | while read file; do
    echo "File Name: $file" >> $2
    echo -e "\n" >> $2
    grep -P "^\s*#" $file >> $2
    grep -P "(?s)('''|\"\"\")(.*?)('''|\"\"\")" $file >> $2
    # - p for perl compatible expression
    echo -e "\n\n\n" >> $2
done