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

const int SCREEN_WIDTH = 600;	// alkalmazás ablak felbontása
const int SCREEN_HEIGHT = 600;


Color image[SCREEN_WIDTH*SCREEN_HEIGHT];	// egy alkalmazás ablaknyi kép

void glVertex3f(const Vector& v) {
  glVertex3f(v.x, v.y, v.z);
}

void glMaterialfv(GLenum face, GLenum p, Color c) {
	float arr[4] = {c.r,c.g,c.b,1};
	glMaterialfv(face,p,arr);
}

struct Drawable {
	virtual void draw() = 0;
	virtual ~Drawable() {};
};

struct Plain : public Drawable {
	Vector p, n;
	Plain (const Vector& _p, const Vector & _n) : p(_p), n(_n) {}
	void draw() {
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

struct UVDrawable : public Drawable {
	float uMin,uMax,du,vMin,vMax,dv;
	
	UVDrawable(	float uMin=0, float uMax = 1*M_PI, int nu = 30,
				float vMin=0, float vMax = 2*M_PI, int nv = 30):
				uMin(uMin),uMax(uMax),vMin(vMin),vMax(vMax) {
					du = (uMax - uMin) / (float)nu;
					dv = (vMax - vMin) / (float)nv;
				}

	virtual Vector getVal(float u, float v) = 0;
	virtual Vector getNorm(float u, float v) = 0;

	void draw (){
		for (float u = uMin; u <uMax;u += du) {
			for (float v = vMin; v < vMax; v+= dv) {
				//DO SG
			}
		}
	}
	
	virtual ~UVDrawable(){}

};

struct Sphere: public UVDrawable {
	Vector p;
	float r;
	
	Sphere(Vector center, float radius = 1): p(center),r(radius) {} 
	
	Vector getVal(float u, float v) {
		return Vector (
			(float)(r * cos(u) * sin (v)),
			(float)(r * sin(u) * sin (v)),
			(float)(r * cos(v))
		);
	}
	
	Vector getNorm(float u, float v) {return (getVal(u,v) - p).norm();}
	
};

struct Csirguru: public Drawable {
	static const float HEAD_POS_X = 0.0f;
	static const float HEAD_POS_Y = 0.0f;
	static const float HEAD_POS_Z = 0.0f;
	static const float HEAD_RADIUS = 1.0f;
	Vector p;
	Sphere head;
	
	Csirguru (Vector middle): p(middle), 
					head(p+Vector(HEAD_POS_X,HEAD_POS_Y,HEAD_POS_Z),HEAD_RADIUS) {}
	
	void draw() {
		head.draw();
	}
};

////////////////////////////////////////////////////////////////////////

struct Camera {
	Vector pos,dir,up, eye;
	
	
	Camera(	Vector pos=Vector(-3,2,-2), 
			Vector dir=Vector(3,-2,2), 
			Vector up=Vector(0,1,0)):
			pos(pos), dir(dir), up(up) {
				eye = pos + dir;
			} 
	
	void set() {
		gluLookAt(pos.x,pos.y,pos.z,eye.x,eye.y,eye.z,up.x,up.y,up.z);
	}
};

void glQuad(const Vector& a, const Vector& b, const Vector& c, const Vector& d) {
  Vector normal = ((c-a) % (b-a)).norm();
  glMaterialfv( GL_FRONT, GL_DIFFUSE,Color(fabs(normal.x),fabs(normal.y),fabs(normal.z)));
  glNormal3f(normal.x, normal.y, normal.z);
  glVertex3f(a); glVertex3f(b); glVertex3f(c); glVertex3f(d);
}

void drawCube(const Vector& size) {
	
	glPushMatrix();
	glTranslatef(4,.5,0);
	
  glBegin(GL_QUADS); {
    /*       (E)-----(A)
             /|      /|
            / |     / |
          (F)-----(B) |
           | (H)---|-(D)
           | /     | /
           |/      |/
          (G)-----(C)        */

    Vector s = size / 2;

    Vector A(+s.x, +s.y, -s.z), B(+s.x, +s.y, +s.z), C(+s.x, -s.y, +s.z), D(+s.x, -s.y, -s.z), 
           E(-s.x, +s.y, -s.z), F(-s.x, +s.y, +s.z), G(-s.x, -s.y, +s.z), H(-s.x, -s.y, -s.z);

    glQuad(A, B, C, D); glQuad(E, H, G, F); glQuad(A, E, F, B);
    glQuad(D, C, G, H); glQuad(B, F, G, C); glQuad(A, D, H, E);
    
    glQuad(A, D, C, B); glQuad(E, F, G, H); glQuad(A, B, F, E);
    glQuad(D, H, G, C); glQuad(B, C, G, F); glQuad(A, E, H, D);

  } glEnd();
  glPopMatrix();
}

////////////////////////////////////////////////////////////////////////

struct World {
	Camera cam;
};

World world;

void onInitialization( ) { 
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_LIGHT0);
	float Ia[4] = {0.1, 0.1, 0.1, 1}, Id[4] = {1.0, 1.0, 1.0, 1}, Is[4] = {2, 2, 2, 1};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ia);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Id);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Is);
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1, 100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	world.cam.set();
	//gluLookAt(-3, 2, -2, 0, 0, 0, 0, 1, 0);
	
	float p[4] = {-1, 1, -1, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, p);

	
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	
	glBegin( GL_QUADS );
	for( int x = -10; x < 10; x++ ) {
		for( int z = -10; z < 10; z++ ) {
			glMaterialfv( GL_FRONT, GL_DIFFUSE, (x ^ z) & 1 ? WHITE : GRAY);
			glVertex3f(x * 1,     0, z * 1);
			glVertex3f((x+1) * 1, 0, z * 1);
			glVertex3f((x+1) * 1, 0, (z+1) * 1);
			glVertex3f(x * 1,     0, (z+1) * 1);
		}
	}
	glEnd( );

	
	drawCube(Vector(1,1,1));

	

	float lightdir[3] = {1,1,1};

	float shadow_mtx[4][4] = {1,                         0,       0,                       0,
		                      -lightdir[0]/lightdir[1],  0,     -lightdir[2]/lightdir[1],  0,
							   0,                        0,      1,                        0,
							   0,                    0.001,      0,                        1};


	glMultMatrixf( &shadow_mtx[0][0] );
	glDisable(GL_LIGHTING);
	glColor3f(0, 0, 0);
	
	
	
	drawCube(Vector(1,1,1));

	glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
	if (key == 'a') glutPostRedisplay( ); //MOVE LEFT 
    if (key == 'w') glutPostRedisplay( ); //MOVE UP
    if (key == 'd') glutPostRedisplay( ); //MOVE RIGHT
    if (key == 'y') glutPostRedisplay( ); //MOVE DOWN
    if (key == ' ') glutPostRedisplay( ); //THROW BOMB
    
   /*float UNIT = .5;
	if (key == 'a') {
		camPos = camPos - world.cam.right * UNIT;
		glutPostRedisplay( );
	}
	if (key == 'd') {
		camPos = camPos + world.cam.right * UNIT;
		glutPostRedisplay( );
	}
	if (key == 'w') {
		camPos = camPos + world.cam.dir * UNIT;
		glutPostRedisplay( );
	}
	if (key == 's') {
		camPos = camPos - world.cam.dir * UNIT;
		glutPostRedisplay( );
	}
	if (key == 'r') {
		camPos = camPos + world.cam.up * UNIT;
		glutPostRedisplay( );
	}
	if (key == 'f') {
		camPos = camPos - world.cam.up * UNIT;
		glutPostRedisplay( );
	}
	if (key == '6') {
		camFwd = (camFwd % camUp + camFwd * 3) / 4; 
		glutPostRedisplay( );
	}
	if (key == '4') {
		camFwd = (camUp % camFwd + camFwd * 3) / 4; 
		glutPostRedisplay( );
	}
	if (key == '8') {
		camFwd = (world.cam.right % world.cam.dir + world.cam.dir*3) / 4; 
		camUp = world.cam.right % world.cam.dir;
		glutPostRedisplay( );
	}
	if (key == '2') {
		camFwd = (world.cam.dir % world.cam.right + world.cam.dir*3) / 4; 
		camUp = world.cam.right % world.cam.dir;
		glutPostRedisplay( );
	}
	*/
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
