#!/bin/bash
pwd
rm *.trace
LD_PRELOAD=glxtrace.so ./glakkegame
qapitrace *.trace
