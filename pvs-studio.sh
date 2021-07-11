#!/bin/bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=On .
pvs-studio-analyzer analyze
plog-converter -t html PVS-Studio.log -o analyze.html
google-chrome-stable ./analyze.html