#ifndef _RGBPLOT_H
#define _RGBPLOT_H
#include"Drawer.h"
#include"RGBTransform.h"
#include<string>
#include<vector>
#include<map>
#include<glut.h>
#include<complex>

const int out_min_deg = 0;	//�ŏ��p�x
const int out_max_deg = 180;	//�ő�p�x
const int out_d_deg   = 1;		//�p�x�Ԋu
const int out_num	  = (out_max_deg-out_min_deg) / out_d_deg;
const int in_min_deg  = -90;	
const int in_max_deg  = 90;
const int in_d_deg    = 10;
const int in_num	  = (in_max_deg-in_min_deg) / in_d_deg;
const int cellNum	  = (in_num+1)*(out_num+1);
const double offset = 0.2;

bool between(int mi, int ma, int a){
	return  (a>=mi && a<=ma);
}

class RGBPlot:public Drawer{
RGBTransform *rgb;
MyColor *ColorMap;
double max_sum;
string Dir;
public:
	RGBPlot(){
		rgb = new RGBTransform();
		ColorMap = new MyColor[cellNum];

		for(int i=0; i<cellNum; i++)
			ColorMap[i] = MyColor(0,0,0);

		max_sum=1;

		setDirTM();
		ifstream fp(getDir() + "WaveAngleStrength.txt" );
		while(fp){
			double tmp;
			complex<double> tmp1;
			fp >> tmp1 >> tmp;
			max_sum = max(max_sum, tmp);	//�ő�̑��a�Ő��K��
		}

		/*
		setDirTE();
		fp.open(getDir() + "WaveAngleStrength.txt" );
		while(fp){
			double tmp;
			complex<double> tmp1;
			fp >> tmp1 >> tmp;
			max_sum = max(max_sum, tmp);	//�ő�̑��a�Ő��K��
		}
		
		for(int j=-90; j <= 0; j+=10)
			for(int i=380; i<=700; i+=5)
				getPlot(i,j);
		
		setDirTM();
		*/
		//max_sum = 1;
		for(int j=0; j < 90; j+=10){
			for(int i=380; i<700; i+=5){
				//if((i>=600 && i<=700)) continue;
				getPlot(i,j);
			}
		}

	}
	
	string getDir(){
		return Dir;
	}

	void setDirTE(){
		Dir = "../../../DataSet/TE/Morpho(1,1.56)M=8/60nm(nonShelf)(10nm,200cell)/NTFF/";	//20[nm]�F��, 50[nm]:��
		//Dir = "../../../DataSet/TE/Morpho/90nm(nonShelf)(10nm,200cell)/NTFF/";
	}

	void setDirTM(){
		Dir = "../../../DataSet/ShelfModel/d=235M=9/";
		//Dir = "../../../DataSet/TM/Morpho(1.56,1)M=8/110nm(nonShelf)(10nm,200cell)/NTFF/";	//20[nm]�F��, 50[nm]:��
		//Dir = "../../../DataSet/TM/Morpho/90nm(nonShelf)(10nm,200cell)/NTFF/";
	}
	void saveColorData(){
		ofstream ofp("Colordata.txt");
		for(int i=0; i< cellNum ;i++)
			ofp << ColorMap[i].r << "  "<< ColorMap[i].g << "  " <<ColorMap[i].b << endl;
	}

	void getPlot(int lam, int in_deg){
		string name   = to_s(in_deg) + "deg" + to_s(lam) + "nm.txt"; 
		//string name = "(WL=" + to_s(lam) + "nm,AOI=" + to_s(in_deg) + "deg).txt"; 
		ifstream fp(getDir()+name);	//�t�@�C�����J��
		if(!fp)	cout <<  "file error"  << name << endl;

		if(in_deg < in_min_deg) return;
		int in = (in_deg - in_min_deg)/in_d_deg;
		int out_deg=0;	//�U���g�̊p�x
		double ref;		//���˗�

		while(fp){
			fp >> ref;
			ref = ref / max_sum;
			if( (out_deg < out_max_deg) && (out_deg >= out_min_deg)){
				double out = (out_deg - out_min_deg)/out_d_deg;
				Color(in,out) = Color(in,out) + rgb->CIE_RGB(rgb->getXYZ(lam, 3*ref));
			}
			out_deg++;
		}
	}

	
	void draw_axis(){
		double dh = 1.0-offset;
		double wid = 2.0*dh/in_num;
		double hei = 2.0*dh/out_num;
		glColor3d(0.0, 0.0, 0.0);

		drawBitmapString(GLUT_BITMAP_HELVETICA_12, "-90", -dh-0.15*offset , -dh-0.4*offset);
		drawBitmapString(GLUT_BITMAP_HELVETICA_12, " 90",  dh-0.15*offset , -dh-0.4*offset);
		drawBitmapString(GLUT_BITMAP_HELVETICA_12, "theta(deg)", -0.1, -dh -0.1);
		/*
		for(int i=out_min_deg; i<=out_max_deg; i+= 30){
			double y = (i - out_min_deg)/out_d_deg*hei - dh;
			drawBitmapString(GLUT_BITMAP_HELVETICA_12, to_s(i-270), -1.0+0.5*offset, y);
		}*/
		drawBitmapString(GLUT_BITMAP_HELVETICA_12, "-90", -dh - 0.5*offset,-dh-0.15*offset);
		drawBitmapString(GLUT_BITMAP_HELVETICA_12, " 90", -dh - 0.5*offset, dh-0.15*offset);
		drawBitmapString(GLUT_BITMAP_HELVETICA_12, "phi (deg)"  , -dh - 0.15, -0.01);

		glColor3d(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		for(int i=-1; i <= 1; i++){
			glVertex2d(-dh, 0.5*i*dh);
			glVertex2d( dh, 0.5*i*dh);
			glVertex2d(0.5*i*dh, -dh);
			glVertex2d(0.5*i*dh,  dh);
		}
		glEnd();
	}

	void drawCell(int i, int j){
		double dh = 1.0-offset;
		double wid = 2.0*dh/in_num;
		double hei = 2.0*dh/out_num;

		double r = 1.0/in_d_deg;
		for(double k=0.0; k<in_d_deg; k+=1){
			double x1 = (i + r*k)*wid - dh;
			double y1 =  j*hei - dh;
			double x2 = x1 + r*wid;
			double y2 = y1 + hei;
			double d = r*(k+0.5);
			MyColor c1 = (1.0 - d) * Color(i,j  ) + d * Color(i+1,j  );
			MyColor c2 = (1.0 - d) * Color(i,j+1) + d * Color(i+1,j+1);
			MyColor c = 0.5*(c1+c2);
			glColor3d(c.r, c.g, c.b);
			glRectd(x1, y1, x2, y2);
			
			//�_�Ώ̂Ȃ̂�,���Α��ɂ��\��
			x1 = (in_num - i - r*k)*wid - dh;
			y1 = (out_num -j)*hei - dh;
			x2 = x1 - r*wid;
			y2 = y1 - hei;
			glRectd(x1, y1, x2, y2);
			
		}
	}

	void drawSample(){
		int lambda = (700 - 380)/5.0; 
		double wid = (2.0-1.2*offset)/lambda;
		double hei = (2.0-1.2*offset)/out_num;
		double dh = 1.0-offset;
		for(int i=0; i<=lambda;i++){
			MyColor c1 = rgb->CIE_RGB(rgb->getXYZ(5*i+380,     0.2));
			MyColor c2 = rgb->CIE_RGB(rgb->getXYZ(5*(i+1)+380, 0.2));
			double r = 1.0/5.0;
			for(int k=0; k<5;k++){
				double x1 = (i+k*r)*wid - dh;
				double x2 = x1 + r;
				MyColor c = r*c2 + (1.0 - r)*c1;
				glColor3d(c.r, c.g, c.b);
				glRectd(x1, -1+dh, x2, 1.0);
			}
		}
	}

	void draw(){
		
		for(int i=in_num/2; i<in_num; i++)
			for(int j=0; j<out_num; j++)
				drawCell(i,j);
		draw_axis();
		
		//drawSample();
	}

	MyColor& Color(const int &i, const int &j){
		int a = (out_num+1)*i + j;
		return ColorMap[a];
	}

};
#endif