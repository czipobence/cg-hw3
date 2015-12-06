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
#include <stdio.h>
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
const float G = 6;
float GLOBAL_TIME = 0;

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

const int SCREEN_WIDTH = 600;	
const int SCREEN_HEIGHT = 600;


Color image[SCREEN_WIDTH*SCREEN_HEIGHT];

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
		if (!(fabs(rot.x) < EPSILON))		glRotatef(rot.x,1,0,0);
		if (!(fabs(rot.y) < EPSILON))		glRotatef(rot.y,0,1,0);
		if (!(fabs(rot.z) < EPSILON))		glRotatef(rot.z,0,0,1);
		if (!sc.isZero())					glScalef(sc.x,sc.y,sc.z);
		
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
	}
	
	virtual ~ ColoredDrawable() {}
	
};

struct BezierCurve {
    Vector cps[10];
    int num;

	BezierCurve() {
		num = 0;
	}

	BezierCurve(Vector * points, int n) {
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

struct CatmullRom {
	Hermite splines[10];
	int n;
	
	CatmullRom():n(0) {}
	
	CatmullRom(Vector*pts, int len=0): n(len-1) {
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

struct CsirguruBody: public ColoredDrawable{
	static const int bz_siz = 6;
	static const int cm_siz = 10;
	BezierCurve bzs[bz_siz];
	CatmullRom cms[cm_siz];
	CatmullRom der[cm_siz];
	
	CsirguruBody(Vector p, Color c = Color(1,0,0)) : 
		ColoredDrawable(p,c){
		
				
		mkBz(&(bzs[0]), Vector(-0.65,0.52,0), Vector(-0.648,0.522,0));
		
		mkBz(&(bzs[1]), Vector(-0.58,-0.13,0), Vector(-0.36,0.26,0));
		
		
		mkBz(&(bzs[2]), Vector(0,-0.5,0), Vector(0,0.22,0));
		
		
		mkBz(&(bzs[3]), Vector(0.45,-0.31,0), Vector(0.2,0.28,0));
		
		mkBz(&(bzs[4]), Vector(0.58,0.14,0), Vector(0.34,0.34,0));
		
		mkBz(&(bzs[5]), Vector(0.68,0.34,0), Vector(0.5,0.46,0));
		
		Vector list[bz_siz];
		
		for (int i = 0; i<cm_siz; i++) {
			float u = (float)i / (cm_siz-1);
			for (int j = 0; j<bz_siz; j++) {
				list[j] = bzs[j].getVal(u);
			}
			cms[i] = CatmullRom(list,bz_siz);
			for (int j = 0; j<bz_siz; j++) {
				list[j] = bzs[j].getDer(u);
			}
			der[i] = CatmullRom(list,bz_siz);
		}
		
	}
	
	
	
	
	
	void drawItem() {
		for (int i = 0; i<cm_siz-1; i++) {
			glBegin(GL_TRIANGLE_STRIP);
			for (int j = 0; j < 20; j++) {
				float v = j * bz_siz / 19.0;
				
				setNormal((cms[i].getDer(v) % (der[i].getVal(v))).norm() );
				putVertex(cms[i].getVal(v));
				
				setNormal((cms[i+1].getDer(v) % (der[i+1].getVal(v))).norm() );
				putVertex(cms[i+1].getVal(v));
			}
			glEnd();
		}
		
		
		glBegin(GL_TRIANGLE_FAN);
		
		setNormal( ((bzs[bz_siz-1].getVal(0.3333) - bzs[bz_siz-1].getVal(0)) % (bzs[bz_siz-1].getVal(0) - bzs[bz_siz-1].getVal(0.666666))).norm() );
		
		for (int i =0; i < cm_siz; i++) {
			float u = i / (float) cm_siz;
			putVertex(bzs[bz_siz-1].getVal(u));
		}
		
		glEnd();
		
	}
	
	
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

struct Sphere: public ColoredDrawable {
	float r;
	
	Sphere(Vector center = Vector(), Color c = Color(), float radius = 1):
	ColoredDrawable(center,c), r(radius) {} 
	
	void drawItem() {
		for (int i = 0; i< 15;i++) {
			float u = i  * 2 * M_PI / 15.0;
			float u_n = (i + 1) * 2* M_PI / 15.0;
			glBegin(GL_TRIANGLE_STRIP);
			for (int j = 0; j < 8; j++) {
				float v = j * M_PI / 7;
				
				glNormal3f(cos(u) * sin(v), sin(u) * sin(v), cos(v));
				glVertex3f(r*cos(u) * sin(v), r*sin(u) * sin(v), r*cos(v));
				
				glNormal3f(cos(u_n) * sin(v), sin(u_n) * sin(v), cos(v));
				glVertex3f(r*cos(u_n) * sin(v), r*sin(u_n) * sin(v), r*cos(v));
				
			}
			glEnd();
		}
		
	}
	
};



const Color CHICKEN_CREST_COLOR (.9,0,0);
const Color CHICKEN_BODY_COLOR (.8,.8,.6);
const Color CHICKEN_BILL_COLOR (.9,.7,.1);
const Color CHICKEN_EYE_COLOR = BLACK;
const Color CHICKEN_LEG_COLOR (1,1,0);

struct Csirguru: public Drawable {
	
	static const float CHICKEN_HEIGHT_START = 0.45;
	static const float CHICKEN_BONE_RADIUS = 0.05;
	static const float CHICKEN_BONE_LENGTH = 0.3;
	
	const static float A = 9;
	const static float V1 = 2.6832;
	const static float V0 = 3;
	
	static const float JMP_ANGLE_SIN = 0.7071;
	static const float JMP_ANGLE_COS = 0.7071;
	
	Vector p0,rot0;
	
	float c_height;
	int phase;
	long phase_entered;
	float turn_angle;
	
	
	CsirguruBody body;
	Sphere head;
	Cone bill;
	Cone crest[3];
	Sphere eye[2];
	Cylinder toe;
	Sphere knuckle;
	Cylinder foot;
	Sphere ankle;
	Cylinder leg;
	Sphere knee;
	
	
	Csirguru (Vector middle): Drawable(middle), 
					c_height(CHICKEN_HEIGHT_START), phase(0),
					body(Vector(0,.5,0), CHICKEN_BODY_COLOR),
					head(body.p + Vector(0.55,0.38,0),CHICKEN_BODY_COLOR, .32),
					bill(Vector(head.p + Vector(.1,-0.05,0)), CHICKEN_BILL_COLOR, 0.22, .4),
					toe(Vector(0,CHICKEN_BONE_RADIUS,0), CHICKEN_LEG_COLOR, CHICKEN_BONE_LENGTH, CHICKEN_BONE_RADIUS),
					knuckle(Vector(0,CHICKEN_BONE_RADIUS,0), CHICKEN_LEG_COLOR, CHICKEN_BONE_RADIUS),
					foot(Vector(0,CHICKEN_BONE_RADIUS,0), CHICKEN_LEG_COLOR, CHICKEN_BONE_LENGTH,  CHICKEN_BONE_RADIUS),
					ankle(Vector(0,0,0), CHICKEN_LEG_COLOR, CHICKEN_BONE_RADIUS),
					leg(Vector(0,0,0), CHICKEN_LEG_COLOR, CHICKEN_BONE_LENGTH, CHICKEN_BONE_RADIUS),
					knee(Vector(0,0,0), CHICKEN_BODY_COLOR, CHICKEN_BONE_RADIUS) {
						phase_entered =  GLOBAL_TIME;
						bill.setRotate(0,0,-110);
						crest[0] = Cone(head.p+Vector(0.04,.28,0), CHICKEN_CREST_COLOR, .1,.23);
						crest[1] = Cone(head.p+Vector(-0.07,.25,0), CHICKEN_CREST_COLOR, .1,.23);
						crest[1].setRotate(0,0,20);
						crest[2] = Cone(head.p+Vector(.14,.25,0), CHICKEN_CREST_COLOR, .1,.23);
						crest[2].setRotate(0,0,-20);
						eye[0] = Sphere(head.p + Vector(0.28,0.07,-0.12), CHICKEN_EYE_COLOR, 0.05);
						eye[1] = Sphere(head.p + Vector(0.28,0.07,0.12), CHICKEN_EYE_COLOR, 0.05);
						
						toe.setRotate(Vector(0,0,-90));
						
					}
	
	void drawItem() {
		
		long tn = GLOBAL_TIME;
		float dt = (tn - phase_entered) / 1000.0;
		
		float dx = V0 * JMP_ANGLE_COS * dt;
			
		switch (phase) {
			case 0:
			c_height = CHICKEN_HEIGHT_START - V1 * dt + dt*dt/2 * A;
			if (c_height > 2 * CHICKEN_BONE_LENGTH) {
				c_height = 2 * CHICKEN_BONE_LENGTH;
				phase = 1;
				phase_entered = tn;
				p0 = p;
				dt = 0;
			}
				break;
			case 1:
			c_height = 2* CHICKEN_BONE_LENGTH;
			setTranslate(p0 + Vector( dx * cos(rot.y * M_PI / 180) , V0 * JMP_ANGLE_SIN * dt - G / 2 * dt *dt , -dx * sin(rot.y * M_PI / 180)));
			if (V0 * 0.8 * dt < G / 2 * dt *dt) {
				setTranslate(Vector(p.x,p0.y,p.z));
				phase = 2;
				phase_entered = tn;
			}
			break;
			case 2:
			c_height = 2 * CHICKEN_BONE_LENGTH - V0 * dt + A / 2 * dt *dt;
			if (c_height < CHICKEN_HEIGHT_START) phase++;
			break;
			case 3:
			c_height = 2 * CHICKEN_BONE_LENGTH - V0 * dt + A / 2 * dt *dt;
			if (c_height > CHICKEN_HEIGHT_START) {
				c_height = CHICKEN_HEIGHT_START;
				phase = 4;
				phase_entered = tn;
				rot0 = rot;
				turn_angle = rand() % 181 - 90;
				if (p.Length() > 9) {
					turn_angle = 180;
				}
			}
			break;
			case 4:
			setRotate(rot0 + Vector(0,dt*turn_angle,0));
			if (dt > 1) {
				setRotate(rot0 + Vector(0,turn_angle,0));
				phase = 0;
				phase_entered = tn;
			}
			break;
		}
		float angRad = asinf(c_height / 2 / CHICKEN_BONE_LENGTH );	
		float legAngle = angRad / M_PI * 180.0;
		toe.draw();
		
		knuckle.draw();
		
		foot.setRotate(Vector(0,0,(legAngle-90)));
		foot.draw();
		
		ankle.setTranslate(Vector(CHICKEN_BONE_LENGTH*cos(angRad),CHICKEN_BONE_RADIUS+CHICKEN_BONE_LENGTH*sin(angRad),0));
		ankle.draw();
		
		leg.setRotate(Vector(0,0,90-legAngle));
		leg.setTranslate(Vector(CHICKEN_BONE_LENGTH*cos(angRad),CHICKEN_BONE_RADIUS+CHICKEN_BONE_LENGTH*sin(angRad),0));
		leg.draw();
		
		glPushMatrix();
		glTranslatef(0,CHICKEN_BONE_LENGTH * 2* sin(angRad) + CHICKEN_BONE_RADIUS, 0);
		
		knee.draw();
		
		body.draw();
		head.draw();
		bill.draw();
		for (int i = 0; i < 3; i++) {
			crest[i].draw();
		}
		eye[0].draw();
		eye[1].draw();
		glPopMatrix();

	}

};

struct ThrownDrawable {
	Vector p0, v0, r0, w0;
	long t_start;
	Drawable *itm;
	ThrownDrawable* next;
	
	ThrownDrawable(Drawable* wt, Vector offset=Vector()){
		next = NULL;
		itm = wt;
		p0 = offset + itm -> p;
		r0 = itm -> rot;
		v0 = Vector((rand()%10) / 5.0  -1, (rand()%5) / 5.0 + 4, (rand()%10) / 5.0 -1);
		w0 = Vector((rand()%180) / 1.0, (rand()%180) / 1.0, (rand()%180) / 1.0);
		t_start = GLOBAL_TIME;
	}
	
	void drawItem() {
		
		float dt = (GLOBAL_TIME- t_start) / 1000.0f;
		itm -> setTranslate(p0 + v0 * dt - Vector(0,G/2,0)*dt*dt);
		itm -> setRotate(r0 + w0 * dt);
		itm -> draw();
		
		if (next != NULL) next -> drawItem();
		
	}
	
	bool aboveGround() {
		return itm -> p.y > 0;
	}
	
	~ThrownDrawable() {
		delete itm;
	}
};

////////////////////////////////////////////////////////////////////////

struct Camera {
	Vector pos,dir,up, eye,right;
	
	
	Camera(	Vector pos=Vector(0,5,10), 
			Vector dir=Vector(0,-0.5,-1), 
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
		csg -> rot = Vector(0,rand() % 360,0);
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

struct Bomb {
	static const float RANGE = 2.0f;
	Vector p0;
	long started;
	Sphere bomb;
	
	Bomb(Vector p = Vector()) : p0(p), started(0), bomb(p0, GRAY, .3) {}
	
	void start() {if (!started) started = GLOBAL_TIME;}
	
	void draw() {
		if (started) {
			float dt = (GLOBAL_TIME - started)/1000.0f;
			bomb.setTranslate(p0 - Vector(0,G/2,0) * dt*dt);
		} else {
			bomb.setTranslate(p0);
		}
		bomb.draw();
	}
	
};

struct World {
	Camera cam;
	CsirguruWrapper* firstCsg;
	CsirguruWrapper* lastCsg;
	ThrownDrawable* firstTh;
	ThrownDrawable* lastTh;
	ThrownDrawable* td;
	Bomb bomb;
	long csgAdded;
	int csgCount;
	static const int CSG_COUNT_MAX = 5;
	
	void init() {
		firstCsg = lastCsg = new CsirguruWrapper(Vector(0,0,1));
		firstTh = lastTh = NULL;
		firstCsg -> drawCsg();
		bomb = Bomb(cam.pos + Vector(cam.dir.x,0,cam.dir.y) * 15);
		csgAdded = 0;
		csgCount = 1;
	}
	
	void createCsirguru(Vector v) {
		csgCount++;
		csgAdded = GLOBAL_TIME;
		CsirguruWrapper* uj = new CsirguruWrapper(v);
		if (firstCsg == NULL) {
			firstCsg = lastCsg = uj;
			return;
		}
		lastCsg -> next = uj;
		lastCsg = uj;
	}
	
	void addCsgUnderBomb() {
		if (csgCount < CSG_COUNT_MAX)
		createCsirguru(Vector(bomb.p0.x,0,bomb.p0.z));
	}
	
	void addThr(Drawable* d, Vector offset = Vector()) {
		ThrownDrawable *uj = new ThrownDrawable(d,offset);
		
		if (lastTh == NULL) {
			firstTh = lastTh = uj;
			return;
		}
		
		lastTh -> next = uj;
		lastTh = uj;
	}
	
	void removeCsirguru(CsirguruWrapper* cs) {
		Vector pcs = cs-> csg -> p;
		Vector offset = pcs + Vector(0,cs->csg->c_height + cs->csg->CHICKEN_BONE_RADIUS, 0);
		addThr(new CsirguruBody(cs->csg->body),offset);
		addThr(new Sphere(cs->csg->head),offset);
		addThr(new Sphere(cs->csg->eye[0]),offset);
		addThr(new Sphere(cs->csg->eye[1]),offset);
		addThr(new Cone(cs->csg->bill),offset);
		addThr(new Cone(cs->csg->crest[0]),offset);
		addThr(new Cone(cs->csg->crest[1]),offset);
		addThr(new Cone(cs->csg->crest[2]),offset);
		addThr(new Sphere(cs->csg->knee),offset);
		addThr(new Sphere(cs->csg->ankle),pcs);
		addThr(new Sphere(cs->csg->knuckle),pcs);
		addThr(new Cylinder(cs->csg->toe),pcs);
		addThr(new Cylinder(cs->csg->foot),pcs);
		addThr(new Cylinder(cs->csg->leg),pcs);
		
		CsirguruWrapper *ptr,*lem;
		csgCount--;
		if (cs == firstCsg) {
			ptr = firstCsg;
			firstCsg = firstCsg->next;
			delete ptr;
		} else {
			lem = firstCsg;
			ptr = firstCsg -> next;
			while (ptr != NULL) {
				if (ptr == cs) {
					lem -> next = ptr -> next;
					if (ptr == lastCsg) lastCsg = lem;
					delete ptr;
					ptr = NULL;
				} else {
					lem = ptr;
					ptr = ptr -> next;
				}
			}
		}
		if (firstCsg == NULL) lastCsg = NULL;
		
	}
	
	void explodeBomb(Vector pos) {
		bomb.started = 0;
		bomb.p0 = cam.pos + Vector(cam.dir.x,0,cam.dir.y) * 15;
		if (firstCsg == NULL) return;
		
		if (firstCsg -> next == NULL) {
			if ((firstCsg -> csg -> p - pos).Length() < bomb.RANGE) removeCsirguru(firstCsg);
			return;
		}
		
		CsirguruWrapper *ptr = firstCsg-> next;
		CsirguruWrapper *lem = firstCsg;
		while (ptr != NULL) {
			if ((ptr -> csg -> p - pos).Length() < bomb.RANGE) {
				ptr = ptr -> next;
				removeCsirguru(lem->next);
			} else {
				lem = ptr;
				ptr = ptr -> next;
			}
		}
	}
	
	void draw() {
		if (!bomb.started) bomb.p0 = cam.pos + Vector(cam.dir.x,0,cam.dir.y) * 15;
		bomb.draw();
		if (bomb.bomb.p.y < 0.1) explodeBomb(bomb.bomb.p);
		if (firstCsg != NULL)
			firstCsg -> drawCsg();
		if (firstTh != NULL) {
			firstTh -> drawItem();
		
			ThrownDrawable* ptr, *lem; 
			while (firstTh != NULL && !(firstTh -> aboveGround())) {
					ptr = firstTh -> next;
					delete firstTh;
					firstTh = ptr;
					if (firstTh == NULL) {
						lastTh = NULL;
					}
			}
		
			if (firstTh != NULL && firstTh -> next != NULL) {
				
				ptr = firstTh -> next;
				lem = firstTh;
				while (ptr != NULL) {
					if (!(ptr -> aboveGround())) {
						lem -> next = ptr -> next;
						if (ptr == lastTh) lastTh = lem;
						delete ptr;
						ptr = lem -> next;
					} else {
						lem = ptr;
						ptr = ptr -> next;
					}
				}
			}
			if (firstTh == NULL) {
				lastTh = NULL;
			}
		}
		
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
	
    world.init();
}

float lightdir[4] = {1,1,1,0};

void onDisplay( ) {
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1, 100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	world.cam.set();
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightdir);
	
	setColor( GL_FRONT, GL_AMBIENT, GRAY);
	
	glClearColor(0.53f, 0.808f, 0.922f, 1.0f);	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_TEXTURE_2D); 
	setColor( GL_FRONT, GL_DIFFUSE, WHITE);
	for( int x = -4; x < 4; x++ ) {
		for( int z = -4; z < 4; z++ ) {
			glBindTexture(GL_TEXTURE_2D, tex);

			glBegin(GL_QUADS);			
				glNormal3f(0,1,0);
				glTexCoord2f(0, 0);
				glVertex3f(x * 8,     0, z * 8);
				glTexCoord2f(1, 0);
				glVertex3f((x+1) * 8, 0, z * 8);
				glTexCoord2f(1, 1);
				glVertex3f((x+1) * 8, 0, (z+1) * 8);
				glTexCoord2f(0, 1);
				glVertex3f(x * 8,     0, (z+1) * 8);
			glEnd();
		}
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
	
	
	world.draw();
	
	
	float shadow_mtx[4][4] = {1,                         0,       0,                       0,
		                      -lightdir[0]/lightdir[1],  0,     -lightdir[2]/lightdir[1],  0,
							   0,                        0,      1,                        0,
							   0,                    0.001,      0,                        1};


	glMultMatrixf( &shadow_mtx[0][0] );
	glDisable(GL_LIGHTING);
	glColor3f(0, 0, 0);
	
	world.draw();

	glutSwapBuffers();     				// Buffercsere: rajzolas vege

}


// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
	if (key == ' ') {
		world.bomb.start();
		glutPostRedisplay( );
	}
	if (!world.bomb.started) {
		float UNIT = .5;
		if (key == 'a') {
			world.cam.pos = world.cam.pos - Vector(UNIT,0,0);
			glutPostRedisplay( );
		}
		if (key == 'd') {
			world.cam.pos = world.cam.pos + Vector(UNIT,0,0);
			glutPostRedisplay( );
		}
		if (key == 'w') {
			world.cam.pos = world.cam.pos - Vector(0,0,UNIT);
			glutPostRedisplay( );
		}
		if (key == 'y') {
			world.cam.pos = world.cam.pos + Vector(0,0,UNIT);
			glutPostRedisplay( );
		}
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
	GLOBAL_TIME =glutGet(GLUT_ELAPSED_TIME);
	if (GLOBAL_TIME > world.csgAdded + 1000) world.addCsgUnderBomb();
	glutPostRedisplay( );
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
