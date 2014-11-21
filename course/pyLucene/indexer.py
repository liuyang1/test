#! /usr/bin/env python
import os,sys,glob
import lucene# need add libjvm.so to ld.conf
from lucene import FSDirectory,System,File,Document,Field,StandardAnalyzer,IndexWriter,VERSION

"""
Example fo Indexing with PyLucene
"""

def Indexer(docdir,indir):
	lucene.initVM()
	DIRTOINDEX   = docdir
	INDEXDIR     = indir
	indexdir     = FSDirectory(File(INDEXDIR))
	analyzer     = StandardAnalyzer(VERSION.LUCENE_30)
	index_writer = IndexWriter(indexdir,analyzer,True,IndexWriter.MaxFieldLength(512))
	for tfile in glob.glob(os.path.join(DIRTOINDEX,'*.txt')):
		print "Indexing ",tfile
		document=Document()
		content = open(tfile,'r').read()
		document.add(Field("text",content,Field.Store.YES,Field.Index.ANALYZED))
		index_writer.addDocument(document)
		print "Done"
	index_writer.optimize()
	print index_writer.numDocs()
	index_writer.close()

if __name__=="__main__":
	Indexer('.','.')
