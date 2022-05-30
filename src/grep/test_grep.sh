#!/bin/bash

SUCCESS=0
FAIL=0
DIFF_OUTPUT=""
REGEX="--regexp=\"[0-9]\+\""
PATTERN="21"
FILE_NAME="./s21_grep.c"

for flag_1 in '' -i -v -c -l -n $REGEX
do 
    for flag_2 in '' -i -v -c -l -n $REGEX
    do
        for flag_3 in '' -i -v -c -l -n $REGEX
        do
            for flag_4 in '' -i -v -c -l -n $REGEX
            do
                if [[ $flag_1 != $flag_2 ]] && [[ $flag_1 != $flag_3 ]] && 
                   [[ $flag_1 != $flag_4 ]] && [[ $flag_2 != $flag_3 ]] && 
                   [[ $flag_2 != $flag_4 ]] && [[ $flag_3 != $flag_4 ]]
                then
                    if [[ "$flag_1" == "$REGEX" ]] || [[ "$flag_3" == "$REGEX" ]] || 
                       [[ "$flag_2" == "$REGEX" ]] || [[ "$flag_4" == "$REGEX" ]]
                    then
                        if [[ "$flag_1" != "-l" ]] && [[ "$flag_3" != "-l" ]] && 
                           [[ "$flag_2" != "-l" ]] && [[ "$flag_4" != "-l" ]]
                        then
                            OPTIONS="$flag_1 $flag_2 $flag_3 $flag_4 $FILE_NAME"
                            echo "sh: $OPTIONS"
                        else
                            continue;
                        fi
                    else
                        OPTIONS="$flag_1 $flag_2 $flag_3 $flag_4 $PATTERN $FILE_NAME"
                        echo "sh: $OPTIONS"
                    fi

                    ./s21_grep $OPTIONS > s21_temp.txt
                    grep $OPTIONS > temp.txt

                    DIFF_OUTPUT="$(diff -s s21_temp.txt temp.txt)"
                    if [ "$DIFF_OUTPUT" == "Files s21_temp.txt and temp.txt are identical" ]
                        then
                            (( SUCCESS++ ))
                        else
                            (( FAIL++ ))
                            echo "ERROR"
                            echo "$(cmp s21_temp.txt temp.txt)"
                            echo "$(diff -s s21_temp.txt temp.txt)"
                    fi

                    rm s21_temp.txt temp.txt
                fi
            done
        done
    done
done


echo "SUCCESS: $SUCCESS"
echo "FAIL: $FAIL"