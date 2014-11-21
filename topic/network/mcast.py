#! /usr/bin/python
import socket
class McastSocket(socket.socket):
    def __init__(self,local_port,reuse=False):
        socket.socket.__init__(self,socket.AF_INET,socket.SOCK_DGRAM,socket.IPPROTO_UDP)
        if(reuse):
            self.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
            if hasattr(socket,"SO_REUSEPORT"):
                self.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
        self.bind(("",local_port))
    def mcast_add(self,mcast_addr,mcast_iface):
        self.setsockopt(socket.IPPROTO_IP,socket.IP_ADD_MEMBERSHIP,socket.inet_aton(mcast_addr)+socket.inet_aton(mcast_iface))
