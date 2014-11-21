String[] lines;
int index=0;

void setup(){
  size(200,200);
  background(255);
  stroke(255,0,0);
  frameRate(12);
  lines=loadStrings("test.dat");
}
void draw(){
  translate(width/2,height/2);
    if(index<lines.length){
    String[] l=split(lines[index],' ');
    if(l.length==2){
      int x=int(l[0]);
      int y=int(l[1]);
      point(x,y);
    }
    index+=1;
  }
}
