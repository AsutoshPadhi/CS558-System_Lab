# !/bin/bash

printf "***************Welcome to Legendary Dictionary***************\n\n"

# Declare a hashmap datastructure called 'dict'
declare -A dict

# Use file_name as the variable to access the file
file_name=dictionary.txt

# Read the file
if [ -e $file_name ]
then
    # Read the existing dictionary into the associative array 'dict'
    while IFS=':' read word meaning
    do
        dict[$word]=$meaning
    done < $file_name
else
    # If there is no pre-existing file, create one
    touch $file_name
fi

while :
do
    printf "\n-----------------------------------------------------------------------\n"
    # Variable to check if word is present in dictionary
    is_present=0
    
    # Read the word and store it in the variable 'word'
    read -p "Enter the new word : " word

    # Search if the word is present in dictionary; if yes, assign 1 to is_present
    [[ -v dict[$word] ]] && is_present=1
    
    # If the word doesn't exist, read the meaning and store it in dictionary.txt
    if [ $is_present == 0 ] 
    then 
        # Read the meaning
        read -p "Enter the meaning : " meaning
     
        # Insert into the dictionary
        dict[$word]=$meaning
        
        # Display the whole dictionary
        echo "$word:${dict[$word]}">>$file_name
    
        # Sort the file
        sort -o $file_name $file_name
    fi
   
    # If the word is present, display the meaning
    if [ $is_present == 1 ]
    then 
        echo $word : ${dict[$word]}
    fi
    
done