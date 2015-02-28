#include <cub.h>

cub::cub()
{
    std::cout<<"Estic en el constructor del cub\n";
    // Cub centrat al 0,0,0 amb dimensió 1 a totes les seves arestes
    xorig = -0.5; yorig = -0.50; zorig = -0.5;
    a = 1.0;
    h = 1.0;
    p = 1.0;
}

// Constructora amb tots els parametres
cub::cub(int an, int al, int profu, const GLfloat x0, GLfloat y0, GLfloat z0)
{
    std::cout<<"Estic en el constructor parametritzat del cub\n";
    a = an;
    h = al;
    p = profu;
    xorig = x0;
    yorig = y0;
    zorig = z0;
}

// Destructora
cub::~cub()
{

}




// Realitzacio de la geometria del cub en una genList: en aquest cas no es fa
void cub::make()
{

}


void cub::draw()
{
    glPushMatrix();

    /*      Posa el color a vermell */
     glColor3f(1.0f, 0.0f, 0.0f);
     glTranslatef(xorig, yorig, zorig);

    /*      Crea un cub de  a x h x p */
    glScalef((GLfloat)a, (GLfloat)h, (GLfloat)p);
    glPopMatrix();
    glPushMatrix();
    glBegin(GL_QUADS);		// Draw The Cube Using quads
        glColor3f(0.0f,1.0f,0.0f);	// Color Blue
        glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Top)
        glVertex3f( 1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Top)
        glColor3f(1.0f,0.5f,0.0f);	// Color Orange
        glVertex3f( 1.0f,-1.0f, 1.0f);	// Top Right Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f, 1.0f);	// Top Left Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Bottom)
        glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Bottom)
        glColor3f(1.0f,0.0f,0.0f);	// Color Red
        glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Front)
        glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Front)
        glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Front)
        glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Front)
        glColor3f(1.0f,1.0f,0.0f);	// Color Yellow
        glVertex3f( 1.0f,-1.0f,-1.0f);	// Top Right Of The Quad (Back)
        glVertex3f(-1.0f,-1.0f,-1.0f);	// Top Left Of The Quad (Back)
        glVertex3f(-1.0f, 1.0f,-1.0f);	// Bottom Left Of The Quad (Back)
        glVertex3f( 1.0f, 1.0f,-1.0f);	// Bottom Right Of The Quad (Back)
        glColor3f(0.0f,0.0f,1.0f);	// Color Blue
        glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Left)
        glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Left)
        glColor3f(1.0f,0.0f,1.0f);	// Color Violet
        glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Right)
        glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Right)
      glEnd();			// End Drawing The Cube
    glPopMatrix();
}

