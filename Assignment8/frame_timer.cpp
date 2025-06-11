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
