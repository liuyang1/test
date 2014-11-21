import re
import urllib
url="http://www.ustc.edu.cn"
a=urllib.urlopen(url).read()
ip=re.findall
