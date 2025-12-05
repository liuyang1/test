#!/usr/bin/env bash

/opt/homebrew/bin/timew | awk '/Total/{print "🔥 " $NF; found=1} END{ if (!found) print "💤"}'
