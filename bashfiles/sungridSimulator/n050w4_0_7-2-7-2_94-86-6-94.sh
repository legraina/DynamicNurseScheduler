#!/bin/bash -l
#$ -cwd
#$ -j y
#$ -o /dev/null
#$ -q idra
#
# optimal script: launch the simulator
java -jar Simulator.jar  --sce datasets/n050w4/Sc-n050w4.txt --his datasets/n050w4/H0-n050w4-0.txt --weeks datasets/n050w4/WD-n050w4-7.txt datasets/n050w4/WD-n050w4-2.txt datasets/n050w4/WD-n050w4-7.txt datasets/n050w4/WD-n050w4-2.txt --solver ./rosterDemandingEvaluation --runDir ./bin --outDir outfiles/n050w4_0_7-2-7-2/seedTestsDemandingEvaluation/94-86-6-94 --rand 94 86 6 94 --timeout 113 --cus
