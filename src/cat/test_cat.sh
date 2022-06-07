#!/bin/bash

SUCCESS=0
FAIL=0
FILE_NAME="test.txt"
DIFF_OUTPUT=""

for flag_1 in '' -b -e -n -s -t  #--number-nonblank --number --squeeze-blank
do
    for flag_2 in '' -b -e -n -s -t # --number-nonblank --number --squeeze-blank
    do
        for flag_3 in '' -b -e -n -s -t # --number-nonblank --number --squeeze-blank
        do
            for flag_4 in '' -b -e -n -s -t # --number-nonblank --number --squeeze-blank
            do
                if [[ $flag_1 != $flag_2 ]] && [[ $flag_1 != $flag_3 ]] && 
                  [[ $flag_1 != $flag_4 ]] && [[ $flag_2 != $flag_3 ]] && 
                  [[ $flag_2 != $flag_4 ]] && [[ $flag_3 != $flag_4 ]]
                then
                    OPTIONS="$flag_1 $flag_2 $flag_3 $flag_4 $FILE_NAME"
                    echo "sh: $OPTIONS"

                    ./s21_cat $OPTIONS > s21_temp.txt
                    cat $OPTIONS > temp.txt

                    DIFF_OUTPUT="$(diff -s s21_temp.txt temp.txt)"
                    if [ "$DIFF_OUTPUT" == "Files s21_temp.txt and temp.txt are identical" ]
                        then
                            (( SUCCESS++ ))
                        else
                            (( FAIL++ ))
                            echo "$(cmp s21_temp.txt temp.txt)"
                    fi

                    rm s21_temp.txt temp.txt
                fi
            done
        done
    done
done

echo "SUCCESS: $SUCCESS"
echo "FAIL: $FAIL"
