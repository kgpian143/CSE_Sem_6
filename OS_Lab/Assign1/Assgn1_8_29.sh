file="main.csv"
if [ ! -f "$file" ]; then
    touch "$file"
    # echo "Date (dd-mm-yy) | Category | Amount | Name"
fi
#!/bin/bash
last_args=( "${@: -4:4}" )
echo "$(IFS=, ; echo "${last_args[*]}")" >> main.csv
while getopts ":c:n:s:h:" option; do
  case "${option}" in
    c)
      cat="${OPTARG}"
      # echo $cat
      awk -F "," '{if ($2 == "'"$cat"'" ) {sum+=$3}} END {print "Total amount spend on " "'"$cat"': " sum}' main.csv
      ;;
    n)
      name1="${OPTARG}"
      awk -F "," '{if ($4 == "'"$name1"'") {sum+=$3}} END {print "Total amount paid by " "'"$name1"': " sum}' main.csv
      ;;
    s)
      col="${OPTARG}"
      if [ $col == "Date" ]
      then
         sort -t "," -k 1 -o main.csv main.csv
      elif [ $col == "Category" ]
      then
         sort -t "," -k 2 -o main.csv main.csv
      elif [ $col == "Name" ] 
      then
         sort -t "," -k 4 -o main.csv main.csv
      else
         sort -t "," -k3n -o main.csv main.csv
      fi
      ;;
    h)
      echo -e "\n"
      echo "SYNOPSIS: file_name.sh [OPTION]...[type]... [Date] [Category] [Amout] [Name]"
      echo "Inserts the [Date],[Category],[Amout],[Name] into the csv"
      echo "Options:"
      echo "-c, ‘Category’: accepts a category and prints the amount of money spend in that
category"
      echo "-n, ‘name’ : accepts a name and print the amount spent by that person"
      echo "-s, ‘column’: sort the csv by column name"
      ;;
    *)
      echo "Invalid option: -${OPTARG}" 
      exit 1
      ;;
  esac
done