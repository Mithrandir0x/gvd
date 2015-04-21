

#include <math.h>

#include <glwidget.h>
#include <QString>


GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)

{
    setFocusPolicy( Qt::StrongFocus );
    esc = new Escena();

    xRot = 0;
    yRot = 0;
    zRot = 0;

    a = 20.0;
    h = 20.0;
    p = 20.0;

    clearColor = Qt::black;
    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

    program = 0;
    moviment = false;

}


GLWidget::~GLWidget()
{
    makeCurrent();
    delete esc;
}


// Create a GLSL program object from vertex and fragment shader files
void
GLWidget::InitShader(const char* vShaderFile, const char* fShaderFile)
{           

    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    // Compilació del vertex shader
    vshader->compileSourceFile(vShaderFile);
    fshader->compileSourceFile(fShaderFile);

    program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);

    program->bindAttributeLocation("vPosition", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("vColor", PROGRAM_COLOR_ATTRIBUTE);


    // muntatge del shader en el pipeline gràfic per a ser usat
    program->link();

    // unió del shader al pipeline gràfic
    program->bind();

}

void GLWidget::initShadersGPU()
{
// Carrega dels shaders i posa a punt per utilitzar els programes carregats a la GPU
   InitShader( "://vshader1.glsl", "://fshader1.glsl" );

}


QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const

{
    return QSize(400, 400);
}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}


void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        update();
    }
}


void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        update();
    }
}


void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    std::cout<<"Estic inicialitzant el shaders"<<std::endl;
    initShadersGPU();

    //glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void GLWidget::paintGL()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   qNormalizeAngle(xRot);
   qNormalizeAngle(yRot);
   qNormalizeAngle(zRot);

   mat4 transform = ( RotateX( xRot / 16.0 ) *
                       RotateY( yRot / 16.0 ) *
                       RotateZ( zRot / 16.0 ) );

   // A modificar si cal girar tots els objectes
   if(xRot != xRotOld || yRot != yRotOld || zRot != zRotOld){
       ejex = transform * ejex;
       ejez = transform * ejez;
       esc->aplicaTG(transform);
   }

   xRotOld = xRot;
   yRotOld = yRot;
   zRotOld = zRot;

   esc->draw(program);


}


void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-1.5, +1.5, -1.5, +1.5, 0.0, 15.0);
#else
    glOrtho(-1.5, +1.5, -1.5, +1.5, 0.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    //std::cout<<"\ndx=" <<dx<<" ,zRot= "<<zRot<<"\n";

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + ROTATIONSPEED * dy);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + ROTATIONSPEED * dy);
        setZRotation(zRot + ROTATIONSPEED * dx);
    }
    //std::cout<<"dx=" <<dx<<" ,zRot= "<<zRot<<"\n";
    lastPos = event->pos();
}


void GLWidget::keyPressEvent(QKeyEvent *event)
{
    // Metode a implementar

    //std::cout<<"\nkeyPressEvent\n";
    int Key_Left = 0x01000012;
    int Key_Up = 0x01000013;
    int Key_Right = 0x01000014;
    int Key_Down = 0x01000015;

    double deltaDesplacament = 0.01;
    double dzP=deltaDesplacament, dzN=-deltaDesplacament, dxP=deltaDesplacament, dxN=-deltaDesplacament;
    mat4 m;
    Capsa3D capBi;


    if(cT.pmin.z - cb.pmin.z > - deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dzN) < deltaDesplacament en borde de mesa con z negativos
        dzN = cT.pmin.z - cb.pmin.z;
    if(cT.pmin.z + cT.p - (cb.pmin.z + cb.p) < deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dzP) < deltaDesplacament en borde de mesa con z positivos
        dzP = cT.pmin.z + cT.p - (cb.pmin.z + cb.p);
    if(cT.pmin.x - cb.pmin.x > -deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dxN) < deltaDesplacament en borde de mesa con x negativos
        dxN = cT.pmin.x - cb.pmin.x;
    if(cT.pmin.x + cT.a - (cb.pmin.x + cb.a) < deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dxN) < deltaDesplacament en borde de mesa con x positivos
        dxP = cT.pmin.x + cT.a - (cb.pmin.x + cb.a);

    for (int i=0; i<esc->conjuntBoles->listaConjuntBoles.size(); i++) {//comparamos bola blanca con resto de bolas
        if(abs((cb.pmin.x+cb.a/2.) - (listaCapsasConjuntBoles[i].pmin.x+listaCapsasConjuntBoles[i].a/2.)) < (cb.a/2. + listaCapsasConjuntBoles[i].a/2.)
                && abs((cb.pmin.z+cb.p/2.) - (listaCapsasConjuntBoles[i].pmin.z+listaCapsasConjuntBoles[i].p/2.)) < cb.p/2. + listaCapsasConjuntBoles[i].p/2.){

            if(event->key() == Key_Up){
                if((cb.pmin.z + cb.p/2.0) - (listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p/2.0)<= 0.0){//si bola blanca con menor z que la i puede haber limitacion
                    if(listaCapsasConjuntBoles[i].pmin.z - cb.pmin.z - cb.p < deltaDesplacament){//si distancia menor que deltaDesplacament hay limitacion
                        dzP = listaCapsasConjuntBoles[i].pmin.z - cb.pmin.z - cb.p;
                        if(dzP < 0.005)dzP = 0.0;
                    }
                }else{
                    dzP = deltaDesplacament;//bola blanca tiene mayor z que la i por lo tanto no hay limitacion
                }
            }

            if(event->key() == Key_Down){
                if((cb.pmin.z + cb.p/2.0) - (listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p/2.0)>= 0.0){//si bola blanca con mayor z que la i, puede haber limitacion
                    if(listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p - cb.pmin.z > -deltaDesplacament){//si distancia menor que deltaDesplacament hay limitacion
                         dzN = listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p - cb.pmin.z;
                         if(dzN > -0.005)dzN = 0.0;
                    }   else{
                           dzN = -deltaDesplacament;//bola blanca tiene menor z que la i por lo tanto no hay limitacion
                        }
                 }
            }

            if(event->key() == Key_Left){
                if((cb.pmin.x + cb.a/2.)- (listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a/2.)<= 0.0){//si bola blanca a la izquierda no hay limitacion
                    dxN = -deltaDesplacament;
                }else{
                    if(listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a - cb.pmin.x > -deltaDesplacament){//si distancia menor que deltaDesplacament hay limitacion
                        dxN = listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a - cb.pmin.x;
                        if(dxN > -0.002)dxN = 0.0;
                    }
                }
            }

            if(event->key() == Key_Right){
                if((cb.pmin.x + cb.a/2.)- (listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a/2.) >= 0.0){//si bola blanca a la derecha no hay limitacion
                    dxP = deltaDesplacament;
                }else{
                    if(listaCapsasConjuntBoles[i].pmin.x - (cb.pmin.x + cb.a) < deltaDesplacament ){//si distancia menor que deltaDesplacament hay limitacion
                        dxP = listaCapsasConjuntBoles[i].pmin.x - (cb.pmin.x + cb.a);
                        if(dxP < 0.002)dxP = 0.0;
                    }
                }
            }

        }
    }


    if (esc->bolaBlanca!=NULL && esc->plaBase!=NULL ) {


       switch ( event->key() )
       {
       case Qt::Key_Up:
                  m = Translate(ejez.x*dzP,  ejez.y*dzP, ejez.z*dzP);
                  cb.pmin.z += dzP;
           break;
       case Qt::Key_Down:
                  m = Translate(ejez.x*dzN,  ejez.y*dzN, ejez.z*dzN);
                  cb.pmin.z += dzN;
           break;
       case Qt::Key_Left:
                  m = Translate(ejex.x*dxN,  ejex.y*dxN, ejex.z*dxN);
                  cb.pmin.x += dxN;
           break;
       case Qt::Key_Right:
                  m = Translate(ejex.x*dxP,  ejex.y*dxP, ejex.z*dxP);
                  cb.pmin.x += dxP;
           break;
       }

       esc->bolaBlanca->aplicaTG(m);
       update();

    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    // Metode a implementar en el cas que es mogui la bola

}



void GLWidget::adaptaObjecteTamanyWidget(Objecte *obj)
{
    // Metode a implementar
    Capsa3D capsa;
    mat4 m;

    capsa = obj->calculCapsa3D();
    if (dynamic_cast<TaulaBillar*>(obj)){
            double aristaMax = 0.0;

        if(capsa.a > capsa.p){
            aristaMax=capsa.a;
        } else{
            aristaMax=capsa.p;
        }
        if(capsa.h > aristaMax){
            aristaMax=capsa.h;
        }

        double escala = 2.0 / aristaMax;
        mat4 m = Scale(escala, escala, escala);
        obj->aplicaTG(m);//la mayor arista de la caja contenedora de la mesa es 2.0

        capsa = obj->calculCapsa3D();

        m = Translate(-(capsa.pmin.x + capsa.a / 2.), -(capsa.pmin.y + capsa.h), -(capsa.pmin.z + capsa.p / 2.));
        obj->aplicaTG(m);//deja el centro de la mesa(x,z) en el origen de coordenadas y el tope en el plano y=0, con arista maxima 2.0
        //capsa = obj->calculCapsa3D();

    }else if (dynamic_cast<Bola*>(obj)){
            m = Translate(0.0,  -capsa.pmin.y, 0.0);//la base de las 16 bolas quedan en y = 0
            obj->aplicaTG(m);
    }else if (dynamic_cast<PlaBase*>(obj)){
            mat4 m = Scale(2.0/(1.8379*capsa.a), 1.0, 2.0/capsa.p)*Translate(-(capsa.pmin.x + capsa.a/2.), -(capsa.pmin.y + capsa.h/2.), -(capsa.pmin.z + capsa.p/2.));
            obj->aplicaTG(m);
    }
            //capsa = obj->calculCapsa3D();
}

void GLWidget::newObjecte(Objecte * obj)
{
    adaptaObjecteTamanyWidget(obj);
    obj->toGPU(program);
    esc->addObjecte(obj);
    updateGL();
}
void GLWidget::newPlaBase()
{
    cout << "Creating new pla base" << endl;
    // Metode que crea un objecte PlaBase poligon amb el punt central al (0,0,0) i perpendicular a Y=0
    point4 v0  = point4( 10.0, 0.0, 10.0, 1.0 );
    point4 v1  = point4( 10.0, 0.0,-10.0, 1.0 );
    point4 v2  = point4(-10.0, 0.0,-10.0, 1.0 );
    point4 v3  = point4(-10.0, 0.0, 10.0, 1.0 );

    color4 cv0 = color4( 1.0, 1.0, 1.0, 1.0 ); //white
    color4 cv1  = color4( 1.0, 0.0, 0.0, 1.0 ); //red
    color4 cv2  = color4( 1.0, 1.0, 0.0, 1.0 ); //yellow
    color4 cv3  = color4( 0.0, 1.0, 0.0, 1.0 ); //green

    PlaBase *plaBase = new PlaBase(v0, v1, v2, v3, cv0, cv1, cv2, cv3);
    newObjecte(plaBase);
}

void GLWidget::newObj(QString fichero)
{
    // Metode que carrega un fitxer .obj llegit de disc
    TaulaBillar *obj;

    obj = new TaulaBillar(fichero);
    newObjecte(obj);
}

void GLWidget::newBola()
{
    // Metode que crea la Bola blanca de joc
     // Metode a implementar

    Bola *obj;

    obj = new Bola(0.0, 0.03075, -0.5, 0.03075, 1.0, 1.0, 1.0, "0");//x0,y0,z0,r,R,G,B,numBola
    newObjecte(obj);
}
void GLWidget::newConjuntBoles()
{
    // Metode que crea les 15 Boles del billar america
    // Metode a implementar
    ConjuntBoles *cb;

    cb = new ConjuntBoles();

    for (int i=0; i<cb->listaConjuntBoles.size(); i++) {
        adaptaObjecteTamanyWidget(cb->listaConjuntBoles[i]);
        cb->listaConjuntBoles[i]->toGPU(program);
    }
    esc->conjuntBoles = cb;
    updateGL();
}
void GLWidget::newSalaBillar()
{
    newPlaBase();
    cT = esc->plaBase->calculCapsa3D();//para el calculo de colisiones
    // Metode que construeix tota la sala de billar: taula, 15 boles i bola blanca
   /* QString fileName = "/home/jj/Qtexamples/P1/resources/taula.obj";
    if (!fileName.isNull())
        newObj(fileName);
    cT = esc->taulaBillar->calculCapsa3D();*/

    newBola();
    cb = esc->bolaBlanca->calculCapsa3D();//para el calculo de colisiones

    newConjuntBoles();
    for (int i=0; i<esc->conjuntBoles->listaConjuntBoles.size(); i++) {
        listaCapsasConjuntBoles.push_back(esc->conjuntBoles->listaConjuntBoles[i]->calculCapsa3D());
    }
}

// Metode per iniciar la dinàmica del joc
void GLWidget::Play()
{

}
