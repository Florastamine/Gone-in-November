@echo off
title 
cls

echo Be patient...

cloc ..\         >> cloc.log
cloc --show-os   >> cloc.log
echo %time%      >> cloc.log
