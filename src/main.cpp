#include <math.h>

#include <GL/freeglut.h>

#include "include/camera.hpp"
#include "include/surface.hpp"

void display();
void reshape(int width, int height);
void DrawAxises();

int window_width;
int window_height;
Camera camera(19, 7, 16);
Surface surface(-5.0f, 5.0f, -5.0f, 5.0f, 100, 100);

int main(int argc, char** argv) {
  glutInit(&argc, argv);  // GLUT initialization.

  // Window initialization.
  glutInitWindowSize(324, 324);  // Width, height of window.
  glutInitWindowPosition(0, 0);  // (x, y) position of window from top-left.
  glutCreateWindow("Surface on grid");

  // Set callbacks.
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  // Initialize OpenGL's flags and parameters for lighting procedure.
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);

  glEnable(GL_LIGHT0);
  GLfloat ambient[] = { 0.2, 0.2, 0.2, 1 };
  GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1 };
  GLfloat position[] = { 100, 100, 100, 1 };
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

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
