#!/bin/bash -l
#$ -cwd
#$ -j y
#$ -o /dev/null
#$ -q idra
#
# optimal script: launch the simulator
java -jar Simulator.jar  --sce datasets/n120w8/Sc-n120w8.txt --his datasets/n120w8/H0-n120w8-1.txt --weeks datasets/n120w8/WD-n120w8-7.txt datasets/n120w8/WD-n120w8-2.txt datasets/n120w8/WD-n120w8-6.txt datasets/n120w8/WD-n120w8-4.txt datasets/n120w8/WD-n120w8-5.txt datasets/n120w8/WD-n120w8-2.txt datasets/n120w8/WD-n120w8-0.txt datasets/n120w8/WD-n120w8-2.txt --solver ./rosterDemandingEvaluation --runDir ./bin --outDir outfiles/n120w8_1_7-2-6-4-5-2-0-2/seedTestsDemandingEvaluation/48-96-65-3-61-91-38-48 --rand 48 96 65 3 61 91 38 48 --timeout 351 --cus
