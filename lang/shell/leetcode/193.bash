re1='^[[:digit:]]{3}-[[:digit:]]{3}-[[:digit:]]{4}$'
re2='^\([[:digit:]]{3}\) [[:digit:]]{3}-[[:digit:]]{4}$'
grep -E "$re1|$re2" file.txt
