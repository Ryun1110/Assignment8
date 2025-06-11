#define _CRT_SECURE_NO_WARNINGS
#include "common.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>

// 외부 전역 변수 및 함수 선언
extern std::vector<Vector3> gPositions;
extern std::vector<Vector3> gNormals;
extern std::vector<Triangle> gTriangles;

extern void load_mesh(std::string fileName);

extern float gTotalTimeElapsed;
extern int   gTotalFrames;
extern void init_timer();
extern void start_timing();
extern float stop_timing();

void display_main() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    start_timing();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.1f, -1.0f, -1.5f);
    glScalef(10.0f, 10.0f, 10.0f);

    glBegin(GL_TRIANGLES);
    for (const auto& tri : gTriangles) {
        for (int i = 0; i < 3; ++i) {
            const Vector3& n = gNormals[tri.indices[i]];
            const Vector3& p = gPositions[tri.indices[i]];
            glNormal3f(n.x, n.y, n.z);
            glVertex3f(p.x, p.y, p.z);
        }
    }
    glEnd();

    float timeElapsed = stop_timing();
    gTotalFrames++;
    gTotalTimeElapsed += timeElapsed;
    float fps = gTotalFrames / gTotalTimeElapsed;

    char title[128];
    sprintf(title, "OpenGL Bunny (Immediate): %.2f FPS", fps);
    glutSetWindowTitle(title);

    glutPostRedisplay();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 1280);
    glutCreateWindow("OpenGL Bunny");

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    GLfloat lightAmbient[] = { 0.f, 0.f, 0.f, 1.f };
    GLfloat lightDiffuse[] = { 1.f, 1.f, 1.f, 1.f };
    GLfloat lightSpecular[] = { 0.f, 0.f, 0.f, 1.f };
    GLfloat lightDir[] = { -1.f, -1.f, -1.f, 0.f };

    float invLen = 1.0f / std::sqrt(lightDir[0] * lightDir[0] + lightDir[1] * lightDir[1] + lightDir[2] * lightDir[2]);
    lightDir[0] *= invLen;
    lightDir[1] *= invLen;
    lightDir[2] *= invLen;

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);

    GLfloat matAmbient[] = { 1.f, 1.f, 1.f, 1.f };
    GLfloat matDiffuse[] = { 1.f, 1.f, 1.f, 1.f };
    GLfloat matSpecular[] = { 0.f, 0.f, 0.f, 1.f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.f);

    glEnable(GL_NORMALIZE);  // 필수: 스케일 때문에

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    load_mesh("bunny.obj");
    init_timer();

    glutDisplayFunc(display_main);
    glutMainLoop();

    return 0;
}
