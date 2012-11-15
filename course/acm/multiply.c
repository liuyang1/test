int fast_multiply(int x,int y){ 
    return x*y;
}
int slow_multiply(int x,int y){ 
	int i,ret=0;
		for(i=0;i<y;i++)
			ret+=x;
	return ret;
}
int main(){
	int i,j;
	int x,y;
	for(i=0;i<200;i++)
		for(j=0;j<300;j++){
		x=fast_multiply(i,j);
		y=slow_multiply(i,j);
	}   
    return 0;
}

