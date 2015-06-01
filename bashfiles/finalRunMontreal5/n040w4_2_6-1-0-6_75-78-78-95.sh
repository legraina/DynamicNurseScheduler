#!/bin/bash -l
#$ -cwd
#$ -j y
#$ -o /dev/null
#$ -q idra
#
# optimal script: launch the simulator
java -jar Simulator.jar  --sce datasets/n040w4/Sc-n040w4.txt --his datasets/n040w4/H0-n040w4-2.txt --weeks datasets/n040w4/WD-n040w4-6.txt datasets/n040w4/WD-n040w4-1.txt datasets/n040w4/WD-n040w4-0.txt datasets/n040w4/WD-n040w4-6.txt --solver ./rosterDemandingEvaluation --runDir ./bin --outDir outfiles/Competition/n040w4_2_6-1-0-6/FinalRun/75-78-78-95/ --rand 75 78 78 95 --timeout 72 --cus
