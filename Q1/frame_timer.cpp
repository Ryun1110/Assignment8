/*
    How to use this code:

    Call init_timer before starting rendering, i.e., before calling
    glutMainLoop. Then, make sure your display function is organized
    roughly as the example below.
*/
#ifdef _MSC_VER          // MSVC에서만
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <float.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

struct Vector3 { float x, y, z; };   // load_mesh.cpp와 완전히 같은 정의
struct Triangle { unsigned int indices[3]; };

extern std::vector<Vector3>   gPositions;   // 🔸 실제 메모리는 load_mesh.cpp
extern std::vector<Vector3>   gNormals;
extern std::vector<Triangle>  gTriangles;

float  					gTotalTimeElapsed = 0;
int 					gTotalFrames = 0;
GLuint 					gTimer;

void init_timer()
{
    glGenQueries(1, &gTimer);
}

void start_timing()
{
    glBeginQuery(GL_TIME_ELAPSED, gTimer);
}

float stop_timing()
{
    glEndQuery(GL_TIME_ELAPSED);

    GLint available = GL_FALSE;
    while (available == GL_FALSE)
        glGetQueryObjectiv(gTimer, GL_QUERY_RESULT_AVAILABLE, &available);

    GLint result;
    glGetQueryObjectiv(gTimer, GL_QUERY_RESULT, &result);

    float timeElapsed = result / (1000.0f * 1000.0f * 1000.0f);
    return timeElapsed;
}

void display()
{
    // (1) 화면ㆍ깊이 버퍼 초기화 -------------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // (2) 모델-뷰 행렬 설정 (카메라가 (0,0,0)에 있고, w축이 +z 방향) -------------
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                          // e = (0,0,0), u=(1,0,0), v=(0,1,0), w=(0,0,1)

    // (3) 타이머 시작 -----------------------------------------------------------
    start_timing();

    // (4) 토끼 모델 변환 : 먼저 스케일, 그다음 평행이동 --------------------------
    glPushMatrix();
    glTranslatef(0.1f, -1.0f, -1.5f);      // T
    glScalef(10.0f, 10.0f, 10.0f);      // S (scale → translate 순이 되도록 T*S)
    /* -------- Immediate-mode로 삼각형 그리기 -------- */
    glBegin(GL_TRIANGLES);
    for (const auto& tri : gTriangles) {
        for (int k = 0; k < 3; ++k) {
            unsigned int idx = tri.indices[k];
            const Vector3& n = gNormals[idx];
            const Vector3& p = gPositions[idx];
            glNormal3f(n.x, n.y, n.z);
            glVertex3f(p.x, p.y, p.z);
        }
    }
    glEnd();
    /* ---------------------------------------------- */
    glPopMatrix();

    // (5) 타이머 정지 → FPS 갱신 -----------------------------------------------
    float timeElapsed = stop_timing();         // sec
    gTotalFrames++;
    gTotalTimeElapsed += timeElapsed;
    float fps = gTotalFrames / gTotalTimeElapsed;

    char title[128];
    sprintf(title, "OpenGL Bunny (Immediate) : %0.2f FPS", fps);
    glutSetWindowTitle(title);

    // (6) 다음 프레임 예약 & 버퍼 스왑 -----------------------------------------
    glutPostRedisplay();
    glutSwapBuffers();
}
