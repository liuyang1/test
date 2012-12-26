
def addr2text(addr):
	return str(addr[0])+"\r\n"+str(addr[1])+"\r\n"

def text2addr(text):
	addr=text.split()
	return (addr[0],int(addr[1]))
