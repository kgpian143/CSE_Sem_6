#!/bin/bash
# # inp_file=$1
# # Extract the "created_at" and "id_str" objects using jq
jq 'unique_by(.created_at, .id_str)' inp.jsonl
data=$(jq '.created_at, .id_str' inp.jsonl)

# Write the data to a CSV file
echo $data | tr ' ' ',' >> output.csv
# #!/bin/bash

# Extract the key-value pairs using jq
# jq -r 'try .created_at,.id_str catch "Error: Invalid data"' inp.json
# jq -r '.created_at,.id_str' inp.jsonl | jq -s 'unique' | tr '\n' ',' >> output.csv
