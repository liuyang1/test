#! /usr/bin/python
import sys
import string
import psyco
import gc
#my_data=[line.split for line in file('data.txt')]
from psyco.classes import *
psyco.full()

fileName="data.txt"
thresNum=5
thresProb=0.5
Gcnt=0

class Rule:
	def __init__(self,cond=(),rslt=(),prob=0.0,cosn=0.0,lift=1.0):
		self.cond=cond
		self.rslt=rslt
		self.prob=prob
		self.cosn=cosn
		self.lift=lift
class Item:
	def __init__(self,itemlst=[],indexlst=[]):
		self.itemlst=itemlst
		self.indexlst=indexlst

def dispItem(item):
	print "itemlst: ",;print item.itemlst
	print "indexlst:",;print item.indexlst

def dispItemSetLst(itemsetlst):
	for item in itemsetlst:
		print "itemlst: ",;print item.itemlst,
		print "indexlst:",;print len(item.indexlst)
		#print "indexlst:",;print item.indexlst

def generate_one_itemset(filename):
	my_data=[line.split() for line in file(filename)]
	oneSet={}
	i=-1
	for trans in my_data:
		i=i+1
		for item in trans:
			item=int(item)
			if oneSet.has_key(item):
				oneSet[item].append(i)
			else:
				oneSet[item]=list([i,])
	onelst=[]
	for k,v in oneSet.iteritems():
		onelst.append(Item([int(k)],v))
	onelst=select_freq(onelst)
	return onelst

def select_freq(itemlst):
	return filter(lambda item:len(item.indexlst)>=thresNum,itemlst)

def generate_next_cand_itemset(freqLst):
	cand_next=[]
	cnt=0
	for i in range(len(freqLst)):
		item1=freqLst[i]
		gc.collect()
		for j in range(i+1,len(freqLst)):
			cnt=cnt+1
			if cnt%100000==0:
				print '-',
				sys.__stdout__.flush()
			item2=freqLst[j]
			if prefixEqu(item1,item2):
				cand_next.append(merge(item1,item2))
			else:
				break
	print ''
	return cand_next

def prefixEqu(item1,item2):
	return item1.itemlst[:-1]==item2.itemlst[:-1]

def merge(item1,item2):
	if item1.itemlst[-1]<item2.itemlst[-1]:
		itemlst=item1.itemlst+item2.itemlst[-1:]
	else:
		itemlst=item2.itemlst+item1.itemlst[-1:]
	idxlst=set(item1.indexlst)&set(item2.indexlst)
	return Item(itemlst,idxlst)

def loop(oneSet):
	allSet=[oneSet,]
	candSet=oneSet
	print "onesSet length %d" % len(candSet)
	cnt=1
	while 1:
		cnt=cnt+1
		print "loop",;print cnt
		print "generate %d cand itemset..." % cnt
		next=generate_next_cand_itemset(candSet)
		print "select freq set..."
		freqSet=select_freq(next)
		print len(freqSet)
		#dispItemSetLst(freqSet)
		if len(freqSet)==0:
			break
		candSet=freqSet
		allSet.append(freqSet)
		gc.collect()
		if cnt==6:
			sys.exit()
	return allSet

if __name__=="__main__":
	print "begin..."
	print "generate one itemlst"
	onelst=generate_one_itemset(fileName)
	dispItemSetLst(onelst)
	print "loop..."
	loop(onelst)
	print "DONE!"
