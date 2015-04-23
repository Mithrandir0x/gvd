

#include <math.h>

#include <glwidget.h>
#include <QString>


GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)

{
    //std::cout<<"GLWidget::GLWidget"<<std::endl;
    setFocusPolicy( Qt::StrongFocus );
    esc = new Escena(this->size().width(), this->size().height());

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


static void qNormalizeAngle(double &angle)
{
    while (angle < 0)
        angle += 360;
    while (angle > 360)
        angle -= 360;
}


void GLWidget::setXRotation(int angle)
{
    //modificando el vrp con angx
    /*if (angle > 0) {
        esc->camGeneral.vs.angx += 0.0005;
    } else if (angle<0)
        esc->camGeneral.vs.angx -= 0.0005;

    if(esc->camGeneral.vs.angx < -90.0 - atan(2.0/20.0))esc->camGeneral.vs.angx = -90.0 - atan(2.0/20.0);
    if(esc->camGeneral.vs.angx > -90.0 + atan(2.0/20.0))esc->camGeneral.vs.angx = -90.0 + atan(2.0/20.0);
    esc->camGeneral.vs.vrp[2] = 0.0 - 20.0 * tan(90.0 + esc->camGeneral.vs.angx);*/

    //modificando el obs con angx
    if (angle > 0) {
        esc->camGeneral.vs.angx += 1.0;
    } else if (angle<0)
        esc->camGeneral.vs.angx -= 1.0;
    qNormalizeAngle(esc->camGeneral.vs.angx);
    esc->camGeneral.vs.obs = esc->camGeneral.CalculObs(esc->camGeneral.vs.vrp,esc->camGeneral.piram.d,esc->camGeneral.vs.angx,esc->camGeneral.vs.angy);
    updateGL();
}


void GLWidget::setYRotation(int angle)
{
    if (angle > 0) {
        esc->camGeneral.vs.angy += 1.0;
    } else if (angle<0)
        esc->camGeneral.vs.angy -= 1.0;

    qNormalizeAngle(esc->camGeneral.vs.angy);
    //esc->camGeneral.vs.obs = esc->camGeneral.CalculObs(esc->camGeneral.vs.vrp,esc->camGeneral.piram.d,esc->camGeneral.vs.angx,esc->camGeneral.vs.angy);
    updateGL();

}

void GLWidget::setZRotation(int angle)
{
    /*if (angle > 0) {
        esc->camGeneral.vs.angz += 1.0;
    } else if (angle<0)
        esc->camGeneral.vs.angz -= 1.0;

    qNormalizeAngle(esc->camGeneral.vs.angz);
    //esc->camGeneral.vs.obs = esc->camGeneral.CalculObs(esc->camGeneral.vs.vrp,esc->camGeneral.piram.d,esc->camGeneral.vs.angx,esc->camGeneral.vs.angy);
    updateGL();*/
}


void GLWidget::initializeGL()
{
    //std::cout<<"GLWidget::initializeGL"<<std::endl;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShadersGPU();
    esc->pr = program;
    //glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    esc->camGeneral.toGPU(program);
    //esc->camGeneral.PrintCamera();
}

void GLWidget::paintGL()
{
   //std::cout<<"\nGLWidget::paintGL"<<std::endl;
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

   vec3 vu = esc->camGeneral.CalculVup(esc->camGeneral.vs.angx, esc->camGeneral.vs.angy, esc->camGeneral.vs.angz);
   esc->camGeneral.vs.vup = vec4(vu[0], vu[1], vu[2], 0.0);
   esc->camGeneral.CalculaMatriuModelView();

   esc->camGeneral.CalculaMatriuProjection();
   //esc->camGeneral.PrintCamera();
   esc->draw();
}


void GLWidget::resizeGL(int width, int height)
{
    //std::cout<<"GLWidget::resizeGL"<<std::endl;
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    esc->camGeneral.setViewport(0, 0, width, height);
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();


    if (event->buttons() & Qt::LeftButton) {
        if(lastPos.y()!= event->y() && lastPos.x()!= event->x()) {
            setYRotation(dx);
            //setXRotation(dy);
        }
        else if(lastPos.y()!= event->y()) {// rotar la camera
            //setXRotation(dy);
        }
        else if (lastPos.x()!= event->x()) {
            setYRotation(dx);
        }
    }

    if (event->buttons() & Qt::RightButton) {//es mas preciso usar un boton para cada giro
        if(lastPos.y()!= event->y() && lastPos.x()!= event->x()) {
            //setYRotation(dx);
            setXRotation(dy);
        }
        else if(lastPos.y()!= event->y()) {// rotar la camera
            setXRotation(dy);
        }
        else if (lastPos.x()!= event->x()) {
            //setYRotation(dx);
        }
    }

    lastPos = event->pos();
}


void GLWidget::keyPressEvent(QKeyEvent *event)
{
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

            if(event->key() == Qt::Key_Up){
                if((cb.pmin.z + cb.p/2.)- (listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p/2.)<= 0.0){//si bola blanca cn menor z que la i no hay limitacion
                    dzN = -deltaDesplacament;
                }else{
                    if(listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p - cb.pmin.z > -deltaDesplacament){//si distancia menor que deltaDesplacament hay limitacion
                        dzN = listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p - cb.pmin.z;
                        if(dzN > -0.002)dzN = 0.0;
                    }
                }
            }

            if(event->key() == Qt::Key_Down){
                if((cb.pmin.z+ cb.p/2.)- (listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p/2.) >= 0.0){//si bola blanca con mayor z que la i no hay limitacion
                    dzP = deltaDesplacament;
                }else{
                    if(listaCapsasConjuntBoles[i].pmin.z - (cb.pmin.z + cb.p) < deltaDesplacament ){//si distancia menor que deltaDesplacament hay limitacion
                        dzP = listaCapsasConjuntBoles[i].pmin.z - (cb.pmin.z + cb.p);
                        if(dzP < 0.002)dzP = 0.0;
                    }
                }
            }

            if(event->key() == Qt::Key_Left){
                if((cb.pmin.x + cb.a/2.)- (listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a/2.)<= 0.0){//si bola blanca a la izquierda no hay limitacion
                    dxN = -deltaDesplacament;
                }else{
                    if(listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a - cb.pmin.x > -deltaDesplacament){//si distancia menor que deltaDesplacament hay limitacion
                        dxN = listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a - cb.pmin.x;
                        if(dxN > -0.002)dxN = 0.0;
                    }
                }
            }

            if(event->key() == Qt::Key_Right){
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
                  if (event->modifiers() & Qt::AltModifier){
                      Pan(0, -0.01);
                      break;
                   }
                  m = Translate(ejez.x*dzN,  ejez.y*dzN, ejez.z*dzN);
                  cb.pmin.z += dzN;
           break;
       case Qt::Key_Down:
                 if (event->modifiers() & Qt::AltModifier){
                     Pan(0, 0.01);
                     break;
                  }
                  m = Translate(ejez.x*dzP,  ejez.y*dzP, ejez.z*dzP);
                  cb.pmin.z += dzP;
           break;
       case Qt::Key_Left:
                 if (event->modifiers() & Qt::AltModifier){
                    Pan(0.01, 0);
                    break;
                  }
                  m = Translate(ejex.x*dxN,  ejex.y*dxN, ejex.z*dxN);
                  cb.pmin.x += dxN;
           break;
       case Qt::Key_Right:
                 if (event->modifiers() & Qt::AltModifier){
                     Pan(-0.01, 0);
                     break;
                  }
                  m = Translate(ejex.x*dxP,  ejex.y*dxP, ejex.z*dxP);
                  cb.pmin.x += dxP;
           break;
       case Qt::Key_Plus:
           Zoom(-0.05);
           break;
       case Qt::Key_Minus:
           Zoom(0.05);
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

void GLWidget::Zoom (double inOut) {
     esc->camGeneral.AmpliaWindow(inOut);//aumentar el tamaño del window equivale a un zoom out(inOut positivo)
     updateGL();
}

void GLWidget::Pan(double dx, double dy) {
    esc->camGeneral.wd.pmin.x = esc->camGeneral.wd.pmin.x + dx;
    esc->camGeneral.wd.pmin.y = esc->camGeneral.wd.pmin.y + dy;
    updateGL();
}



void GLWidget::adaptaObjecteTamanyWidget(Objecte *obj)
{

}

void GLWidget::newObjecte(Objecte * obj)
{
    esc->addObjecte(obj);
    updateGL();
}

void GLWidget::newPlaBase()
{
    PlaBase *plaBase;

    plaBase = this->newPlaBs();
    newObjecte(plaBase);
}

PlaBase* GLWidget::newPlaBs(){
    point4 v0  = point4( 0.5441, 0.0, 1.0, 1.0 );
    point4 v1  = point4( 0.5441, 0.0,-1.0, 1.0 );
    point4 v2  = point4(-0.5441, 0.0,-1.0, 1.0 );
    point4 v3  = point4(-0.5441, 0.0, 1.0, 1.0 );

    color4 cv0 = color4( 1.0, 1.0, 1.0, 1.0 ); //white
    color4 cv1  = color4( 1.0, 0.0, 0.0, 1.0 ); //red
    color4 cv2  = color4( 1.0, 1.0, 0.0, 1.0 ); //yellow
    color4 cv3  = color4( 0.0, 1.0, 0.0, 1.0 ); //green

    PlaBase *plaBase = new PlaBase(v0, v1, v2, v3, cv0, cv1, cv2, cv3);
    return plaBase;
}

void GLWidget::newObj(QString fichero)
{
    // Metode que carrega un fitxer .obj llegit de disc

    TaulaBillar *taulabillar = new TaulaBillar(fichero);
    newObjecte(taulabillar);
}

void GLWidget::newBola()
{
    Bola *bolablanca = new Bola(0.0, 0.03075, 0.5, 0.03075, 1.0, 1.0, 1.0, "0");//x0,y0,z0,r,R,G,B,numBola
    newObjecte(bolablanca);
    cb = bolablanca->calculCapsa3D();//para el calculo de colisiones
}
void GLWidget::newConjuntBoles()
{
    ConjuntBoles *conjuntboles = new ConjuntBoles();

    esc->conjuntBoles = conjuntboles;
    for(int i=0; i<conjuntboles->listaConjuntBoles.size(); i++){
        esc->listaObjectes.push_back(conjuntboles->listaConjuntBoles[i]);
        listaCapsasConjuntBoles.push_back(conjuntboles->listaConjuntBoles[i]->calculCapsa3D());
    }

    updateGL();
}


    void GLWidget::newSalaBillar()
    {
        PlaBase *plaBase = this->newPlaBs();
        esc->addObjecte(plaBase);
        cT = plaBase->calculCapsa3D();//para el calculo de colisiones

       /* QString fileName = "/home/jj/Qtexamples/P1/resources/taula.obj";
        if (!fileName.isNull())
            newObj(fileName);
        cT = esc->taulaBillar->calculCapsa3D();*/

        Bola *bolab = new Bola(0.0, 0.03075, 0.5, 0.03075, 1.0, 1.0, 1.0, "0");//x0,y0,z0,r,R,G,B,numBola
        esc->addObjecte(bolab);
        cb = bolab->calculCapsa3D();//para el calculo de colisiones

        ConjuntBoles *conjuntboles = new ConjuntBoles();
        esc->conjuntBoles = conjuntboles;
        for(int i=0; i<conjuntboles->listaConjuntBoles.size(); i++){
            esc->listaObjectes.push_back(conjuntboles->listaConjuntBoles[i]);
            listaCapsasConjuntBoles.push_back(conjuntboles->listaConjuntBoles[i]->calculCapsa3D());
        }

        esc->CapsaMinCont3DEscena();
        esc->camGeneral.vs.vrp[0] = esc->capsaMinima.pmin[0]+(esc->capsaMinima.a/2.0);
        esc->camGeneral.vs.vrp[1] = esc->capsaMinima.pmin[1]+(esc->capsaMinima.h/2.0);
        esc->camGeneral.vs.vrp[2] = esc->capsaMinima.pmin[2]+(esc->capsaMinima.p/2.0);

        //esc->camGeneral.CalculWindow(esc->capsaMinima);
        esc->camGeneral.wd.a = 2.5;//al disminuir reduce al ancho de los objetos y los lleva a la derecha
                                   //al aumentar reduce al ancho de los objetos y los lleva a la izquierda
        esc->camGeneral.wd.h = 2.5;//al disminuir aumenta la profundidad de los objetos y los lleva hacia arriba
                                   //al aumentar disminuye la profundidad de los objetos y los lleva hacia abajo
        esc->camGeneral.wd.pmin[0] = -1.25;//al aumentar mueve los objetos a la izquierda
        esc->camGeneral.wd.pmin[1] = -1.25;//al aumentar mueve los objetos hacia abajo

        updateGL();

    }

// Metode per iniciar la dinàmica del joc
void GLWidget::Play()
{

}
