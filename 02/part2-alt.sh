#!/bin/sh

cat input2.txt |
  sed 's/A X/3/g' |
  sed 's/A Y/4/g' |
  sed 's/A Z/8/g' |
  sed 's/B X/1/g' |
  sed 's/B Y/5/g' |
  sed 's/B Z/9/g' |
  sed 's/C X/2/g' |
  sed 's/C Y/6/g' |
  sed 's/C Z/7/g' > tmp.txt

awk 'BEGIN{sum=0} {sum=sum+$1} END {print sum}' tmp.txt

# you can do a lot with sed + awk  :D 
