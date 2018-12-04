#!/bin/bash
FILES=./tests/*.in

for f in $FILES
do

out=${f::-2}
out="${out}out"
echo $f
./prze <$f >pr.out
diff -bq $out pr.out
done






