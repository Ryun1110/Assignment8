// -----------------------------------------------------------------------------
//  main_vertex_array.cpp  –  HW8 Q2 : VAO + VBO + glDrawElements
//    빌드 예시(Linux):
//      g++ main_vertex_array.cpp load_mesh.cpp timer_only.cpp \
//          -lGLEW -lglut -lGLU -lGL -std=c++17 -O2 -o bunny_vertex_array
// -----------------------------------------------------------------------------
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <cstdio>
#include <string>

//-------------- 외부 자원 -------------------------------------------------------
struct Vector3 { float x, y, z; };
struct Triangle { unsigned int indices[3]; };

extern std::vector<Vector3>   gPositions;   // load_mesh.cpp에서 정의
extern std::vector<Vector3>   gNormals;
extern std::vector<Triangle>  gTriangles;

void  load_mesh(std::string fileName);
void  init_timer();          // timer_only.cpp
void  start_timing();
float stop_timing();

//-------------- VAO/VBO 핸들 ---------------------------------------------------
static GLuint gVAO = 0, gVBOpos = 0, gVBOnrm = 0, gEBO = 0;
static GLsizei gIndexCount = 0;

//==============================================================================
//  GPU 버퍼 구축 : 한 번만 호출
//==============================================================================
void build_buffers()
{
    // 1) 인덱스 배열로 변환 -----------------------------------------------------
    std::vector<unsigned int> indices;
    indices.reserve(gTriangles.size() * 3);
    for (const Triangle& t : gTriangles)
        for (int k = 0; k < 3; ++k) indices.push_back(t.indices[k]);
    gIndexCount = static_cast<GLsizei>(indices.size());

    // 2) VAO -------------------------------------------------------------------
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);

    // 3-A) 위치 VBO ------------------------------------------------------------
    glGenBuffers(1, &gVBOpos);
    glBindBuffer(GL_ARRAY_BUFFER, gVBOpos);
    glBufferData(GL_ARRAY_BUFFER,
        gPositions.size() * sizeof(Vector3),
        gPositions.data(), GL_STATIC_DRAW);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vector3), (void*)0);

    // 3-B) 노멀 VBO ------------------------------------------------------------
    glGenBuffers(1, &gVBOnrm);
    glBindBuffer(GL_ARRAY_BUFFER, gVBOnrm);
    glBufferData(GL_ARRAY_BUFFER,
        gNormals.size() * sizeof(Vector3),
        gNormals.data(), GL_STATIC_DRAW);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(Vector3), (void*)0);

    // 4) 인덱스(EBO) -----------------------------------------------------------
    glGenBuffers(1, &gEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW);

    // 5) 뒷정리 -----------------------------------------------------------------
    glBindVertexArray(0);
}

//==============================================================================
//  OpenGL 상태 초기화
//==============================================================================
void init_opengl()
{
    glutInitWindowSize(1280, 1280);
    glutCreateWindow("OpenGL Bunny (Vertex Array)");

    glewInit();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);        // 스케일링 후 노멀 보정

    GLfloat Ia[4] = { 0.2f,0.2f,0.2f,1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ia);

    GLfloat zero[4] = { 0,0,0,1 };
    GLfloat white[4] = { 1,1,1,1 };
    GLfloat L_dir[4] = { -1.0f, -1.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, zero);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, zero);
    glLightfv(GL_LIGHT0, GL_POSITION, L_dir);
    GLfloat ka_kd[4] = { 1,1,1,1 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka_kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ka_kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
}

//==============================================================================
//  디스플레이 콜백 – glDrawElements()
//==============================================================================
float gTotalTime = 0.f;   int gFrame = 0;
void display_vertex_array()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 카메라 위치 설정 (MV 행렬을 단위행렬로).
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    // (2) 타이머 시작
    start_timing();

    // (3) 버니 모델 변환: ★순서 주의★ (Translate 먼저 → 그 다음 Scale)
    glPushMatrix();
    // 3-1) 평행 이동 (Translate)
    glTranslatef(0.1f, -1.f, -1.5f);
    // 3-2) 크기 10배 (Scale)
    glScalef(10.f, 10.f, 10.f);

    // 3-3) VAO 바인딩 및 드로우
    glBindVertexArray(gVAO);
    glDrawElements(GL_TRIANGLES, gIndexCount, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    glPopMatrix();

    // (4) 타이머 정지 및 FPS 계산/제목 갱신
    float dt = stop_timing();
    gTotalTime += dt;
    ++gFrame;
    char buf[128];
    // 타이틀 오탈자 수정: "Vertex Array"
    sprintf(buf, "OpenGL Bunny (Vertex Array) : %.2f FPS", gFrame / gTotalTime);
    glutSetWindowTitle(buf);

    // (5) 버퍼 스왑 & 다음 프레임 예약
    glutSwapBuffers();
    glutPostRedisplay();
}

//==============================================================================
//  main
//==============================================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    init_opengl();

    // OBJ 로드 → CPU 메모리
    load_mesh("bunny.obj");

    // VBO / VAO → GPU 메모리
    build_buffers();

    // 타이머 준비 + 렌더 콜백
    init_timer();
    glutDisplayFunc(display_vertex_array);

    glutMainLoop();
    return 0;
}
