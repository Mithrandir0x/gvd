#include "plabase.h"

PlaBase::PlaBase() : Objecte(NumVertices)
{
    //0.03075
    double h = 0.0;

    vertices[0] = point4( 0.5, h, 0.5, 1.0 );
    vertices[1] = point4( 0.5, 0.0,-0.5, 1.0 );
    vertices[2] = point4(-0.5, 0.0,-0.5, 1.0 );
    vertices[3] = point4(-0.5, h, 0.5, 1.0 );

    vertex_colors[0] = color4( 1.0, 1.0, 1.0, 1.0 ); //white
    vertex_colors[1] = color4( 1.0, 0.0, 0.0, 1.0 ); //red
    vertex_colors[2] = color4( 1.0, 1.0, 0.0, 1.0 ); //yellow
    vertex_colors[3] = color4( 0.0, 1.0, 0.0, 1.0 ); //green

    make();
    capsa = calculCapsa3D();

    double escalap = 2.0 / capsa.p;//para escalar la dimension maxima(profundidad) a 2
    double escalaa = 1.08821869 / capsa.a;//temporal

    mat4 m = Scale(escalaa, 1.0, escalap)*Translate(-(capsa.pmin.x + capsa.a/2.), -(capsa.pmin.y + capsa.h/2.), -(capsa.pmin.z + capsa.p/2.));
    aplicaTG(m);
    capsa = calculCapsa3D();
}

PlaBase::~PlaBase()
{

}

void PlaBase::make()
{
    Index = 0;
    quad( 0, 1, 2, 3 );
    initTextura();
}

// quad generates two triangles for each face and assigns colors
//    to the vertices

void PlaBase::quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; vertexsTextura[Index] = vec2(0.0, 0.0); Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; vertexsTextura[Index] = vec2(1.0, 0.0); Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; vertexsTextura[Index] = vec2(1.0, 1.0); Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; vertexsTextura[Index] = vec2(1.0, 1.0); Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; vertexsTextura[Index] = vec2(0.0, 0.0); Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; vertexsTextura[Index] = vec2(1.0, 0.0); Index++;
}

void PlaBase::initTextura()
 {
     qDebug() << "Initializing textures...";

     // Carregar la textura
     glActiveTexture(GL_TEXTURE0);
     texture = new QOpenGLTexture(QImage("://resources/Fabric_Green_L.jpg"));
     //texture = new QOpenGLTexture(QImage("://resources/Bola4.jpg"));
     texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
     texture->setMagnificationFilter(QOpenGLTexture::Linear);

     texture->bind(0);

 }



