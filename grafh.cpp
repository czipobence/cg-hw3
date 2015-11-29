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

void glVertex3f(const Vector& v) {
  glVertex3f(v.x, v.y, v.z);
}

void glNormal3f(const Vector& v) {
  glNormal3f(v.x, v.y, v.z);
}

void glMaterialfv(GLenum face, GLenum p, Color c) {
	float arr[4] = {c.r,c.g,c.b,1};
	glMaterialfv(face,p,arr);
}

void glTranslatef(Vector v) {
	glTranslatef(v.x,v.y,v.z);
}
void glRotatef(float angle, Vector v) {
	glRotatef(angle,v.x,v.y,v.z);
}
void glScalef(Vector v) {
	glScalef(v.x,v.y,v.z);
}

struct Drawable {
	
	Vector p;
	
	Drawable(Vector p): p(p) {}
	
	virtual void setProperties() {}
	virtual void drawItem() = 0;
	
	void draw() {
		glPushMatrix();
		glTranslatef(p);
		
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
				glMaterialfv( GL_FRONT, GL_DIFFUSE, (x ^ z) & 1 ? WHITE : GRAY);
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
	
	virtual void setProperties() {
		glMaterialfv(GL_FRONT,GL_AMBIENT, kd*.3);
        glMaterialfv(GL_FRONT,GL_DIFFUSE, kd);
        //glMaterialfv(GL_FRONT,GL_SPECULAR, ks);
        //glMaterialf (GL_FRONT,GL_SHININESS, 50);
	}
	
	virtual ~ ColoredDrawable() {}
	
};

struct BezierCurve : ColoredDrawable {
    Vector cps[10];
    int num;

	BezierCurve(Vector p, Color c=Color(0,1,0)): ColoredDrawable(p,c) {
		num = 0;
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

	Vector r(float t) {
		Vector rr(0,0,0);
		for(int i = 0; i < num; i++)
			rr = rr + cps[i] * B(num-1,i,t);
      	return rr;
	}
   
	//http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/Bezier/bezier-der.html
	Vector der(float t) {
		Vector rr(0,0,0);
		for(int i = 0; i < num-1; i++)
			rr = rr + (cps[i+1] - cps[i]) * B(num-2,i,t);
      	return rr;
	}
   
	void drawItem() {
	
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(kd.r, kd.g, kd.b);
	
	glBegin(GL_LINE_STRIP);
	
	for (float t = 0; t < 1; t+= 0.01) {
		glVertex3f(r(t));
	}
	
	glEnd();
	
	glBegin(GL_LINE_STRIP);
	
	for (int i = 0; i<num;i++) {
		glVertex3f(cps[i]);
	}
	
	glEnd();
	
	
	glDisable(GL_COLOR_MATERIAL);   
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
	Vector lp, lv;
	
	CatmullRom(Vector sp, Vector sv): lp(sp), lv(sv) {n = 0;}
	
	void addSpline(Vector p, Vector v) {
		if (n < 10) {
			splines[n] = Hermite(lp,p,lv,v,n,n+1);
			lp = p;
			lv = v;
			n++;
		}
	}
	
	Vector getVal(float t) {
		return splines[(int)(floor(t))].getVal(t);
	}
	
	Vector getDer(float t) {
		return splines[(int)(floor(t))].getDer(t);
	}
	 
};

struct UVDrawable : public ColoredDrawable {
	float uMin,uMax,du,vMin,vMax,dv;
	
	UVDrawable(	Vector _p,
				float uMin=0, float uMax = 2*M_PI, int nu = 30,
				float vMin=0, float vMax = 1*M_PI, int nv = 30,
				Color _c = Color(0,0,0)):
				ColoredDrawable(_p,_c),uMin(uMin),uMax(uMax),vMin(vMin),vMax(vMax) {
					du = (uMax - uMin) / (float)nu;
					dv = (vMax - vMin) / (float)nv;
				}

	virtual Vector getVal(float u, float v) = 0;
	virtual Vector getNorm(float u, float v) = 0;
	

	void putPoint(float u, float v) {
		glNormal3f(getNorm(u,v));
		glVertex3f(getVal(u,v));		
	}

	void drawItem (){
        glBegin(GL_QUADS);
		
		for (float u = uMin; u <uMax + EPSILON ;u += du) {
			for (float v = vMin; v < vMax+EPSILON; v+= dv) {
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

struct Cone: public ColoredDrawable {
	float r,h;
	
	Cone(Vector p, float h = 1, float r=1, Color c = Color(.9,.6,.2)):
	ColoredDrawable(p,c), r(r), h(h) {}
	
	virtual void drawItem() {
		float dt = 2 * M_PI / 30.0f;
		
		glBegin(GL_TRIANGLE_FAN);
		
		glNormal3f(0,1,0);
		glVertex3f(0,h,0);
		
		for (float t = 0; t < 2.0*M_PI; t += dt) {
			glNormal3f(cos(t),0,sin(t));
			glVertex3f(r*cos(t), 0, r * sin(t));
		}
		
		glEnd();
		
		glBegin(GL_TRIANGLE_FAN);
		
		glNormal3f(0,-1,0);
		glVertex3f(0,0,0);
		
		for (float t = 0; t < 2.0*M_PI + EPSILON ; t += dt) {
			glVertex3f(r*sin(t), 0, r * cos(t));
		}
		
		glEnd();
		
	}
	
};

struct Cylinder: public ColoredDrawable {
	float r,h;
	
	Cylinder(Vector p, float h = 1, float r=1, Color c = Color(.9,.6,.2)):
	ColoredDrawable(p,c), r(r), h(h) {}
	
	virtual void drawItem() {
		float dt = 2 * M_PI / 30.0f;
		
		glBegin(GL_TRIANGLE_STRIP);
		
		
		for (float t = 0; t < 2.0*M_PI; t += dt) {
			glNormal3f(sin(t),0,cos(t));
			glVertex3f(r*sin(t), 0, r * cos(t));
			glVertex3f(r*sin(t), h, r * cos(t));
		}
		
		glEnd();
		
		glBegin(GL_TRIANGLE_FAN);
		
		glNormal3f(0,1,0);
		glVertex3f(0,h,0);
		
		for (float t = 0; t < 2.0*M_PI + EPSILON ; t += dt) {
			glVertex3f(r*cos(t), h, r * sin(t));
		}
		
		glEnd();
		
		glBegin(GL_TRIANGLE_FAN);
		
		glNormal3f(0,-1,0);
		glVertex3f(0,0,0);
		
		for (float t = 0; t < 2.0*M_PI + EPSILON ; t += dt) {
			glVertex3f(r*sin(t), 0, r * cos(t));
		}
		
		glEnd();
		
		
	}
	
};


struct Sphere: public UVDrawable {
	float r;
	
	Sphere(Vector center, float radius = 1, Color c = Color(.9,.9,.6)):
	UVDrawable(center,0,2*M_PI,40,0,M_PI,20,c),r(radius) {} 
	
	Vector getVal(float u, float v) {
		return Vector (
			(float)(r * cos(u) * sin (v)),
			(float)(r * sin(u) * sin (v)),
			(float)(r * cos(v))
		);
	}
	
	Vector getNorm(float u, float v) {return (getVal(u,v)).norm();}
	
};

struct Csirguru: public Drawable {
	static const float HEAD_POS_X = 0.0f;
	static const float HEAD_POS_Y = 0.0f;
	static const float HEAD_POS_Z = 0.0f;
	static const float HEAD_RADIUS = 1.0f;
	Sphere head;
	
	Csirguru (Vector middle): Drawable(middle), 
					head(p+Vector(HEAD_POS_X,HEAD_POS_Y,HEAD_POS_Z),HEAD_RADIUS) {}
	
	void draw() {
		head.draw();
	}
};

////////////////////////////////////////////////////////////////////////

struct Camera {
	Vector pos,dir,up, eye,right;
	
	
	Camera(	Vector pos=Vector(-0,1,-4), 
			Vector dir=Vector(0,0,4), 
			Vector up=Vector(0,1,0)):
			pos(pos), dir(dir), up(up) {
				fit();
			} 
			
	void fit() {
		eye = pos + dir;		
		this -> right = (dir % up).norm();
		this -> up = (this-> right % this->dir).norm();
	}
	
	void set() {
		gluLookAt(pos.x,pos.y,pos.z,eye.x,eye.y,eye.z,up.x,up.y,up.z);
	}
};

////////////////////////////////////////////////////////////////////////

struct World {
	Camera cam;
};

World world;

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
	
	glMaterialfv( GL_FRONT, GL_AMBIENT, GRAY);
	//glMaterialfv( GL_FRONT, GL_SPECULAR, BLACK);	
	
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	
	glBegin( GL_QUADS );
	for( int x = -10; x < 10; x++ ) {
		for( int z = -10; z < 10; z++ ) {
			glMaterialfv( GL_FRONT, GL_DIFFUSE, (x ^ z) & 1 ? WHITE : GRAY);
			glNormal3f(0,1,0);
			glVertex3f(x * 1,     0, z * 1);
			glVertex3f((x+1) * 1, 0, z * 1);
			glVertex3f((x+1) * 1, 0, (z+1) * 1);
			glVertex3f(x * 1,     0, (z+1) * 1);
		}
	}
	glEnd( );

	

	Sphere a (Vector(0,1,0),1);
	a.draw();
	Cone b (Vector(2,3,0), 1, 1);
	b.draw();
	//Cylinder c (Vector(-2,5,0), 1, 1);
	//c.draw();
	
	BezierCurve d (Vector(-2,0,0));
	d.addPoint(Vector(0,0,0));
	d.addPoint(Vector(0,0,1));
	d.addPoint(Vector(0,1,6));
	d.addPoint(Vector(0,2,1));
	d.addPoint(Vector(0,6,0));
	d.addPoint(Vector(0,2,-1));
	d.addPoint(Vector(0,0,-1));
	d.addPoint(Vector(0,0,0));
	d.draw();

	float shadow_mtx[4][4] = {1,                         0,       0,                       0,
		                      -lightdir[0]/lightdir[1],  0,     -lightdir[2]/lightdir[1],  0,
							   0,                        0,      1,                        0,
							   0,                    0.001,      0,                        1};


	glMultMatrixf( &shadow_mtx[0][0] );
	glDisable(GL_LIGHTING);
	glColor3f(0, 0, 0);
	
	

	a.draw();

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
