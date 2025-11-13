#include <GL/glut.h>
#include <cmath>

float hingeAngle = 0.0f;
float targetAngle = 0.0f;
bool animating = false;
int lastMouseX = 0, lastMouseY = 0;
bool rotating = false;
float camAzimuth = 30.0f, camElevation = 20.0f;
float camDistance = 6.0f;

void drawBox(float w, float h, float d) {
    float hw = w*0.5f, hh = h*0.5f, hd = d*0.5f;
    glBegin(GL_QUADS);
    glNormal3f(0,0,1); glVertex3f(-hw,-hh, hd); glVertex3f( hw,-hh, hd); glVertex3f( hw, hh, hd); glVertex3f(-hw, hh, hd);
    glNormal3f(0,0,-1); glVertex3f(-hw,-hh,-hd); glVertex3f(-hw, hh,-hd); glVertex3f( hw, hh,-hd); glVertex3f( hw,-hh,-hd);
    glNormal3f(-1,0,0); glVertex3f(-hw,-hh,-hd); glVertex3f(-hw,-hh, hd); glVertex3f(-hw, hh, hd); glVertex3f(-hw, hh,-hd);
    glNormal3f(1,0,0); glVertex3f(hw,-hh,-hd); glVertex3f(hw, hh,-hd); glVertex3f(hw, hh, hd); glVertex3f(hw,-hh, hd);
    glNormal3f(0,1,0); glVertex3f(-hw, hh,-hd); glVertex3f(-hw, hh, hd); glVertex3f( hw, hh, hd); glVertex3f( hw, hh,-hd);
    glNormal3f(0,-1,0); glVertex3f(-hw,-hh,-hd); glVertex3f( hw,-hh,-hd); glVertex3f( hw,-hh, hd); glVertex3f(-hw,-hh, hd);
    glEnd();
}

void drawWindowModel() {
    glPushMatrix();
    glScalef(2.6f, 2.2f, 0.18f);
    GLfloat ambient[] = {0.0f,0.5f,0.2f,1.0f};
    GLfloat diffuse[] = {0.1f,0.2f,0.7f,1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    drawBox(1.0f,1.0f,1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.1f); 
    glScalef(2.5f, 0.12f, 0.02f); 
    GLfloat amb2[] = {0.18f,0.1f,0.05f,1.0f};
    GLfloat dif2[] = {0.4f,0.25f,0.14f,1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif2);
    drawBox(1.0f,1.0f,1.0f);
    glPopMatrix();
    GLfloat ambG[] = {0.05f,0.15f,0.18f,0.6f};
    GLfloat difG[] = {0.1f,0.4f,0.5f,0.6f};

    glPushMatrix();
    glTranslatef(0.5f, -0.06f, 0.1f); 
    glRotatef(-hingeAngle, 1, 0, 0); 
    glTranslatef(0.0f, -0.52f, 0.0f); 
    glScalef(2.5f, 1.04f, 0.02f); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambG);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difG);
    drawBox(1.0f,1.0f,1.0f);
    glDisable(GL_BLEND);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.06f, 0.1f); 
    glRotatef(-hingeAngle, 0, 0, 0); 
    glTranslatef(0.0f, -1.0f, 0.06f); 
    glScalef(0.06f, 0.06f, 0.06f); 
    GLfloat ambH[] = {0.2f,0.2f,0.2f,1.0f};
    GLfloat difH[] = {0.6f,0.6f,0.6f,1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difH);
    drawBox(1.0f,1.0f,1.0f);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    float az = camAzimuth * (M_PI/180.0f), el = camElevation * (M_PI/180.0f);
    float cx = camDistance * cosf(el) * sinf(az);
    float cy = camDistance * sinf(el);
    float cz = camDistance * cosf(el) * cosf(az);
    gluLookAt(cx, cy, cz, 0,0,0, 0,1,0);
    glPushMatrix();
    glTranslatef(0.0f, -1.1f, 0.0f);
    glScalef(6.0f, 0.02f, 6.0f);
    GLfloat ambGd[] = {0.12f,0.12f,0.12f,1.0f};
    GLfloat difGd[] = {0.25f,0.25f,0.25f,1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambGd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difGd);
    drawBox(1.0f,1.0f,1.0f);
    glPopMatrix();
    drawWindowModel();
    glutSwapBuffers();
}

void idleFunc() {
    if (animating) {
        float delta = targetAngle - hingeAngle;
        float step = 2.5f;
        if (fabs(delta) < 0.1f) { hingeAngle = targetAngle; animating = false; }
        else {
            hingeAngle += (delta > 0 ? 1 : -1) * step;
            if ((delta > 0 && hingeAngle > targetAngle) || (delta < 0 && hingeAngle < targetAngle)) hingeAngle = targetAngle;
        }
        glutPostRedisplay();
    }
}

void reshape(int w, int h) {
    if (h==0) h=1;
    float ar = (float)w/(float)h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, ar, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key=='w' || key=='W') { targetAngle = 75.0f; animating = true; }
    if (key=='s' || key=='S') { targetAngle = 0.0f; animating = true; }
    if (key=='a' || key=='A') { targetAngle = (hingeAngle < 45.0f) ? 75.0f : 0.0f; animating = true; }
    if (key=='q' || key=='Q') exit(0);
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) { targetAngle = (hingeAngle < 45.0f) ? 75.0f : 0.0f; animating = true; }
    if (button==GLUT_RIGHT_BUTTON) {
        if (state==GLUT_DOWN) { rotating = true; lastMouseX = x; lastMouseY = y; }
        else rotating = false;
    }
}

void motion(int x, int y) {
    if (rotating) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        camAzimuth += dx * 0.5f;
        camElevation += dy * 0.3f;
        if (camElevation > 89.0f) camElevation = 89.0f;
        if (camElevation < -10.0f) camElevation = -10.0f;
        lastMouseX = x; lastMouseY = y;
        glutPostRedisplay();
    }
}

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightPos[] = {5.0f, 8.0f, 6.0f, 1.0f};
    GLfloat lightDiff[] = {0.9f,0.9f,0.9f,1.0f};
    GLfloat lightAmb[] = {0.2f,0.2f,0.2f,1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glClearColor(0.75f, 0.85f, 0.95f, 1.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(900,600);
    glutCreateWindow("3D Jendela (FreeGLUT) - Tekan O/C/T atau klik kiri");
    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idleFunc);
    glutMainLoop();
    return 0;
}