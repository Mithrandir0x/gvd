#ifndef CUB_H
#define CUB_H


#include <Common.h>
#include <stdio.h>


const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

typedef Common::vec4  color4;
typedef Common::vec4  point4;

class cub
{
  public:
      cub();
      cub(int an, int al, int profu, GLfloat x0, GLfloat y0, GLfloat z0);
      ~cub();
      void make();
      void draw();

  private:

      GLdouble xorig, yorig, zorig;

      int a; // amplada
      int h; // alcada
      int p; // profunditat

};

#endif // CUB_H
