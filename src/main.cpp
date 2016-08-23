#include <math.h>

#include <GL/freeglut.h>

#include "include/camera.hpp"
#include "include/surface.hpp"

void display();
void reshape(int width, int height);
void DrawAxises();

int window_width;
int window_height;
Camera camera;
Surface surface(-5.0f, 5.0f, -5.0f, 5.0f, 50, 50);

int main(int argc, char** argv) {
  glutInit(&argc, argv);  // GLUT initialization.

  // Window initialization.
  glutInitWindowSize(324, 324);  // Width, height of window.
  glutInitWindowPosition(0, 0);  // (x, y) position of window from top-left.
  glutCreateWindow("Surface on grid");

  // Set callbacks.
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(display);

  glutMainLoop();
  return 0;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  camera.Setup(window_width, window_height);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
    DrawAxises();
    surface.Draw();

  camera.MakeScreenshot(window_width, window_height);

  glutSwapBuffers();
}

void DrawAxises() {
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

void reshape(int width, int height) {
  window_width = width;
  window_height = height;
  glViewport(0, 0, width, height);
}
