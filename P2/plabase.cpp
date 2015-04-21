#include "plabase.h"

PlaBase::PlaBase(point4 v0, point4 v1, point4 v2, point4 v3, color4 cv0, color4 cv1, color4 cv2, color4 cv3) : Objecte(NumVertices)
{

    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    vertices[3] = v3;

    vertex_colors[0] = cv0; //white
    vertex_colors[1] = cv1; //red
    vertex_colors[2] = cv2; //yellow
    vertex_colors[3] = cv3; //green


    make();
    capsa = calculCapsa3D();

    /*double escalap = 2.0 / capsa.p;//para escalar la dimension maxima(profundidad) a 2
    double escalaa = 1.08821869 / capsa.a;//para que coincida con la mesa de billar

    mat4 m = Scale(escalaa, 1.0, escalap)*Translate(-(capsa.pmin.x + capsa.a/2.), -(capsa.pmin.y + capsa.h/2.), -(capsa.pmin.z + capsa.p/2.));
    aplicaTG(m);
    capsa = calculCapsa3D();*/
}

PlaBase::~PlaBase()
{

}

void PlaBase::make()
{
    Index = 0;
    quad( 3, 2, 1, 0 );//llamando en este orden los triangulos se definen counterclok wise
    initTextura();
}

// quad generates two triangles for each face and assigns colors
//    to the vertices

void PlaBase::quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; vertexsTextura[Index] = vec2(1.0, 0.0); Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; vertexsTextura[Index] = vec2(0.0, 0.0); Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; vertexsTextura[Index] = vec2(0.0, 1.0); Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; vertexsTextura[Index] = vec2(1.0, 0.0); Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; vertexsTextura[Index] = vec2(0.0, 1.0); Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; vertexsTextura[Index] = vec2(1.0, 1.0); Index++;
}

void PlaBase::initTextura()
 {
     qDebug() << "Initializing textures...";

     // Carregar la textura
     glActiveTexture(GL_TEXTURE0);
     texture = new QOpenGLTexture(QImage("://resources/Fabric_Green_L.jpg"));
     //texture = new QOpenGLTexture(QImage("://resources/Bola9.jpg"));
     texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
     texture->setMagnificationFilter(QOpenGLTexture::Linear);

     texture->bind(0);

 }



