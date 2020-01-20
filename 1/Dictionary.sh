# !/bin/sh

printf "***************Welcome to Legendary Dictionary***************\n\n"

# declare a hashmap datastructure called 'dict'
declare -A dict

while :
do
    
    # Read the word and store it in the variable 'word'
    read -p "Enter the new word : " word

    # TODO: Read the meaning if needed

    # Read the meaning
    read -p "Enter the meaning : " meaning
    
    # Insert into the dictionary
    dict[$word]=$meaning

    # Display the whole dictionary
    for key in "${!dict[@]}";
    do
        echo $key : ${dict[$key]};
    done
    
done