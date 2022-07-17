///////////////////////////////////
// Youssef Altherwy
///////////////////////////////////

// INCLUDES //

#ifdef __APPLE__
#  include "GL/glew.h"
#  include <OpenGL/glext.h>
#  include "GL/freeglut.h"
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

#include <vector>
#include <math.h>
#include <unistd.h>
#include <fstream>
#include "Blade.h"
#include "TGALoader.h"
#include <time.h>
#include "timer.h"

using namespace std;


static vector<Blade*> blades;
static vector<Vector3f> vertices;
static vector<long> differentSpeeds;
unsigned int texture[2];
int _size = 140;
int _zmax = 10;
int _xmax = 20;
int _ymax = 2;
float divideBy = 10.0;
timer tim(false); // pause = false
long _speed = 100;
long _secondSpeed= 300;
float _maxWidth = 40.0;
float _maxDepth = 2.0; // 1.1


vector<float> randomXGenerator(int _size, float additionFactor);
vector<float> randomZGenerator(int _max, int _size);
vector<float> randomYGenerator(int _max, int _size);
vector<float> generateRandomDepth(int _size);
vector<long> randomSpeedGenerator(int _size);
float cutBlade(float _length , float _depth);
void printList(vector<float> _list);
void drawRectangle();
void texturing(const char* tgaFileName, const char* tgaFileName2);


// generate blades
void bladesGenerator(string mode_, float additionFactor){
    vector<float> _x = randomXGenerator(_size, additionFactor);
    vector<float> _y = randomYGenerator(_ymax, _size);
    vector<float> _z = randomYGenerator(_zmax, _size);
    vector<float> _depth = generateRandomDepth(_size);
    for (int i=0; i < _x.size(); i++) {
        Blade* _blade = new Blade(i);
        //cutBlade(_y[i], _depth[i])
        Point* _startPoint = new Point(_x[i], _depth[i], - _z[i],_x[i],_depth[i], - _z[i]);
        _blade->createBlade(_startPoint,cutBlade(_y[i], _depth[i]), mode_,0,0,0,0);
        blades.push_back(_blade);
    }
    
}

void glDrawBlade(Blade* _blade){
    vector<vector<Vector3f> > single_blade = _blade->drawBlade();
    vector<Vector3f> first_curve = single_blade[0];
    vector<Vector3f> second_curve = single_blade[1];
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_LINE_STRIP);
    // FIRST CURVE
    for (int j = 0; j < first_curve.size(); j += 3) {
        if (j >= first_curve.size()-12) glColor3f(0.00392, 0.6509, 0.0666667); // light green
        glVertex3f(first_curve[j](0), first_curve[j](1), first_curve[j](2));
    }
    // SECOND CURVE
    
    for (int j = 0; j < second_curve.size(); j += 3) {
        if (j >= 12) glColor3f(0.0, 0.3607, 0.0352); // dark green
        glVertex3f(second_curve[j](0), second_curve[j](1), second_curve[j](2));
    }
    glEnd();
    
}


// Drawing routine.
void drawScene(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glTranslatef(0.0, 0.0, -2.0);
    drawRectangle();
    //glColor3f(0.0, 0.3607, 0.0352);
    glColor3f(0.00392, 0.6509, 0.0666667);
    
    glPushMatrix();
    for (int i=0; i < blades.size(); i++) {
        Blade* _blade = blades[i];
       // _blade->motion(_blade->rotation_limits(0.0, 0.05));
        //_blade->sine_waveMotions(glutGet(GLUT_ELAPSED_TIME), _blade->rotation_limits(0.01, 0.0), tim);
        
        if (_blade->start->x <= 2.0) {
        _blade->sine_waveMotions(_speed, _blade->rotation_limits(0.005, 0.0), tim);
        }
        else
        _blade->sine_waveMotions(_secondSpeed, _blade->rotation_limits(0.005, 0.0), tim);
        glDrawBlade(_blade);
    }
    glPopMatrix();
    
    glutPostRedisplay();
    glutSwapBuffers();
}

// Initialization routine.
void setup()
{

    differentSpeeds  = randomSpeedGenerator(_size);
    //bladesGenerator("left", 0.0);
    bladesGenerator("right", 0.5);
    texturing("../doc/blue_sky.tga", "../doc/ground.tga");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(0.0, 1.0, 0.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'q':
            exit(0);
            break;
        case '+':
            _speed += 100;
            glutPostRedisplay();
            break;
        case '-':
            _speed -= 100;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("square.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    
    glewExperimental = GL_TRUE;
    glewInit();
    setup();
    glutMainLoop(); 
}



vector<float> randomXGenerator(int _size, float additionFactor){
    vector<float> _list;
    srand(time(NULL));
    int counter = 0;
    for (int i=0; i <_size ; i++) {
        if (counter >= _maxWidth) {counter  = 0; divideBy +=0.5;}
        float _temp = counter/divideBy;
        _temp += additionFactor;
        _list.push_back(_temp);
        counter++;
    }
    return _list;
}

vector<float> randomZGenerator(int _max, int _size){
    /*vector<float> _list;
    srand(time(NULL));
    for (int i=0; i <_size/2 ; i++) {
        
        float temp = rand() % _max; // generate int number from 0 to max
        _list.push_back(temp); // with no decimal (1,2, .. )
         temp += (rand() % 10 + 1.0)/10.0; // add .number to temp
        _list.push_back(temp);
    }
    return _list;*/
    vector<float> _list;
    srand(time(NULL));
    for (int i=0; i <_size ; i++) {
        
        float temp = (rand() % _max)/10.0; // generate int number from 0 to max
        temp += 1.6;
        _list.push_back(temp); // with no decimal (1,2, .. )
    }
    
    return _list;
    
}

vector<float> randomYGenerator(int _max, int _size){
    vector<float> _list;
    srand(time(NULL));
    for (int i=0; i <_size ; i++) {
        
        float temp = (rand() % _max)/10.0; // generate int number from 0 to max
        temp += 1.6;
        _list.push_back(temp); // with no decimal (1,2, .. )
    }
    
    return _list;
}

vector<float> generateRandomDepth(int _size){
    vector<float> _list;
    float _depth = 0.0;
    srand(time(NULL));
    for (int i=0; i <_size ; i++) {
       /* if (_depth >= 1.0)   _depth = 0.0;  // .25
        _list.push_back(_depth);
        _depth += 0.05;*/
        int temp = _maxDepth * 10;
        _depth = (rand() % temp)/10.0 ;
        _list.push_back(_depth);
    }
    return _list;
}

vector<long> randomSpeedGenerator(int _size){
    vector<long> _list;
    _size /= 40;
    int speed =100;
    for (int i=0; i < _size; i++ ) {
        _list.push_back(speed);
        speed += 200;
    }
    return _list;
}


float cutBlade(float _length , float _depth){
    
    float _temp = 0.0;
    if (_depth < 0.5)
        _temp =  (_length - _depth) + 0.2;
    else
        _temp = (_length - _depth) + 1.1;
    return _temp;
    
}

void printList(vector<float> _list){
    for (int i=0; i< _list.size(); i++) cout << _list[i] << " ";

    cout << endl;
}

void drawRectangle(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    //glColor3f(.564, .490, .247);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0,0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2d(1.0,0.0); glVertex3f(2.0, 0.0, 0.0);
    glTexCoord2d(1.0,1.0); glVertex3f(2.0, 2.0, 0.0);
    glTexCoord2d(0.0,1.0); glVertex3f(0.0, 2.0, 0.0);
    glEnd();
    
    
    // Ground
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0,0.0); glVertex3f(0.0, -0.03, 0.0);
    glTexCoord2d(1.0,0.0); glVertex3f(2.0, -0.03, 0.0);
    glTexCoord2d(1.0,0.4); glVertex3f(2.0, 0.6, 0.0); // .1
    glTexCoord2d(0.0,0.4); glVertex3f(0.0, 0.6, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void texturing(const char* tgaFileName, const char* tgaFileName2){
    /// TGA LOADER
    glGenTextures(2, texture);
     NS_TGALOADER::IMAGE tga;
    tga.LoadTGA(tgaFileName);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    gluBuild2DMipmaps (GL_TEXTURE_2D, 4, tga.getWidth (), tga.getHeight (), GL_BGRA, GL_UNSIGNED_BYTE, tga.getDataForOpenGL  ());
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, Pixels.data());
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_NEAREST);
    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    tga.LoadTGA(tgaFileName2);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    gluBuild2DMipmaps (GL_TEXTURE_2D, 4, tga.getWidth (), tga.getHeight (), GL_BGRA, GL_UNSIGNED_BYTE, tga.getDataForOpenGL  ());
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, Pixels.data());
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_NEAREST);
    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    
}


