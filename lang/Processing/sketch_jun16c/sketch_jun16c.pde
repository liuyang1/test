import processing.opengl.*;

Cube stage;
float bounds=250;
float rotateSpeed=0.0005;
int nball=3;
float pos[][]=new float[nball][3];
float speed[][]=new float[nball][3];
float r[]=new float[nball];
float w[]=new float[nball];
int ustcc[][]=new int[nball][3];
float ratio=0.001*0.01;
float colork=0.8;
void setup() {
  size(640, 480, OPENGL);
  stage=new Cube(bounds, bounds, bounds);
  //float r0=15;
  for (int i=0;i<nball;i++) {
    for (int j=0;j<3;j++) {
      pos[i][j]=random(-bounds/2, bounds/2);
      speed[i][j]=random(-3,3);
    }
    r[i]=random(7.5,15);
    //r[i]=r0;
    w[i]=r[i]*r[i]*r[i]/20;
  }
  ustcc[0][0]=255;ustcc[0][1]=0;ustcc[0][2]=0;
  ustcc[1][0]=0;ustcc[1][1]=255;ustcc[1][2]=0;
  ustcc[2][0]=0;ustcc[2][1]=0;ustcc[2][2]=255;
  /*
  ustcc[0][0]=0;
  ustcc[0][1]=77;
  ustcc[0][2]=149;
  ustcc[1][0]=207;
  ustcc[1][1]=3;
  ustcc[1][2]=4;
  ustcc[2][0]=4;
  ustcc[2][1]=0;
  ustcc[2][2]=1;
  */
  for (int i=3;i<nball;i++) {
    int c=255/(nball)*i;
    for (int j=0;j<3;j++) {
      ustcc[i][j]=c;
    }
  }
  smooth();
}

void draw() {
  // update background
  background(200, 200, 200, 0);
  // light
//  lights();
  
//  float dirY=(mouseY/float(height)-0.5)*2;
//  float dirX=(mouseX/float(width)-0.5)*2;
  directionalLight(204, 204, 204, -1, -1, -1);
  
  // draw cube
  //  fill(200,200,200,0);
  noFill();
  translate(width/2, height/2, 0);
  rotateX(frameCount*rotateSpeed);
  rotateY(frameCount*rotateSpeed);
  rotateZ(frameCount*rotateSpeed);
  stroke(50);
  stage.create();
  //draw balls
  for (int i=0;i<nball;i++) {
    pushMatrix();
    translate(pos[i][0], pos[i][1], pos[i][2]);
    fill(ustcc[i][0], ustcc[i][1], ustcc[i][2]);
    noStroke();
    sphere(r[i]);
    popMatrix();
  }
  // draw the line between balls
  stroke(150);
  for (int i=0;i<nball-1;i++) {
    line(pos[i][0], pos[i][1], pos[i][2], pos[i+1][0], pos[i+1][1], pos[i+1][2]);
  }
  line(pos[nball-1][0], pos[nball-1][1], pos[nball-1][2], pos[0][0], pos[0][1], pos[0][2]);
  // check collison between ball and cube
  for (int i=0;i<nball;i++) {
    for (int j=0;j<3;j++) {
      if (pos[i][j]+r[i]>bounds/2) {
        speed[i][j]*=-1;
        pos[i][j]=bounds/2-r[i];//must add this
      }
      else if (pos[i][j]-r[i]<-1*bounds/2) {
        speed[i][j]*=-1;
        pos[i][j]=-bounds/2+r[i];
      }
    }
  }
  // check collsion between balls
  // elastic collision
  for (int i=0;i<nball;i++) {
    for (int j=i+1;j<nball;j++) {
      float rvd=dist(pos[i][0], pos[i][1], pos[i][2], pos[j][0], pos[j][1], pos[j][2]);
      if (rvd<r[i]+r[j]) {
        float rv[]=new float[3];
        // dist vector
        for(int k=0;k<3;k++){
          rv[k]=(pos[j][k]-pos[i][k]);
        }
        float vr1[]=new float[3];
        float vt1[]=new float[3];
        float vr2[]=new float[3];
        float vt2[]=new float[3];
        float vrd1=0;
        float vrd2=0;
        // dot prod between speed vector and dist vector
        for(int k=0;k<3;k++){
          vrd1+=speed[i][k]*rv[k]/rvd;
          vrd2+=speed[j][k]*rv[k]/rvd;
        }
        // speed vector project to dist vector
        for(int k=0;k<3;k++){
          vr1[k]=vrd1*rv[k]/rvd;
          vr2[k]=vrd2*rv[k]/rvd;
          vt1[k]=speed[i][k]-vr1[k];
          vt2[k]=speed[j][k]-vr2[k];
        }
        //collision
        float vrn1[]=new float[3];
        float vrn2[]=new float[3];
        for(int k=0;k<3;k++){
          vrn1[k]=((w[i]-w[j])*vr1[k]+2*w[j]*vr2[k])/(w[i]+w[j]);
          vrn2[k]=((w[j]-w[i])*vr2[k]+2*w[i]*vr1[k])/(w[i]+w[j]);
        }
        // construct to new speed
        for(int k=0;k<3;k++){
          speed[i][k]=vrn1[k]+vt1[k];
          speed[j][k]=vrn2[k]+vt2[k];
        }
        for (int k=0;k<3;k++) {
         //speed[i][k]*=-1;//reverse orien
         //speed[j][k]*=-1;
         /*
         float tmp=speed[j][k];
         speed[j][k]=speed[i][k];
         speed[i][k]=tmp;//swap speed
         */
         }
         // change color
         float oldc;
         for(int k=0;k<3;k++){
           oldc=ustcc[i][k];
           ustcc[i][k]=int(colork*ustcc[i][k]+(1-colork)*ustcc[j][k]);
           ustcc[j][k]=int(colork*ustcc[j][k]+(1-colork)*oldc);
         }
      }
    }
  }
  // 引力吸引
/*
  for (int i=0;i<nball;i++) {
    for (int j=i+1;j<nball;j++) {
      float d=dist(pos[i][0], pos[i][1], pos[i][2], pos[j][0], pos[j][1], pos[j][2]);
      float fall=ratio*w[i]*w[j]/d/d/d;
      float f[]=new float[3];
      for (int k=0;k<3;k++) {
        f[k]=+1*fall*(pos[j][k]-pos[i][k]);
        speed[i][k]+=f[k];
        speed[j][k]-=f[k];
      }
    }
  }
  */
  // update position
  for (int i=0;i<nball;i++) {
    for (int j=0;j<3;j++) {
      pos[i][j]+=speed[i][j];
    }
  }
}
void mousePressed() {
  // update color and speed
  for (int i=0;i<nball;i++) {
    for (int j=0;j<3;j++) {
      speed[i][j]+=random(-2, 2);
      ustcc[i][j]=0;
    }
    ustcc[i][i]=255;
  }
}

