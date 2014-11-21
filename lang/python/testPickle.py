#import pickle
import cPickle as pickle

obj={"a":1,"b":2}
pickle.dump(obj,open("tmp.txt","w"))

obj2=pickle.load(open("tmp.txt","r"))

print obj2
