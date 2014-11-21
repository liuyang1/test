import sys
fn = sys.argv[1]
fp = open(fn)
fo = open(fn+".flv","wb")
data = fp.read()
offset = int(sys.argv[2])
if len(sys.argv)==3:
    fo.write(data[offset:])
else:
    l = int(sys.argsv[3])
    fo.write(data[offset:offset+l])
fo.close()
fp.close()
