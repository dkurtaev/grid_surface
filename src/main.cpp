#include <GL/freeglut.h>

void display();
void reshape(int width, int height);
void setup_camera();
void draw_axises();

int window_width;
int window_height;

int main(int argc, char** argv) {
  glutInit(&argc, argv);  // GLUT initialization.

  // Window initialization.
  glutInitWindowSize(324, 324);  // Width, height of window.
  glutInitWindowPosition(0, 0);  // (x, y) position of window from top-left.
  glutCreateWindow("Surface on grid");

  // Set callbacks.
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return 0;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  setup_camera();
  draw_axises();

  glutSwapBuffers();
}

void draw_axises() {
  // Ox is red, Oy is blue, Oz is green.
  glBegin(GL_LINES);
    glColor3ub(255, 0, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);

    glColor3ub(0, 255, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);

    glColor3ub(0, 0, 255);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
  glEnd();
}

void setup_camera() {
  static const float near_clip_plane = 0.1f;
  static const float far_clip_plane = 1000.0f;
  static const float view_angle = 30.0f;
  const float aspect = (float)window_width / window_height;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(view_angle, aspect, near_clip_plane, far_clip_plane);
  gluLookAt(100, 100, 50,  // Camera position.
            0, 0, 0,       // View point position.
            0, 0, 1);      // Camera axis.
}

void reshape(int width, int height) {
  window_width = width;
  window_height = height;
  glViewport(0, 0, width, height);
}
