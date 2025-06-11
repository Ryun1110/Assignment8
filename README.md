## OpenGL Bunny Renderer (Assignment 8)

---

### 🎯 Features

- **Mesh Loading**  
  - Parses a Wavefront `.obj` file (`bunny.obj`) containing 69,451 triangles  
  - Extracts vertex positions and normals, and builds an index buffer  
- **Transformations & Camera**  
  - Uniform scale ×10, then translate by (0.1, –1, –1.5)  
  - Camera at eye (0, 0, 0), looking down the –z axis (`u=(1,0,0)`, `v=(0,1,0)`, `w=(0,0,1)`)  
  - Perspective projection via `glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0)`  
  - Square viewport of 1280×1280 pixels  
- **Lighting & Shading**  
  - Depth testing enabled; back-face culling disabled  
  - Material:  
    - Ambient & diffuse reflectance `ka = kd = (1.0, 1.0, 1.0)`  
    - No specular component (`ks = (0, 0, 0)`, shininess = 0)  
  - Global ambient light `Ia = (0.2, 0.2, 0.2)`  
  - Single directional light from (–1, –1, –1) with diffuse color (1, 1, 1)  
- **Performance Timing**  
  - `frame_timer.cpp` uses `GL_TIME_ELAPSED` queries to measure render time per frame  
  - Updates GLUT window title with “OpenGL Bunny (Mode) : XX.XX FPS”  
- **Two Rendering Modes**  
  1. **Immediate Mode** (`Q1.cpp`)  
     - Calls `glBegin(GL_TRIANGLES) / glNormal3f / glVertex3f / glEnd` every frame  
  2. **Vertex Array + glDrawElements** (`Q2.cpp`)  
     - Creates VAO and VBOs for positions, normals, and indices  
     - Uploads all mesh data once to GPU memory  
     - Draws with `glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0)` for maximum efficiency  

---


### ⚙️ Prerequisites

- **Platform:** Linux, macOS, or Windows  
- **Libraries:**  
  - GLEW  
  - freeglut (or GLUT)  
  - OpenGL Utility Library (GLU)  
- **Compiler:**  
  - GCC / Clang (C++17 support)  
  - MSVC (with `_CRT_SECURE_NO_WARNINGS` defined)

---

🚀 Expected Performance
Mode	Typical FPS*
Immediate Mode (Q1)	~20–60
Vertex Array (Q2)	~100–300

*FPS varies with hardware and driver configuration.

`
