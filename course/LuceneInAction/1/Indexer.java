import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;

import java.io.File;
import java.io.IOException;
import java.io.FileReader;
import java.util.Date;
public class Indexer{
	public static void main(String[] args)
		throws Exception
	{
		if(args.length!=2){
			throw new Exception("Usage: java " + Indexer.class.getName() + " <index dir> <data dir>");
		}
		File indexDir = new File(args[0]);
		File dataDir = new File(args[1]);

		long start = new Data().getTime();
		int numIndexed = index(indexDir,dataDir);
		long end = new Data.getTime();

		System.out.println("Indexing " + numIndexed + " files took " + (end - start) + " ms");
	}

	public static int index(File indexDir,File dataDir)
		throws IOException
	{
		if(!dataDir.exists()||!dataDir.isDirectory()){
			throw new IOException(dataDir + "does not exist or is not a directory");
		}
		IndexWrite writer = new IndexWriter(indexDir,new StandardAnalyzer(),true);
		writer.setUseCompoundDir(false);

		indexDirectory(writer,dataDir);

		int numIndexed = writer.docCount();
		writer.optimize();
		writer.close();
		return numIndexed;
	}
	private static void indexDirectory(IndexWriter writer,File dir)
		throws IOException
	{
		File[] files = dir.listFiles();
		for(int i=0;i<files.length;i++){
			File f = files[i];
			if(f.isDirectory()){
				indexDirectory(writer,f);// DFS search
			}else{// index all file
				indexFile(writer,f);
			}
		}
	}
	private static void indexFile(IndexWriter writer,File f)
		throws IOException
	{
		if(f.isHidden()||!f.exists() || !f.canRead()){
			return;
		}
		System.out.println("Indexing " +f.getCanonicalPath());
		Document doc = new Document();
		doc.add(Field.Text("contents",new FileReader(f)));
		doc.add(Field.Keyword("filename",f.getCanonicalPath()));
		writer.addDocument(doc);
	}
}
