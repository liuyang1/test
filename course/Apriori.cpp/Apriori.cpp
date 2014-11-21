#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
class Item{
	public:
		vector<int>	mNumLst;
		vector<int> mIdxLst;
		
		Item(vector<int> numlst,vector<int> idxlst)
			:mNumLst(numlst),mIdxLst(idxlst){
			}
		Item(){}
		~Item(){}
};

int mergeItem(Item src1,Item src2,Item &dst){
	return 1;
}
int createOneItemLst(vector<Item> &itemlst1){
	return 1;
}
int selectItemLst(vector<Item> &itemlst){
	return 1;
}

int createNextItemLst(vector<Item> itemlst,vector<Item> &next){
	return 1;
}
int loop(vector<Item> itemlst1){
	vector<Item> itemlst=itemlst1;
	vector<Item> next;
	vector<vector<Item> >	itemlstAll;
	while(1){
		createNextItemLst(itemlst,next);
		itemlstAll.push_back(next);
		if(next.size()==0)
			break;
	}
	return 1;
}
int loadFile(char* filename,vector<vector<int>> &data){
	FILE *fp=fopen(filename,'r');
	
	close(fp);
	return 1;
}
int main(){
	return 1;
}
