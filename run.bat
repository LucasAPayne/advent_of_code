@echo off

set year=%1
set day=%2

cd src/%year%/%day%
start ../../../build/%year%/%day%/aoc_%year%_%day%.exe
cd ../../..
