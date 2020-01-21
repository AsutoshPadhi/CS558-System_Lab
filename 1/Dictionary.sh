# !/bin/bash

printf "***************Welcome to Legendary Dictionary***************\n\n"

# declare a hashmap datastructure called 'dict'
declare -A dict
while IFS='=' read word meaning
   do
    dict[$word]=$meaning
     echo "$word : ${dict[$word]}"
  done < file.txt
    
while :
do
     a=0
    # Read the word and store it in the variable 'word'
    read -p "Enter the new word : " word

    # TODO: Read the meaning if needed
      # [[ -v dict[$word] ]] && a=1 echo $word : ${dict[$word]};
       [[ -v dict[$word] ]] && a=1 
if [ $a == 0 ] 
then 
    # Read the meaning
    read -p "Enter the meaning : " meaning
     
    # Insert into the dictionary
    dict[$word]=$meaning
    # Display the whole dictionary
    echo "$word=${dict[$word]}">>file.txt
    : '
    for key in "${!dict[@]}";
    do
        echo $key : ${dict[$key]};
    done
    '
    sort file.txt>copy.txt
    cp copy.txt file.txt
     
    # echo >file.txt
#      for key in "${!dict[@]}"; do
#   printf '%s:%s\n' "$key" "${dict[$key]}"
#      done | sort -t : -k 2n      
fi
   if [ $a == 1 ] 
then 
    echo $word : ${dict[$word]}
    fi
    # for key in "${!dict[@]}";
    # do
    #     echo "$key : ${dict[$key]}">>file.txt
    # done
 done