import suds
url = "http://www.ecubicle.com/iptocontry.asmx?wsdl"
client = suds.client.Client(url)
print client
