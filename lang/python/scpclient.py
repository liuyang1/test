#! /usr/bin/env python
# for test scpclient which is wrapped of paramiko.
import paramiko
import scpclient
client=paramiko.SSHClient()
client.load_system_host_keys()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
print "prepare to connect sshd"
client.connect("localhost",username="liuy",password="liuy@lyAG")
#client.connect("::1")

print "connect ok"
with closing(Write(ssh.get_transport(),'.')) as scp:
	print "prepare to send file"
	scp.send_file('testfilesrc','testfiledst')
	print "send file ok"
print "close file"
