#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

int polygons = 1;
int sun_theta = 0;
int camera_theta = 0;
float light = 0.3;
float vec3[3];
float diffuse[3];
float specular[3];
float ambient[3];
float emission[4];
float pos[4];
float direction[3];

void initialize()
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(400,50);
    glutInitWindowSize(700,700);
    glutCreateWindow ("House of the Rising Sun");
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-60, 60, -60, 60,-150,150);
    gluLookAt(0,40,70,0,0,0,0,1,0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

}

GLfloat tetrahedron[4][3] = {
        {0, 0, 1},
        {0, 0.942809, -0.33333},
        {-0.816497, -0.471405, -0.333333},
        {0.816497, -0.471405, -0.333333}
};

void normalize(float* v)
{
    float d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

void crossProduct(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float cp[])
{
    float v1[3], v2[3];
    v1[0] = x1 - x2; v1[1] = y1 - y2; v1[2] = z1 - z2;
    v2[0] = x3 - x2; v2[1] = y3 - y2; v2[2] = z3 - z2;
    cp[0] = v1[1] * v2[2] - v1[2] * v2[1];
    cp[1] = v1[2] * v2[0] - v1[0] * v2[2];
    cp[2] = v1[0] * v2[1] - v1[1] * v2[0];
    normalize(cp);
}

void recursive_subdivision(float a[], float b[], float c[], int step)
{
    if (step > 0) {
        float ab[3], ac[3], bc[3];
        for (int i = 0; i < 3; i++){
            ab[i] = a[i] + b[i];
            ac[i] = a[i] + c[i];
            bc[i] = b[i] + c[i];
        }
        normalize(ab);
        normalize(ac);
        normalize(bc);

        recursive_subdivision(a, ab, ac, step - 1);
        recursive_subdivision(b, bc, ab, step - 1);
        recursive_subdivision(c, ac, bc, step - 1);
        recursive_subdivision(ab, bc, ac, step - 1);
    }
    else {
        glBegin(GL_LINE_LOOP);
        glVertex3fv(a);
        glVertex3fv(b);
        glVertex3fv(c);
        glEnd();
    }
}


void display()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glRotatef(camera_theta,0,1,0);

    glPushMatrix();
    glRotatef(-sun_theta,0,0,1);
    glTranslatef(-50,0,0);

    // setting light source properties
    pos[0] = 0; pos[1] = 0; pos[2] = 0; pos[3] = 1;
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    diffuse[0] = light; diffuse[1] = light; diffuse[2] = light;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    specular[0] = light; specular[1] = light; specular[2] = light;
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    //Setting sphere properties
    diffuse[0] = 0; diffuse[1] = 0; diffuse[2] = 0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,diffuse);

    specular[0] = 0; specular[1] = 0; specular[2] = 0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,specular);

    ambient[0] = 0; ambient[1] = 0; ambient[2] = 0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,ambient);

    emission[0] = 1; emission[1] = 1; emission[2] = 0; emission[3] = 1;
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);

    recursive_subdivision(tetrahedron[0], tetrahedron[2], tetrahedron[1], 4);
    recursive_subdivision(tetrahedron[0], tetrahedron[3], tetrahedron[2], 4);
    recursive_subdivision(tetrahedron[0], tetrahedron[1], tetrahedron[3], 4);
    recursive_subdivision(tetrahedron[1], tetrahedron[2], tetrahedron[3], 4);

    glPopMatrix();

    //Resetting emission to black
    emission[0] = 0; emission[1] = 0; emission[2] = 0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);

    //Spotlight
    pos[0] = 0; pos[1] = 30; pos[2] = -10; pos[3] = 1;
    glLightfv(GL_LIGHT1, GL_POSITION, pos);

    direction[0] = 0; direction[1] = -3; direction[2] = 1;
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,direction);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30);

    diffuse[0] = 1; diffuse[1] = 1; diffuse[2] = 1;
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);

    specular[0] = 1; specular[1] = 1; specular[2] = 1;
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);


    //Creating grass
    diffuse[0] = 0; diffuse[1] = 1; diffuse[2] = 0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,diffuse);

    specular[0] = 0; specular[1] = 0; specular[2] = 0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,specular);

    ambient[0] = 0; ambient[1] = 0.2; ambient[2] = 0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,ambient);

    crossProduct(50, 0, -50, 50, 0, 50, -50, 0, 50, vec3);
    glNormal3fv(vec3);
    if (polygons == 1){
        glBegin(GL_POLYGON);
        glVertex3f(50, 0, -50);
        glVertex3f(50, 0, 50);
        glVertex3f(-50, 0, 50);
        glVertex3f(-50, 0, -50);
        glEnd();
    }
    else {
        int i,j;
        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j ++) {
                glBegin(GL_POLYGON);
                glVertex3f(-50 + 10 * i, 0, -50 + 10 * j);
                glVertex3f(-50 + 10 * i, 0, -40 + 10 * j);
                glVertex3f(-40 + 10 * i, 0, -40 + 10 * j);
                glVertex3f(-40 + 10 * i, 0, -50 + 10 * j);
                glEnd();
            }
    }

    glTranslatef(0,0,-10);

    //Setting walls properties
    diffuse[0] = 1; diffuse[1] = 0; diffuse[2] = 0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,diffuse);

    specular[0] = 0; specular[1] = 0; specular[2] = 0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,specular);

    ambient[0] = 0.3; ambient[1] = 0; ambient[2] = 0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,ambient);

    //Front side
    glBegin(GL_POLYGON);
    crossProduct(-5, 0, 0, -5, 10, 0, 5, 10, 0, vec3);
    vec3[0] = 0; vec3[1] = 0; vec3[2] = 1;
    glNormal3fv(vec3);
    glVertex3f(-5, 0, 0);
    glVertex3f(-5, 10, 0);
    glVertex3f(5, 10, 0);
    glVertex3f(5, 0, 0);
    glEnd();

    //Back side
    glBegin(GL_POLYGON);
    vec3[0] *= -1; vec3[1] *= -1; vec3[2] *= -1;
    glNormal3fv(vec3);
    glVertex3f(-5, 0, -20);
    glVertex3f(-5, 10, -20);
    glVertex3f(5, 10, -20);
    glVertex3f(5, 0, -20);
    glEnd();

    //Left side
    glBegin(GL_POLYGON);
    crossProduct(-5, 0, 0, -5, 0, -20, -5, 10, -20, vec3);
    glNormal3fv(vec3);
    glVertex3f(-5, 0, 0);
    glVertex3f(-5, 0, -20);
    glVertex3f(-5, 10, -20);
    glVertex3f(-5, 10, 0);
    glEnd();

    //Right side
    glBegin(GL_POLYGON);
    vec3[0] *= -1; vec3[1] *= -1; vec3[2] *= -1;
    glNormal3fv(vec3);
    glVertex3f(5, 0, 0);
    glVertex3f(5, 0, -20);
    glVertex3f(5, 10, -20);
    glVertex3f(5, 10, 0);
    glEnd();

    //Bottom side
    crossProduct(5, 0, 0, 5, 0, -20, -5, 0, -20, vec3);
    glNormal3fv(vec3);
    glBegin(GL_POLYGON);
    glVertex3f(5, 0, 0);
    glVertex3f(5, 0, -20);
    glVertex3f(-5, 0, -20);
    glVertex3f(-5, 0, 0);
    glEnd();

    //Top side
    vec3[0] *= -1; vec3[1] *= -1; vec3[2] *= -1;
    glNormal3fv(vec3);
    glBegin(GL_POLYGON);
    glVertex3f(5, 10, 0);
    glVertex3f(5, 10, -20);
    glVertex3f(-5, 10, -20);
    glVertex3f(-5, 10, 0);
    glEnd();

    //Setting roof properties
    diffuse[0] = 0.7; diffuse[1] = 0.7; diffuse[2] = 0.7;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,diffuse);

    specular[0] = 0.7; specular[1] = 0.7; specular[2] = 0.7;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,specular);

    ambient[0] = 0.2; ambient[1] = 0.2; ambient[2] = 0.2;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,ambient);

    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);

    //Front roof side
    glBegin(GL_POLYGON);
    crossProduct(-5, 10, 0, 0, 20, 0, 5, 10, 0, vec3);
    glNormal3fv(vec3);
    glVertex3f(-5, 10, 0);
    glVertex3f(0, 20, 0);
    glVertex3f(5, 10, 0);
    glEnd();

    //Back roof side
    glBegin(GL_POLYGON);
    vec3[0] *= -1; vec3[1] *= -1; vec3[2] *= -1;
    glNormal3fv(vec3);
    glVertex3f(5, 10, -20);
    glVertex3f(0, 20, -20);
    glVertex3f(-5, 10, -20);
    glEnd();

    //Left roof side
    glBegin(GL_POLYGON);
    crossProduct(-5, 10, 0, -5, 10, -20, 0, 20, -20, vec3);
    glNormal3fv(vec3);
    glVertex3f(-5, 10, 0);
    glVertex3f(-5, 10, -20);
    glVertex3f(0, 20, -20);
    glVertex3f(0, 20, 0);
    glEnd();

    //Right roof side
    glBegin(GL_POLYGON);
    vec3[0] *= -1; vec3[1] *= -1; vec3[2] *= -1;
    glNormal3fv(vec3);
    glVertex3f(5, 10, 0);
    glVertex3f(5, 10, -20);
    glVertex3f(0, 20, -20);
    glVertex3f(0, 20, 0);
    glEnd();

    glFlush();
    glutSwapBuffers();
}

void idle()
{
    sun_theta += 1;
    if (sun_theta == 180)
        sun_theta = 0;
    if (sun_theta <= 90){
        light += 0.0077;
        if (light > 1)
            light = 1;
    }
    else{
        light -= 0.0077;
        if (light < 0.3)
            light = 0.3;
    }
    glutPostRedisplay();
}

void keyboard(int key, int x, int y){
    if (key == GLUT_KEY_LEFT)
        camera_theta += 1;
    else if (key == GLUT_KEY_RIGHT)
        camera_theta -= 1;
}

void menu(int id){
    if (id == 1)
        exit(0);
}

void grass_menu(int id){
    if (id == 1)
        polygons = 1;
    if (id == 2)
        polygons = 100;
}

void spotlight_menu(int id){
    if (id == 1)
        glEnable(GL_LIGHT1);
    if (id == 2)
        glDisable(GL_LIGHT1);
}

void shading_menu(int id){
    if (id == 1)
        glShadeModel(GL_SMOOTH);
    if (id == 2)
        glShadeModel(GL_FLAT);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    initialize();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutSpecialFunc(keyboard);

    int grass_sub_menu = glutCreateMenu(grass_menu);
    glutAddMenuEntry("1",1);
    glutAddMenuEntry("100",2);
    int spotlight_sub_menu = glutCreateMenu(spotlight_menu);
    glutAddMenuEntry("On",1);
    glutAddMenuEntry("Off",2);
    int shading_sub_menu = glutCreateMenu(shading_menu);
    glutAddMenuEntry("Smooth",1);
    glutAddMenuEntry("Flat",2);
    glutCreateMenu(menu);
    glutAddSubMenu("Grass Polygons", grass_sub_menu);
    glutAddSubMenu("Spotlight", spotlight_sub_menu);
    glutAddSubMenu("Shading", shading_sub_menu);
    glutAddMenuEntry("Quit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();

    return 0;
}
