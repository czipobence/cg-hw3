//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül 
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:  
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D, 
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi, 
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Czipo Bence
// Neptun : B462JZ
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy 
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem. 
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a 
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb 
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem, 
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.  
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat 
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)                                                                                                                                                                                                            
#include <OpenGL/gl.h>                                                                                                                                                                                                            
#include <OpenGL/glu.h>                                                                                                                                                                                                           
#include <GLUT/glut.h>                                                                                                                                                                                                            
#else                                                                                                                                                                                                                             
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)                                                                                                                                                                       
#include <windows.h>                                                                                                                                                                                                              
#endif                                                                                                                                                                                                                            
#include <GL/gl.h>                                                                                                                                                                                                                
#include <GL/glu.h>                                                                                                                                                                                                               
#include <GL/glut.h>                                                                                                                                                                                                              
#endif          


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

const float EPSILON = 0.001f;

//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
struct Vector {
   float x, y, z;

   Vector( ) { 
	x = y = z = 0;
   }
   Vector(float x0, float y0, float z0 = 0) { 
	x = x0; y = y0; z = z0;
   }
   Vector operator*(float a) const { 
	return Vector(x * a, y * a, z * a); 
   }
   Vector operator/(float a) const {
	return (*this) * (1.0 / a);
   }
   Vector operator+(const Vector& v) const {
 	return Vector(x + v.x, y + v.y, z + v.z); 
   }
   Vector operator-(const Vector& v) const {
 	return Vector(x - v.x, y - v.y, z - v.z); 
   }
   float operator*(const Vector& v) const { 	// dot product
	return (x * v.x + y * v.y + z * v.z); 
   }
   Vector operator%(const Vector& v) const { 	// cross product
	return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
   }
   bool operator==(const Vector& v) {
	return (*this - v).Length() < EPSILON;
   }
  
  bool isZero() {
	return (x*x < EPSILON) & (y*y < EPSILON) & (z*z < EPSILON);
  }
  
   float Length() const { return sqrt(x * x + y * y + z * z); }
   float Dist(Vector v) const { return (*this - v).Length(); }
   Vector norm() const {return *this * (1 / this->Length()); }
   
};
 
//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
   float r, g, b;

   Color( ) { 
	r = g = b = 0;
   }
   Color(float r0, float g0, float b0) { 
	r = r0; g = g0; b = b0;
   }
   Color operator*(float a) { 
	return Color(r * a, g * a, b * a); 
   }
   Color operator*(const Color& c) { 
	return Color(r * c.r, g * c.g, b * c.b); 
   }
   Color operator+(const Color& c) {
 	return Color(r + c.r, g + c.g, b + c.b); 
   }
};

const Color WHITE(1,1,1);
const Color GRAY(.2,.2,.2);
const Color BLACK(0,0,0);

const int SCREEN_WIDTH = 600;	// alkalmazás ablak felbontása
const int SCREEN_HEIGHT = 600;


Color image[SCREEN_WIDTH*SCREEN_HEIGHT];	// egy alkalmazás ablaknyi kép

void putVertex(const Vector& v) {
  glVertex3f(v.x, v.y, v.z);
}

void setNormal(const Vector& v) {
  glNormal3f(v.x, v.y, v.z);
}

void setColor(GLenum face, GLenum p, Color c) {
	float arr[4] = {c.r,c.g,c.b,1};
	glMaterialfv(face,p,arr);
}

struct Drawable {
	
	Vector p ,sc, rot;
	
	Drawable(const Vector& p, const Vector& sc = Vector(0,0,0), const Vector& rot = Vector(0,0,0)): 
	p(p), sc(sc), rot(rot) {}
	
	virtual void setProperties() {return;}
	virtual void drawItem() = 0;
	
	void setScale(const Vector& v) {sc = v;}
	
	void setTranslate(const Vector& v) {p = v;}
	
	void setRotate(const Vector& v) {rot = v;}
	void setRotate(float rx, float ry, float rz) {rot = Vector(rx,ry,rz);}
	
	void draw() {
		glPushMatrix();
		if (!p.isZero())					glTranslatef(p.x,p.y,p.z);
		if (!sc.isZero())					glScalef(sc.x,sc.y,sc.z);
		if (!(fabs(rot.x) < EPSILON))		glRotatef(rot.x,1,0,0);
		if (!(fabs(rot.y) < EPSILON))		glRotatef(rot.y,0,1,0);
		if (!(fabs(rot.z) < EPSILON))		glRotatef(rot.z,0,0,1);
		
		setProperties();
		drawItem();
		glPopMatrix();
	}
	virtual ~Drawable() {};
};

struct Plain : public Drawable {
	Vector n;
	Plain (const Vector& _p, const Vector & _n) : Drawable(_p), n(_n) {}
	void drawItem() {
		glBegin( GL_QUADS );
		for( int x = -20; x < 20; x++ ) {
			for( int z = -20; z < 20; z++ ) {
				setColor( GL_FRONT, GL_DIFFUSE, (x ^ z) & 1 ? WHITE : GRAY);
				glVertex3f(x * 1,     0, z * 1);
				glVertex3f((x+1) * 1, 0, z * 1);
				glVertex3f((x+1) * 1, 0, (z+1) * 1);
				glVertex3f(x * 1,     0, (z+1) * 1);
			}
		}
		glEnd( );	
			
	}
};

struct ColoredDrawable: public Drawable {
	Color kd;
	
	ColoredDrawable(Vector p, Color c) : Drawable(p), kd(c) {}
	
	void setProperties() {
		setColor(GL_FRONT,GL_AMBIENT, kd*.3);
        setColor(GL_FRONT,GL_DIFFUSE, kd);
        //glMaterialfv(GL_FRONT,GL_SPECULAR, ks);
        //glMaterialf (GL_FRONT,GL_SHININESS, 50);
	}
	
	virtual ~ ColoredDrawable() {}
	
};

struct BezierCurve : public ColoredDrawable {
    Vector cps[10];
    int num;

	BezierCurve(Vector p = Vector(0,0,0), Color c=Color(1,1,0)): ColoredDrawable(p,c) {
		num = 0;
	}

	BezierCurve(Vector * points, int n,Vector p=Vector(0,0,0), Color c=Color(0,1,0)): ColoredDrawable(p,c) {
		num = n;
		for (int i = 0; i <n; i++) cps[i] = points[i];
	}


	float B(int n, int i, float t) {
		float choose = 1;
      	for(int j = 1; j <= i; j++) choose *= (float)(n-j+1)/j;
      	return choose * pow(t, i) * pow(1-t, n-i);
	}
	
	void addPoint(Vector v) {
		if (num < 10) {
			cps[num] = v;
			num++;
		}
	}
	void addPoint(float px, float py, float pz) {
		addPoint(Vector(px,py,pz));
	}
	
	void finish() {
		addPoint(cps[0]);
	}

	Vector getVal(float t) {
		Vector rr(0,0,0);
		for(int i = 0; i < num; i++)
			rr = rr + cps[i] * B(num-1,i,t);
      	return rr;
	}
   
	//http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/Bezier/bezier-der.html
	Vector getDer(float t) {
		Vector rr(0,0,0);
		for(int i = 0; i < num-1; i++)
			rr = rr + (cps[i+1] - cps[i]) * B(num-2,i,t);
      	return rr;
	}
   
	void drawItem() {
	
		//glEnable(GL_COLOR_MATERIAL);
		//glColor3f(kd.r, kd.g, kd.b);
		
		glBegin(GL_LINE_STRIP);
		
		for (float t = 0; t < 1; t+= 0.01) {
			putVertex(getVal(t));
		}
		
		glEnd();
		
		/*glBegin(GL_LINE_STRIP);
		
		for (int i = 0; i<num;i++) {
			glVertex3f(cps[i]);
		}
		
		glEnd();*/
		
		
		//glDisable(GL_COLOR_MATERIAL);   
	}
   
   
   
};

struct Hermite {
	Vector p0, p1, v0, v1;
	long t0, t1;
	
	Hermite() {
	p0 = p1 = v0 = v1 = Vector(0,0);
	t0 = t1 = 0;
	}
	
	Hermite(Vector p0, Vector p1, Vector v0, Vector v1, long t0, long t1)
	: p0(p0), p1(p1), v0(v0),v1(v1),t0(t0),t1(t1) {}
	
	Vector getVal(float t) {
		float dt1 = t - t0;
		float dt = t1 - t0;
		
		Vector a0 = p0;
		Vector a1 = v0;
		Vector a2 = (p1- p0) * (3.0 / (dt * dt)) - (v1 + v0 * 2.0) * (1.0 / dt) ;
		Vector a3 = (p0 - p1) * (2.0 / dt / dt / dt) + (v0 + v1) * (1.0 / dt / dt);
		return (a3 * dt1 * dt1 * dt1 + a2 * dt1 * dt1 + a1 * dt1 + a0); 
	}
	
	Vector getDerived(float t) {
		float dt1 = t - t0;
		float dt = t1 - t0;
		
		Vector a1 = v0;
		Vector a2 = (p1- p0) * (3.0 / (dt * dt)) - (v1 + v0 * 2.0) * (1.0 / dt) ;
		Vector a3 = (p0 - p1) * (2.0 / dt / dt / dt) + (v0 + v1) * (1.0 / dt / dt);
		return (a3 * dt1 * dt1 * 3 + a2 * dt1 * 2 + a1); 
	}
	
};

struct CatmullRom : ColoredDrawable {
	Hermite splines[10];
	int n;
	
	CatmullRom(): ColoredDrawable(Vector(0,0,0), Color(0,0,0)), n(0) {}
	
	CatmullRom(Vector*pts, int len=0, Vector p=Vector(0,0,0), Color c=Color(1,0,0)):
	 ColoredDrawable(p,c), n(len-1) {
		for (int i = 0; i < n; i++) {
			splines[i].p0 = pts[i];
			splines[i].p1 = pts[i+1];
			splines[i].t0 = (float)i;
			splines[i].t1 = i + 1.0f;
		}
		splines[0].v0 = Vector(0,0,0);
		splines[n-1].v1 = Vector(0,0,0);
		for (int i = 0; i < n-1 ; i++) {
			splines[i].v1 = (pts[i+2] - pts[i]) * 0.5f;
		}
		for (int i = 1; i < n ; i++) {
			splines[i].v0 = splines[i-1].v1;
		}
	}
	
	
	Vector getVal(float t) {
		return splines[(int)(floor(t))].getVal(t);
	}
	
	Vector getDer(float t) {
		return splines[(int)(floor(t))].getDerived(t);
	}
	
	void drawItem() {
	
		//glEnable(GL_COLOR_MATERIAL);
		//glColor3f(kd.r, kd.g, kd.b);
		
		glBegin(GL_LINE_STRIP);
		
		for (float t = 0; t < n; t+= 0.01) {
			putVertex(getVal(t));
		}
		
		glEnd();
		
		/*glBegin(GL_LINE_STRIP);
		
		for (int i = 0; i<n;i++) {
			glVertex3f(splines[i].p0);
		}
		glVertex3f(splines[n-1].p1);
		
		glEnd();
		*/
		
		//glDisable(GL_COLOR_MATERIAL);   
	}
	 
};

struct UVDrawable : public ColoredDrawable {
	float uMin,uMax,du,vMin,vMax,dv;
	
	UVDrawable(	Vector _p, Color _c = Color(0,0,0),
				float uMin=0, float uMax = 2*M_PI, int nu = 30,
				float vMin=0, float vMax = 1*M_PI, int nv = 30):
				ColoredDrawable(_p,_c),uMin(uMin),uMax(uMax),vMin(vMin),vMax(vMax) {
					du = (uMax - uMin) / (float)nu;
					dv = (vMax - vMin) / (float)nv;
				}

	virtual Vector getVal(float u, float v) = 0;
	virtual Vector getNorm(float u, float v) = 0;
	

	void putPoint(float u, float v) {
		setNormal(getNorm(u,v));
		putVertex(getVal(u,v));		
	}

	void drawItem (){
        glBegin(GL_QUADS);
		
		for (float u = uMin; u < (uMax-du) + EPSILON ;u += du) {
			for (float v = vMin; v < (vMax-du)+EPSILON; v+= dv) {
				putPoint(u,v);
				putPoint(u+du,v);
				putPoint(u+du,v+dv);
				putPoint(u,v+dv);
			}
		}
		
		glEnd();
	}
	
	virtual ~UVDrawable(){}

};

void mkBz(BezierCurve* bzs, Vector also, Vector felso) {
	float hlp = 0.8f;
	Vector diff = felso - also;
	float rad = diff.Length();
	Vector tcp = felso + (diff) * hlp;  
	bzs->addPoint(also);
	bzs->addPoint(also.x,also.y,rad*hlp);
	bzs->addPoint(tcp.x,tcp.y,rad*hlp);
	bzs->addPoint(tcp.x,tcp.y,-rad*hlp);
	bzs->addPoint(also.x,also.y,-rad*hlp);
	bzs->finish();
}

struct CsirguruBodyHalo: public ColoredDrawable{
	static const int cm_siz = 4;
	CatmullRom cms[cm_siz];
	static const int bz_siz = 6;
	BezierCurve bzs[bz_siz];
	
	CsirguruBodyHalo(Vector p, Color c = Color(1,0,0)) : ColoredDrawable(p,c){
		
		
		mkBz(&(bzs[0]), Vector(-0.65,0.52,0), Vector(-0.648,0.522,0));
		
		mkBz(&(bzs[1]), Vector(-0.58,-0.13,0), Vector(-0.36,0.26,0));
		
		
		mkBz(&(bzs[2]), Vector(0,-0.5,0), Vector(0,0.22,0));
		
		
		mkBz(&(bzs[3]), Vector(0.45,-0.31,0), Vector(0.2,0.28,0));
		
		mkBz(&(bzs[4]), Vector(0.58,0.14,0), Vector(0.34,0.34,0));
		
		mkBz(&(bzs[5]), Vector(0.68,0.34,0), Vector(0.5,0.46,0));
		
	}
	
	void drawItem() {
		//for (int i = 0; i< cm_siz; i++) cms[i].draw();
		
		
		
		for (int i = 0; i< bz_siz; i++) {
			bzs[i].draw();
		}
		
		for (float i = 0; i<1.0f; i += 0.1f) {
			Vector list[bz_siz];
			for (int j = 0; j<bz_siz; j++) {
				list[j] = bzs[j].getVal(i);
			}
			CatmullRom(list,bz_siz).draw();
		}
		
	}
	
	
};

struct CsirguruBody: public UVDrawable{
	static const int cm_siz = 4;
	CatmullRom cms[cm_siz];
	static const int bz_siz = 6;
	BezierCurve bzs[bz_siz];
	
	CsirguruBody(Vector p, Color c = Color(1,0,0)) : 
		UVDrawable(p,c, 0, 1, 20, 0, bz_siz-2,20){
		/*bzs[0].addPoint(Vector(-2.5,1,0));
		bzs[0].addPoint(Vector(-2.5,1,0));
		bzs[0].addPoint(Vector(-2.5,1,0));
		bzs[0].addPoint(Vector(-2.5,1,0));
		bzs[0].addPoint(Vector(-2.5,1,0));
		
		bzs[1].addPoint(Vector(-2.2,.1,0));
		bzs[1].addPoint(Vector(-2.2,.1,0.7));
		bzs[1].addPoint(Vector(-1.8,0.85,0));
		bzs[1].addPoint(Vector(-2.2,.1,-0.7));
		bzs[1].addPoint(Vector(-2.2,.1,0));
		
		bzs[2].addPoint(Vector(-0.75,-1.1,0));
		bzs[2].addPoint(Vector(-0.75,-1.1,2.1));
		bzs[2].addPoint(Vector(-1.1,0.85,0));
		bzs[2].addPoint(Vector(-0.75,-1.1,-2.1));
		bzs[2].addPoint(Vector(-0.75,-1.1,0));
		
		bzs[3].addPoint(Vector(1.2,-0.55,0));*/
		
				
		mkBz(&(bzs[0]), Vector(-0.65,0.52,0), Vector(-0.648,0.522,0));
		
		mkBz(&(bzs[1]), Vector(-0.58,-0.13,0), Vector(-0.36,0.26,0));
		
		
		mkBz(&(bzs[2]), Vector(0,-0.5,0), Vector(0,0.22,0));
		
		
		mkBz(&(bzs[3]), Vector(0.45,-0.31,0), Vector(0.2,0.28,0));
		
		mkBz(&(bzs[4]), Vector(0.58,0.14,0), Vector(0.34,0.34,0));
		
		mkBz(&(bzs[5]), Vector(0.68,0.34,0), Vector(0.5,0.46,0));
	
		
		
		/*Vector list1[6] = {Vector(-2.5,1,0), Vector(-2.2,.1,0), Vector(-0.75,-1.1,0), 
			Vector(1.2,-0.55,0), Vector(1.9,0.85,0), Vector(2.2,1.6,0)};
		cms[0] = CatmullRom(list1, 6, Vector(0,0,0), Color(0,0,1)); 
		
		Vector list2[6] = {Vector(-2.5,1,0.01), Vector(-2.2,.1,0.7), Vector(-0.75,-1.1,2.1), 
			Vector(1.2,-0.55,2.8), Vector(1.9,0.85,1.4), Vector(2.2,1.6,0.3)};
		cms[1] = CatmullRom(list2, 6, Vector(0,0,0), Color(0,0,1)); 
		
		Vector list3[6] = {Vector(-2.5,1.01,0), Vector(-1.8,0.85,0), Vector(-1.1,0.85,0), 
			Vector(0.25,1.65,0), Vector(1,1.85,0), Vector(1.6,2,0)};
		cms[2] = CatmullRom(list3, 6, Vector(0,0,0), Color(0,0,1)); 
		
		Vector list4[6] = {Vector(-2.5,1,-0.01), Vector(-2.2,.1,-0.7), Vector(-0.75,-1.1,-2.1), 
			Vector(1.2,-0.55,-2.8), Vector(1.9,0.85,-1.4), Vector(2.2,1.6,-0.3)};
		cms[3] = CatmullRom(list4, 6, Vector(0,0,0), Color(0,0,1)); 
		
		for (int i = 0; i< bz_siz; i++) {
			bzs[i].addPoint(list1[i]);
			bzs[i].addPoint(list2[i]);
			bzs[i].addPoint(list3[i]);
			bzs[i].addPoint(list4[i]);
			bzs[i].addPoint(list1[i]);
		}
		*/
	}
	
	Vector getVal(float u, float v) {
		Vector list[bz_siz];
		for (int j = 0; j<bz_siz; j++) {
			list[j] = bzs[j].getVal(u);
		}
		return CatmullRom(list,bz_siz).getVal(v);
	}
	
	Vector getNorm(float u, float v) {
		Vector list[bz_siz];
		for (int j = 0; j<bz_siz; j++) {
			list[j] = bzs[j].getVal(u);
		}
		Vector tan1 = CatmullRom(list,bz_siz).getDer(v);
		
		
		for (int j = 0; j<bz_siz; j++) {
			list[j] = bzs[j].getDer(u);
		}
		Vector tan2 = CatmullRom(list,bz_siz).getVal(v);
		return (tan1%tan2).norm();
	}
	
	
	
	
	/*void drawItem() {
		//for (int i = 0; i< cm_siz; i++) cms[i].draw();
		
		
		
		for (int i = 0; i< bz_siz; i++) {
			bzs[i].draw();
		}
		
		for (float i = 0; i<1.0f; i += 0.1f) {
			Vector list[bz_siz];
			for (int j = 0; j<bz_siz; j++) {
				list[j] = bzs[j].getVal(i);
			}
			CatmullRom(list,bz_siz).draw();
		}
		
	}*/
	
	
};

struct Cone: public ColoredDrawable {
	float r, h;
	static const float step = 2 * M_PI / 20.0f;
	
	Cone(Vector p = Vector(0,0,0), Color c = Color(), float r = 1, float h = 1):
	ColoredDrawable(p,c), r(r), h(h) {}
	
	virtual void drawItem() {
		
		glBegin(GL_TRIANGLE_FAN);
		
		glNormal3f(0,1,0);
		glVertex3f(0,h,0);
		
		for (int i = 0; i<21;i++) {
			float t = i * step;
			glNormal3f(cos(t),0,sin(t));
			glVertex3f(r*cos(t), 0, r * sin(t));
		}
		
		glEnd();
		
		glBegin(GL_TRIANGLE_FAN);
		
		glNormal3f(0,-1,0);
		glVertex3f(0,0,0);
		
		for (int i = 0; i<21;i++) {
			float t = i * step;
			glVertex3f(r*sin(t), 0, r * cos(t));
		}
		
		glEnd();
		
	}
	
};

struct Cylinder: public ColoredDrawable {
	float r,h;
	static const float step = 2 * M_PI / 20.0f;
	
	Cylinder(Vector p, Color c = Color(), float h = 1, float r=1):
	ColoredDrawable(p,c), r(r), h(h) {}
	
	virtual void drawItem() {
		glBegin(GL_TRIANGLE_STRIP);
		
		for (int i = 0; i<21; i++) {
			float t = i * step;
			glNormal3f(sin(t),0,cos(t));
			glVertex3f(r*sin(t), 0, r * cos(t));
			glVertex3f(r*sin(t), h, r * cos(t));
		}
		
		glEnd();
		
		glBegin(GL_TRIANGLE_FAN);
		
		glNormal3f(0,1,0);
		glVertex3f(0,h,0);
		
		for (int i = 0; i<21; i++) {
			float t = i * step;
			glVertex3f(r*cos(t), h, r * sin(t));
		}
		
		glEnd();
		
		glBegin(GL_TRIANGLE_FAN);
		
		glNormal3f(0,-1,0);
		glVertex3f(0,0,0);
		
		for (int i = 0; i<21; i++) {
			float t = i * step;
			glVertex3f(r*sin(t), 0, r * cos(t));
		}
		
		glEnd();
		
		
	}
	
};


struct Sphere: public UVDrawable {
	float r;
	
	Sphere(Vector center = Vector(), float radius = 1, Color c = Color()):
	UVDrawable(center,c,0,2*M_PI,20,0,M_PI,10),r(radius) {} 
	
	Vector getVal(float u, float v) {
		return Vector (
			(float)(r * cos(u) * sin (v)),
			(float)(r * sin(u) * sin (v)),
			(float)(r * cos(v))
		);
	}
	
	Vector getNorm(float u, float v) {return (getVal(u,v)).norm();}
	
};

struct Bone: public Drawable{
	Cylinder b;
	Sphere j;
	
	Bone(Vector p, int len) : Drawable(p), b(p), j(p) {}
	
	void draw() {
		b.draw();
		j.draw();
	}
	
};


const Color CHICKEN_CREST_COLOR (.9,0,0);
const Color CHICKEN_BODY_COLOR (.8,.8,.6);
const Color CHICKEN_BILL_COLOR (.9,.7,.1);
const Color CHICKEN_EYE_COLOR = BLACK;

struct Csirguru: public Drawable {
	
	static const int NUM_OF_PARTS = 8;
	
	Drawable* parts[NUM_OF_PARTS];
	
	Sphere head;
	CsirguruBody body;
	Cone bill;
	Cone crest[3];
	Sphere eye[2];
	
	Csirguru (Vector middle): Drawable(middle), 
					head(Vector(0.55,0.38,0),.32,CHICKEN_BODY_COLOR),
					body(Vector(0,0,0), CHICKEN_BODY_COLOR),
					bill(Vector(head.p + Vector(.1,-0.05,0)), CHICKEN_BILL_COLOR, 0.22, .4) {
						bill.setRotate(0,0,-110);
						crest[0] = Cone(head.p+Vector(0.04,.28,0), CHICKEN_CREST_COLOR, .1,.23);
						crest[1] = Cone(head.p+Vector(-0.07,.25,0), CHICKEN_CREST_COLOR, .1,.23);
						crest[1].setRotate(0,0,20);
						crest[2] = Cone(head.p+Vector(.14,.25,0), CHICKEN_CREST_COLOR, .1,.23);
						crest[2].setRotate(0,0,-20);
						eye[0] = Sphere(head.p + Vector(0.28,0.07,-0.12), 0.05, CHICKEN_EYE_COLOR);
						eye[1] = Sphere(head.p + Vector(0.28,0.07,0.12), 0.05, CHICKEN_EYE_COLOR);
						
						parts[0] = new CsirguruBody(Vector(0,0,0), CHICKEN_BODY_COLOR);
						
						parts[1] = new Sphere(Vector(0.55,0.38,0),.32,CHICKEN_BODY_COLOR);
						
						parts[2] = new Cone(Vector(head.p + Vector(.1,-0.05,0)), CHICKEN_BILL_COLOR, 0.22, .4);
						parts[2] -> setRotate(0,0,-110);
						
						parts[3] = new Cone(head.p+Vector(0.04,.28,0), CHICKEN_CREST_COLOR, .1,.23);
						
						parts[4] = new Cone(head.p+Vector(-0.07,.25,0), CHICKEN_CREST_COLOR, .1,.23);
						parts[4] -> setRotate(0,0,20);
						
						parts[5] = new Cone(head.p+Vector(.14,.25,0), CHICKEN_CREST_COLOR, .1,.23);
						parts[5] -> setRotate(0,0,-20);
						
						
						parts[6] = new Sphere(head.p + Vector(0.28,0.07,-0.12), 0.05, CHICKEN_EYE_COLOR);
						parts[7] = new Sphere(head.p + Vector(0.28,0.07,0.12), 0.05, CHICKEN_EYE_COLOR);
						
					}
	
	void drawItem() {
		/*body.draw();
		head.draw();
		bill.draw();
		for (int i = 0; i < 3; i++) {
			crest[i].draw();
		}
		eye[0].draw();
		eye[1].draw();*/
		for (int i = 0 ; i<NUM_OF_PARTS;i++) {
			parts[i] -> draw();
		}
	}
};

////////////////////////////////////////////////////////////////////////

struct Camera {
	Vector pos,dir,up, eye,right;
	
	
	Camera(	Vector pos=Vector(0,1,2), 
			Vector dir=Vector(0,0,-1), 
			Vector up=Vector(0,1,0)):
			pos(pos), dir(dir), up(up) {
				fit();
			} 
			
	void fit() {
		eye = pos + dir;
		dir = dir.norm();
		this -> right = (dir % up).norm();
		this -> up = (this-> right % this->dir).norm();
	}
	
	void set() {
		gluLookAt(pos.x,pos.y,pos.z,eye.x,eye.y,eye.z,up.x,up.y,up.z);
	}
};

////////////////////////////////////////////////////////////////////////

struct CsirguruWrapper {
	Csirguru* csg;
	CsirguruWrapper* next;
	
	CsirguruWrapper(Vector pos = Vector(0,0,0)) {
		csg = new Csirguru(Vector(pos));
		next = NULL;
	}
	
	void drawCsg() {
		csg -> draw();
		if (next != NULL)
			next -> drawCsg();
	}
	
	~CsirguruWrapper() {
		delete csg;
	}
	
};

struct World {
	Camera cam;
	CsirguruWrapper* firstCsg;
	CsirguruWrapper* lastCsg;
	
	World() {
		firstCsg = lastCsg = new CsirguruWrapper(Vector(0,1,0));
		
		
	/*createCsirguru(Vector(5,1,0));
	createCsirguru(Vector(0,1,5));
	createCsirguru(Vector(5,1,5));
	createCsirguru(Vector(0,1,-5));
	createCsirguru(Vector(5,1,-5));
	createCsirguru(Vector(5,4,0));
	createCsirguru(Vector(0,4,5));
	createCsirguru(Vector(5,4,5));
	createCsirguru(Vector(0,4,-5));
	createCsirguru(Vector(5,4,-5));*/
	
		
	}
	
	void createCsirguru(Vector v) {
		CsirguruWrapper* uj = new CsirguruWrapper(v);
		lastCsg -> next = uj;
		lastCsg = uj;
	}
	
	void draw() {
		firstCsg -> drawCsg();
	}
	
	
	
};

World world;

////////////////////////////////////////////////////////////////////////
// TEX T�RA
////////////////////////////////////////////////////////////////////////



GLuint tex;

void createTexture() {
	glGenTextures(1,  &tex);

	const char* ascii_textures = {
		"....********...."
		"....********...."
		"....********...."
		"....********...."
		"****++++++++****"
		"****++++++++****"
		"****++++++++****"
		"****++++++++****"
		"****++++++++****"
		"****++++++++****"
		"****++++++++****"
		"****++++++++****"
		"....********...."
		"....********...."
		"....********...."
		"....********...."
	};

	GLubyte texture_data[256][3];
    for(int i = 0; i < 256; i++) {
		switch(ascii_textures[i]) {
			case '*':
				texture_data[i][0] = 0x30;
				texture_data[i][1] = 0xC0;
				texture_data[i][2] = 0x00;
			break;
			case '+':
				texture_data[i][0] = 0x00;
				texture_data[i][1] = 0xFF;
				texture_data[i][2] = 0x00;
			break;
			default:
				texture_data[i][0] = 0x30;
				texture_data[i][1] = 0x90;
				texture_data[i][2] = 0x30;
			break;
      }
    }

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);

	//glBindTexture(GL_TEXTURE_2D, 0);
}

/////////////////////////////////////////////////////////////////////////
// END
/////////////////////////////////////////////////////////////////////////





void onInitialization( ) { 
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glEnable(GL_DEPTH_TEST);
	
	float Ia[4] = {0.1, 0.1, 0.1, 1}, Id[4] = {1.0, 1.0, 1.0, 1}, Is[4] = {2, 2, 2, 1};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ia);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Id);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Is);
	glEnable(GL_LIGHT0);

	
	glShadeModel(GL_SMOOTH);
	
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	createTexture();
    
}

float lightdir[4] = {1,1,1,0};

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1, 100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	world.cam.set();
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightdir);
	
	setColor( GL_FRONT, GL_AMBIENT, GRAY);
	//glMaterialfv( GL_FRONT, GL_SPECULAR, BLACK);	
	
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_TEXTURE_2D); 
	setColor( GL_FRONT, GL_DIFFUSE, WHITE);
	for( int x = -10; x < 10; x++ ) {
		for( int z = -10; z < 10; z++ ) {
			glBindTexture(GL_TEXTURE_2D, tex); // Ezt semmiképpen se rakd a glBegin - glEnd blokk közé

			glBegin(GL_QUADS);			
				glNormal3f(0,1,0);
				glTexCoord2f(0, 0);
				glVertex3f(x * 1,     0, z * 1);
				glTexCoord2f(1, 0);
				glVertex3f((x+1) * 1, 0, z * 1);
				glTexCoord2f(1, 1);
				glVertex3f((x+1) * 1, 0, (z+1) * 1);
				glTexCoord2f(0, 1);
				glVertex3f(x * 1,     0, (z+1) * 1);
			glEnd();
		}
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
	

	/*Sphere a (Vector(0,1,0),1);
	a.draw();
	Cone b (Vector(2,3,0), 1, 1);
	b.draw();*/
	//Cylinder c (Vector(-2,5,0), 1, 1);
	//c.draw();
	
	/*BezierCurve d (Vector(-2,0,0));
	d.addPoint(Vector(0,0,0));
	d.addPoint(Vector(0,0,1));
	d.addPoint(Vector(0,1,6));
	d.addPoint(Vector(0,2,1));
	d.addPoint(Vector(0,6,0));
	d.addPoint(Vector(0,2,-1));
	d.addPoint(Vector(0,0,-1));
	d.addPoint(Vector(0,0,0));
	d.draw();
	*/
	
	/*Vector pts[5] = {Vector(0,0,0), Vector(0,0,1), Vector(0,2,1), Vector(0,2,-1), Vector(0,0,-1)};
	CatmullRom e (pts, 5, Vector(-2,0,0), Color(0,1,0));
	e.draw();*/
	
	//CsirguruBody f(Vector(-2,2,0), Color(1,0,0));
	//f.draw();
	
	//Csirguru g(Vector(0,1,0));
	//g.draw();
	
	world.draw();
	
	CsirguruBodyHalo h (Vector(0,1,0));
	//h.draw();
	
	float shadow_mtx[4][4] = {1,                         0,       0,                       0,
		                      -lightdir[0]/lightdir[1],  0,     -lightdir[2]/lightdir[1],  0,
							   0,                        0,      1,                        0,
							   0,                    0.001,      0,                        1};


	glMultMatrixf( &shadow_mtx[0][0] );
	glDisable(GL_LIGHTING);
	glColor3f(0, 0, 0);
	
	//world.draw();
	
	//f.draw();
	//a.draw();

	glutSwapBuffers();     				// Buffercsere: rajzolas vege

}


// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
	if (key == 'a') glutPostRedisplay( ); //MOVE LEFT 
    if (key == 'w') glutPostRedisplay( ); //MOVE UP
    if (key == 'd') glutPostRedisplay( ); //MOVE RIGHT
    if (key == 'y') glutPostRedisplay( ); //MOVE DOWN
    if (key == ' ') glutPostRedisplay( ); //THROW BOMB
    
   float UNIT = .5;
	if (key == 'a') {
		world.cam.pos = world.cam.pos - world.cam.right * UNIT;
		glutPostRedisplay( );
	}
	if (key == 'd') {
		world.cam.pos = world.cam.pos + world.cam.right * UNIT;
		glutPostRedisplay( );
	}
	if (key == 'w') {
		world.cam.pos = world.cam.pos + world.cam.dir * UNIT;
		glutPostRedisplay( );
	}
	if (key == 's') {
		world.cam.pos = world.cam.pos - world.cam.dir * UNIT;
		glutPostRedisplay( );
	}
	if (key == 'r') {
		world.cam.pos = world.cam.pos + world.cam.up * UNIT;
		glutPostRedisplay( );
	}
	if (key == 'f') {
		world.cam.pos = world.cam.pos - world.cam.up * UNIT;
		glutPostRedisplay( );
	}
	if (key == '6') {
		world.cam.dir = (world.cam.dir % world.cam.up + world.cam.dir * 3) / 4; 
		glutPostRedisplay( );
	}
	if (key == '4') {
		world.cam.dir = (world.cam.up % world.cam.dir + world.cam.dir * 3) / 4; 
		glutPostRedisplay( );
	}
	if (key == '8') {
		world.cam.dir = (world.cam.right % world.cam.dir + world.cam.dir*3) / 4; 
		world.cam.up = world.cam.right % world.cam.dir;
		glutPostRedisplay( );
	}
	if (key == '2') {
		world.cam.dir = (world.cam.dir % world.cam.right + world.cam.dir*3) / 4; 
		world.cam.up = world.cam.right % world.cam.dir;
		glutPostRedisplay( );
	}
	
	world.cam.fit();
}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
		glutPostRedisplay( ); 						 // Ilyenkor rajzold ujra a kepet
}

// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{

}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {

}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel 
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();					// Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse); 
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMotionFunc(onMouseMotion);

    glutMainLoop();					// Esemenykezelo hurok
    
    return 0;
}
