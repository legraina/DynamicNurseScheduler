#!/bin/bash -l
#$ -cwd
#$ -j y
#$ -o /dev/null
#$ -q idra
#
# optimal script: launch the simulator
java -jar Simulator.jar  --sce datasets/n060w4/Sc-n060w4.txt --his datasets/n060w4/H0-n060w4-1.txt --weeks datasets/n060w4/WD-n060w4-6.txt datasets/n060w4/WD-n060w4-1.txt datasets/n060w4/WD-n060w4-1.txt datasets/n060w4/WD-n060w4-5.txt --solver ./rosterDemandingEvaluation --runDir ./bin --outDir outfiles/Competition/n060w4_1_6-1-1-5/FinalRunAL/6-45-56-39/ --rand 6 45 56 39 --timeout 134 --cus
