#!/bin/bash -l
#$ -cwd
#$ -j y
#$ -o /dev/null
#$ -q idra
#
# optimal script: launch the simulator
java -jar Simulator.jar  --sce datasets/n050w8/Sc-n050w8.txt --his datasets/n050w8/H0-n050w8-1.txt --weeks datasets/n050w8/WD-n050w8-1.txt datasets/n050w8/WD-n050w8-7.txt datasets/n050w8/WD-n050w8-8.txt datasets/n050w8/WD-n050w8-5.txt datasets/n050w8/WD-n050w8-7.txt datasets/n050w8/WD-n050w8-4.txt datasets/n050w8/WD-n050w8-1.txt datasets/n050w8/WD-n050w8-8.txt --solver ./rosterDemandingEvaluation --runDir ./bin --outDir outfiles/n050w8_1_1-7-8-5-7-4-1-8/seedTestsDemandingEvaluation/5-36-93-3-8-84-72-58 --rand 5 36 93 3 8 84 72 58 --timeout 113 --cus
