#! /usr/bin/env python

import netsnmp
oid=netsnmp.Varbind('sysDescr')
res=netsnmp.snmpwalk(oid,Version=2,DestHost="localhost",Community="public")
