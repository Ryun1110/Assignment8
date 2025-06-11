#ifdef _MSC_VER          // MSVC에서만
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <cstdio>
#include <string>
void init_timer();
void start_timing();
float stop_timing();
void display();
void load_mesh(std::string fileName);
struct Vector3 { float x, y, z; };
struct Triangle { unsigned int indices[3]; };

extern std::vector<Vector3>   gPositions;  // load_mesh.cpp에서 정의
extern std::vector<Vector3>   gNormals;
extern std::vector<Triangle>  gTriangles;

void init_opengl()
{
    // 뷰포트 1280×1280 ---------------------------------------------------------
    glutInitWindowSize(1280, 1280);
    glutCreateWindow("OpenGL Bunny (Immediate)");

    // GLEW 초기화 --------------------------------------------------------------
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // 투영 행렬 : l,r,b,t,n,f = -0.1,0.1,-0.1,0.1,0.1,1000 (near/far > 0) ------
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);

    // 렌더링 상태 ---------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);          // 깊이 버퍼
    glDisable(GL_CULL_FACE);          // 뒷면 제거 비활성

    // 고정 기능 파이프라인용 조명 ---------------------------------------------
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    GLfloat globalAmbient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    GLfloat lightAmb[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat lightDiff[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpec[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat lightDir[4] = { -1.0f, -1.0f, -1.0f, 0.0f };  // w=0 → 방향광
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);

    // 재질(ka = kd = 1, ks=0, p=0) --------------------------------------------
    GLfloat ka_kd[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ks[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka_kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ka_kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
}

int main(int argc, char** argv)
{
    // -------------------------------------------------------------------------
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    init_opengl();            // 상태/조명/투영 설정

    // 토끼 OBJ 로드 ------------------------------------------------------------
    load_mesh("bunny.obj");   // 같은 폴더에 bunny.obj가 있어야 함

    // 타이머 초기화 & 콜백 등록 ----------------------------------------------
    init_timer();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
