#!/bin/bash
make
echo $(($1+$2)) $(($1-$2)) > answer.txt
echo $1 $2 | ./main $1 $2 > output.txt
diff answer.txt output.txt > result.txt
cat result.txt
make clean
