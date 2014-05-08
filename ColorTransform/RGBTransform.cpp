
#include "RGBTransform.h"
#include <glut.h>

Vec3 operator+(const Vec3 &a, const Vec3 &b){
	Vec3 v = Vec3(a.x+b.x, a.y+b.y, a.z+b.z); 
	return v;
};

Vec3 operator*(const double &c, const Vec3 &a){
	Vec3 v = Vec3(c*a.x, c*a.y, c*a.z);
	return v;
};

RGBTransform::RGBTransform(){
	ifstream fp("XYZtable.txt");	//XYZ�e�[�u���̓ǂݍ���
	K = 0;
	d_lambda = 5.0*pow(10,-9.0);
	
	while(fp){
		double l,x,y,z,s;
		fp >> l >> x >> y >> z >> s;
		if(l < 300 || l > 700) continue;
		K += s*y*d_lambda;
		XYZTable.insert( map<int, Vec4>::value_type(l, Vec4(x,y,z,s)));
	}
	K = 100.0/K;
}


//CIE RGB�F���
MyColor RGBTransform::CIE_RGB(Vec3 XYZ){
	MyColor _RGB;
	_RGB.r =  2.3655*XYZ.x - 0.8971*XYZ.y - 0.4683*XYZ.z;
	_RGB.g = -0.5151*XYZ.x + 1.4264*XYZ.y + 0.0887*XYZ.z;
	_RGB.b =  0.0052*XYZ.x - 0.0144*XYZ.y + 1.0089*XYZ.z;
	return _RGB;
}

//sRGB �F���(D65)
MyColor RGBTransform::D65_RGB(Vec3 XYZ){
	MyColor _RGB;
	_RGB.r = 3.2410*XYZ.x - 1.5374*XYZ.y - 0.4986*XYZ.z;
	_RGB.g = 0.9692*XYZ.x + 1.8760*XYZ.y + 0.0416*XYZ.z;
	_RGB.b = 0.0556*XYZ.x - 0.2040*XYZ.y + 1.5070*XYZ.z;
	return _RGB;
}

void RGBTransform::draw(){
	glColor3d(RGB.r, RGB.g, RGB.b);
	glRectd(-1,-1, 1,1);
}

/*
//�t�@�C������,�g���Ƃ��̔��˗���ǂݍ���, 3�h�����擾
//�t�@�C���̃t�H�[�}�b�g��, ��s�� �g�� ���˗� 
Vec3 RGBTransform::getXYZ(){
	Vec3 XYZ = Vec3(0,0,0);
	vector<WaveData>::iterator it = data.begin();
	while(it != data.end()){
		XYZ.x += (XYZTable[(*it).wave_length]).x * (XYZTable[(*it).wave_length]).s * (*it).refrect;	//x(��)*s(��)*R(��)
		XYZ.y += (XYZTable[(*it).wave_length]).y * (XYZTable[(*it).wave_length]).s * (*it).refrect;	//y(��)*s(��)*R(��)
		XYZ.z += (XYZTable[(*it).wave_length]).z * (XYZTable[(*it).wave_length]).s * (*it).refrect;	//z(��)*s(��)*R(��)
		it++;
	}
	XYZ = K*XYZ;
	return XYZ;
}
*/

/*
RGBTransform::RGBTransform(string name){
	//�g���Ɣ��˗��̃f�[�^�擾
	int n=0;
	ifstream fp(name);
	while(fp){
		WaveData tmp;
		fp >> tmp.wave_length >> tmp.refrect;
		data.push_back(tmp);
		n++;
	}
	cout << "n = " << n << endl;

	//XYZ�e�[�u���̓ǂݍ���
	ifstream fp2("XYZtable.txt");
	K = 0;
	while(fp2){
		double l,x,y,z,s;
		fp2 >> l >> x >> y >> z >> s;
		K += s*y;
		XYZTable.insert( map<int, Vec4>::value_type(l, Vec4(x,y,z,s)));
	}

	K = 1.0/K;
	cout << K << endl;

	XYZ = getXYZ();
	//�F�̌v�Z
	RGB = CIE_RGB(XYZ);
	cout << "R = " << RGB.r << endl << "G = " << RGB.g << endl << "B = " << RGB.b << endl;
};
*/