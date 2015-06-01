#!/bin/bash -l
#$ -cwd
#$ -j y
#$ -o /dev/null
#$ -q idra
#
# optimal script: launch the simulator
java -jar Simulator.jar  --sce datasets/n040w8/Sc-n040w8.txt --his datasets/n040w8/H0-n040w8-2.txt --weeks datasets/n040w8/WD-n040w8-5.txt datasets/n040w8/WD-n040w8-0.txt datasets/n040w8/WD-n040w8-4.txt datasets/n040w8/WD-n040w8-8.txt datasets/n040w8/WD-n040w8-7.txt datasets/n040w8/WD-n040w8-1.txt datasets/n040w8/WD-n040w8-7.txt datasets/n040w8/WD-n040w8-2.txt --solver ./rosterDemandingEvaluation --runDir ./bin --outDir outfiles/n040w8_2_5-0-4-8-7-1-7-2/seedTestsDemandingEvaluation/34-77-23-62-20-35-15-12 --rand 34 77 23 62 20 35 15 12 --timeout 79 --cus
