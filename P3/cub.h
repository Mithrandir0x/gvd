#ifndef CUB_H
#define CUB_H

#include <QGLShaderProgram>
#include <QOpenGLTexture>


#include <stdio.h>
#include <Common.h>

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

typedef vec4  color4;
typedef vec4  point4;

class cub
{
  public:



      cub();
      cub(int an, int al, int profu, GLfloat x0, GLfloat y0, GLfloat z0);
      ~cub();
      void make();
      void toGPU(QGLShaderProgram *program);
      void draw();
      void aplicaTG(mat4 m);

  private:

      void quad( int a, int b, int c, int d );
      void initTextura();

      int a; // amplada
      int h; // alcada
      int p; // profunditat
      GLdouble xorig, yorig, zorig;

      point4 vertices[8]; // 8 vertexs del cub
      color4 vertex_colors[8]; // 8 colors RGBA associats a cada vertex

      int Index;
      point4 points[NumVertices]; // 36 punts: cub triangulat
      color4 colors[NumVertices]; // colors en aquests punts
      vec2 vertexsTextura[NumVertices]; // coordenades de textura associades a cada vertex

      QOpenGLTexture *texture[2];

};

#endif // CUB_H
