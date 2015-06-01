#!/bin/bash -l
#$ -cwd
#$ -j y
#$ -o /dev/null
#$ -q idra
#
# optimal script: launch the simulator
java -jar Simulator.jar  --sce datasets/n120w8/Sc-n120w8.txt --his datasets/n120w8/H0-n120w8-0.txt --weeks datasets/n120w8/WD-n120w8-0.txt datasets/n120w8/WD-n120w8-9.txt datasets/n120w8/WD-n120w8-9.txt datasets/n120w8/WD-n120w8-4.txt datasets/n120w8/WD-n120w8-5.txt datasets/n120w8/WD-n120w8-1.txt datasets/n120w8/WD-n120w8-0.txt datasets/n120w8/WD-n120w8-3.txt --solver ./rosterDemandingEvaluation --runDir ./bin --outDir outfiles/n120w8_0_0-9-9-4-5-1-0-3/seedTestsDemandingEvaluation/38-18-67-17-85-18-41-0 --rand 38 18 67 17 85 18 41 0 --timeout 351 --cus
