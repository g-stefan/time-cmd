@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> clean time-cmd

if exist output\ rmdir /Q /S output
if exist temp\ rmdir /Q /S temp
