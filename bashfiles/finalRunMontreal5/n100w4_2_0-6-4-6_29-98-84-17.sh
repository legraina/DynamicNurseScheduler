#!/bin/bash -l
#$ -cwd
#$ -j y
#$ -o /dev/null
#$ -q idra
#
# optimal script: launch the simulator
java -jar Simulator.jar  --sce datasets/n100w4/Sc-n100w4.txt --his datasets/n100w4/H0-n100w4-2.txt --weeks datasets/n100w4/WD-n100w4-0.txt datasets/n100w4/WD-n100w4-6.txt datasets/n100w4/WD-n100w4-4.txt datasets/n100w4/WD-n100w4-6.txt --solver ./rosterDemandingEvaluation --runDir ./bin --outDir outfiles/Competition/n100w4_2_0-6-4-6/FinalRun/29-98-84-17/ --rand 29 98 84 17 --timeout 258 --cus
