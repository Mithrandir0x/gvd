

#include <math.h>

#include <glwidget.h>
#include <QString>


GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)

{
    //std::cout<<"GLWidget::GLWidget"<<std::endl;
    setFocusPolicy( Qt::StrongFocus );

    cameraActual = true;
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
    if (angle > 0) {
        esc->camGeneral.vs.angx += 1.0;
    } else if (angle<0)
        esc->camGeneral.vs.angx -= 1.0;
    qNormalizeAngle(esc->camGeneral.vs.angx);
    esc->setAnglesCamera(cameraActual, esc->camGeneral.vs.angx, esc->camGeneral.vs.angy, esc->camGeneral.vs.angz);
    updateGL();
}


void GLWidget::setYRotation(int angle)
{
    if (angle > 0) {
        esc->camGeneral.vs.angy += 1.0;
    } else if (angle<0)
        esc->camGeneral.vs.angy -= 1.0;

    qNormalizeAngle(esc->camGeneral.vs.angy);
    esc->setAnglesCamera(cameraActual, esc->camGeneral.vs.angx, esc->camGeneral.vs.angy, esc->camGeneral.vs.angz);
    updateGL();

}

void GLWidget::setZRotation(int angle)
{
}


void GLWidget::initializeGL()
{
    //std::cout<<"GLWidget::initializeGL"<<std::endl;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShadersGPU();
    esc->pr = program;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(cameraActual==true){
        esc->camGeneral.toGPU(program);
    }else{
        esc->camFirstP.toGPU(program);
    }
}

void GLWidget::paintGL()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   esc->actualizaMatr(cameraActual);
   esc->draw(cameraActual);

   if(cameraActual == true){
       std::cout<<"\ncamGeneral";
       esc->camGeneral.PrintCamera();
   }else{
       std::cout<<"\ncamFirstP";
       esc->camFirstP.PrintCamera();
   }
}


void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    if(cameraActual==true){
        esc->camGeneral.setViewport(0, 0, width, height);
    }else{
        esc->camFirstP.setViewport(0, 0, width, height);
    }
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(cameraActual == false)return;//solo funciona con camGeneral

    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();


    if (event->buttons() & Qt::LeftButton) {
        if(lastPos.y()!= event->y() && lastPos.x()!= event->x()) {
            //setYRotation(dx);
            //setXRotation(dy);
        }
        else if(lastPos.y()!= event->y()) {
            setXRotation(dy);
        }
        else if (lastPos.x()!= event->x()) {
            setYRotation(dx);
        }
    }

    lastPos = event->pos();
}


void GLWidget::keyPressEvent(QKeyEvent *event)
{
    mat4 m;
    double angx;
    cb = esc->bolaBlanca->calculCapsa3D();
    ctrB.x = cb.pmin.x + cb.a/2.0;
    ctrB.y = cb.pmin.y + cb.h/2.0;
    ctrB.z = cb.pmin.z + cb.p/2.0;

    if (esc->bolaBlanca!=NULL && esc->plaBase!=NULL && esc->conjuntBoles!=NULL){
        esc->computeCollisions(cb, cT, ctrB, listaCapsasConjuntBoles, event);
    }

       switch ( event->key() )
       {
       case Qt::Key_B:
                  /*z = 0.45;
                  timer = new QTimer(this);
                  connect(timer, SIGNAL(timeout()),this, SLOT(transition2First()));
                  timer->start(10);*/
                  cameraActual = false; //camFirstP
                  break;
       case Qt::Key_T:
                  /*z = 0.6;
                  timer1 = new QTimer(this);
                  connect(timer1, SIGNAL(timeout()),this, SLOT(transition2General()));
                  cameraActual = true;//camGen
                  timer1->start(10);*/
                  cameraActual = true;//CamGen
                  break;
       case Qt::Key_Up:
                  if (event->modifiers() & Qt::AltModifier){
                      Pan(0, 0.01);
                      break;
                   }
                  m = Translate(0.0,  0.0, esc->dzN) * Translate(ctrB.x,  ctrB.y, ctrB.z) * RotateX((esc->dzN*180.0)/(0.0307474*M_PI)) * Translate(-ctrB.x,  -ctrB.y, -ctrB.z );
           break;
       case Qt::Key_Down:
            if (event->modifiers() & Qt::AltModifier){
              Pan(0, -0.01);
              break;
            }
            m = Translate(0.0,  0.0, esc->dzP) * Translate(ctrB.x,  ctrB.y, ctrB.z) * RotateX((esc->dzP*180.0)/(0.0307474*M_PI)) * Translate(-ctrB.x,  -ctrB.y, -ctrB.z );
            break;
       case Qt::Key_Left:
           if (event->modifiers() & Qt::AltModifier){
              Pan(-0.01, 0);
              break;
           }
           m = Translate(esc->dxN,  0.0, 0.0) * Translate(ctrB.x,  ctrB.y, ctrB.z) * RotateZ(-(esc->dxN*180.0)/(0.0307474*M_PI)) * Translate(-ctrB.x,  -ctrB.y, -ctrB.z );
           break;
       case Qt::Key_Right:
          if (event->modifiers() & Qt::AltModifier){
              Pan(0.01, 0);
              break;
           }
           m = Translate(esc->dxP,  0.0, 0.0) * Translate(ctrB.x,  ctrB.y, ctrB.z) * RotateZ(-(esc->dxP*180.0)/(0.0307474*M_PI)) * Translate(-ctrB.x,  -ctrB.y, -ctrB.z );
           break;
       case Qt::Key_Plus:
           Zoom(-0.05);
           break;
       case Qt::Key_Minus:
           Zoom(0.05);
           break;
       }

       if (esc->bolaBlanca!=NULL){
          esc->bolaBlanca->aplicaTG(m);

          if(cameraActual == false){
              esc->setVRPCamera(false, point4(ctrB.x, ctrB.y, ctrB.z, 1.0));
              esc->camFirstP.vs.obs = vec4(ctrB.x, 0.0307474, cb.pmin.z + (cb.p/2.0) + 0.1, 1.0);
              esc->camFirstP.piram.dpost = esc->camFirstP.vs.obs.z - esc->capsaMinima.pmin.z + 0.1;
          }
       }

       update();
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    // Metode a implementar en el cas que es mogui la bola

}

void GLWidget::Zoom (double inOut) {
     esc->camGeneral.AmpliaWindow(inOut);//aumentar el tamaño del window equivale a un zoom out(inOut positivo)
     esc->camGeneral.CalculaMatriuProjection();
     updateGL();
}

void GLWidget::Pan(double dx, double dy) {
    esc->camGeneral.wd.pmin.x = esc->camGeneral.wd.pmin.x + dx;
    esc->camGeneral.wd.pmin.y = esc->camGeneral.wd.pmin.y + dy;
    esc->camGeneral.CalculaMatriuProjection();
    updateGL();
}

void GLWidget::transition2First(){
    std::cout<<"\ntransition2First";

    z += 0.001;
    if(z>0.601){
        updateGL();
        timer->stop();
        delete timer;
        timer = NULL;
        return;
    }

    if(z<0.6){
        esc->camGeneral.vs.obs = vec4(0.0, 20.0 - 33.282087667 * z, z, 1.0);
        esc->camGeneral.vs.vup = vec4(0.0, z/0.6, -1.0+(z/0.6), 0.0);
        esc->camGeneral.CalculaMatriuModelView();
    }else{
        cameraActual = false;//camFirstP
    }

    updateGL();
}

void GLWidget::transition2General(){
    std::cout<<"\ntransition2General";

    if(z>0.4){
       z -= 0.001;
    }else{
       z -= 0.2;
    }

    if(z<-0.001){
        updateGL();
        timer1->stop();
        delete timer1;
        timer1 = NULL;
        return;
    }

    esc->camGeneral.vs.obs = vec4(0.0, 20.0 - 33.282087667 * z, z, 1.0);
    esc->camGeneral.vs.vup = vec4(0.0, z/0.6, -1.0+(z/0.6), 0.0);
    esc->camGeneral.CalculaMatriuModelView();

    updateGL();
}

void GLWidget::adaptaObjecteTamanyWidget(Objecte *obj)
{

}

void GLWidget::newObjecte(Objecte * obj)
{
    point4 vrp;

    esc->addObjecte(obj);
    esc->CapsaMinCont3DEscena();
    vrp[0] = esc->capsaMinima.pmin[0]+(esc->capsaMinima.a/2.0);
    vrp[1] = esc->capsaMinima.pmin[1]+(esc->capsaMinima.h/2.0);
    vrp[2] = esc->capsaMinima.pmin[2]+(esc->capsaMinima.p/2.0);
    vrp[3] = 1.0;
    esc->setVRPCamera(cameraActual, vrp);
    if(cameraActual == true){
        esc->camGeneral.CalculWindow(esc->capsaMinima);
        esc->camGeneral.AmpliaWindow(-0.1);
    }else{
        esc->camFirstP.vs.vrp = point4(0.0, 0.0307474, 0.5, 1.0);//por si apretamos B antes de cargar obj
    }


    updateGL();
}

void GLWidget::newPlaBase()
{
    PlaBase *plaBase;

    plaBase = this->newPlaBs();
    cT = plaBase->calculCapsa3D();//para el calculo de colisiones
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
}

void GLWidget::newConjuntBoles()
{
    point4 vrp;
    ConjuntBoles *conjuntboles = new ConjuntBoles();

    esc->conjuntBoles = conjuntboles;
    for(int i=0; i<conjuntboles->listaConjuntBoles.size(); i++){
        esc->listaObjectes.push_back(conjuntboles->listaConjuntBoles[i]);
        listaCapsasConjuntBoles.push_back(conjuntboles->listaConjuntBoles[i]->calculCapsa3D());
    }

    esc->CapsaMinCont3DEscena();
    vrp[0] = esc->capsaMinima.pmin[0]+(esc->capsaMinima.a/2.0);
    vrp[1] = esc->capsaMinima.pmin[1]+(esc->capsaMinima.h/2.0);
    vrp[2] = esc->capsaMinima.pmin[2]+(esc->capsaMinima.p/2.0);
    vrp[3] = 1.0;
    esc->setVRPCamera(cameraActual, vrp);
    if(cameraActual == true){
        esc->camGeneral.CalculWindow(esc->capsaMinima);
        esc->camGeneral.AmpliaWindow(-0.1);
    }else{
        esc->camFirstP.vs.vrp = point4(0.0, 0.0307474, 0.5, 1.0);
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

        ConjuntBoles *conjuntboles = new ConjuntBoles();
        esc->conjuntBoles = conjuntboles;
        for(int i=0; i<conjuntboles->listaConjuntBoles.size(); i++){
            esc->listaObjectes.push_back(conjuntboles->listaConjuntBoles[i]);
            listaCapsasConjuntBoles.push_back(conjuntboles->listaConjuntBoles[i]->calculCapsa3D());
        }

        esc->CapsaMinCont3DEscena();
        point4 vrp;
        vrp[0] = esc->capsaMinima.pmin[0]+(esc->capsaMinima.a/2.0);
        vrp[1] = esc->capsaMinima.pmin[1]+(esc->capsaMinima.h/2.0);
        vrp[2] = esc->capsaMinima.pmin[2]+(esc->capsaMinima.p/2.0);
        vrp[3] = 1.0;

        esc->camGeneral.vs.vrp = point4(vrp[0], vrp[1] , vrp[2], 1.0);

        esc->camGeneral.CalculWindow(esc->capsaMinima);
        esc->camGeneral.AmpliaWindow(-0.1);

        esc->camFirstP.vs.vrp = point4(0.0, 0.0307474, 0.5, 1.0);

        updateGL();
    }

// Metode per iniciar la dinàmica del joc
void GLWidget::Play()
{

}
