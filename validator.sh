#!/bin/bash
# validator script: launch the validator

#instance = n005w4: (1 2 3 3)
#instance = n012w8: (3 5 0 2 0 4 5 2)
dataFile="datasets/"
instance="n030w4"
#"${dataFile}${instance}/Solution_H_0-WD_1-2-3-3/"
solutionDir="outfiles/"
weeksValue=(6 2 9 1)
weeksName="6291"
history="${dataFile}${instance}/H0-${instance}-1.txt"
scenario="${dataFile}${instance}/Sc-${instance}.txt"

demand0="WD-${instance}-"
solution0="${instance}-${weeksName}-sol-week"
weeks=""
sols=""
i=0

for var in ${weeksValue[*]}
do
demand[$i]="${dataFile}${instance}/${demand0}${var}.txt"
weeks="${weeks} ${demand[$i]}"

solution[$i]="${solutionDir}${solution0}${i}.txt"
sols="${sols} ${solution[$i]}"

((i++))
done

java -jar validator.jar --his $history --sce $scenario --weeks $weeks --sols $sols

exit 0;
