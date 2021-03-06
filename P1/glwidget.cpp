

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

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + ROTATIONSPEED * dy);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + ROTATIONSPEED * dy);
        setZRotation(zRot + ROTATIONSPEED * dx);
    }
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

    /*
     * Para cada dirección de desplazamiento, calculamos cual va a ser la cantidad de espacio que
     * se va a desplazar en dicha dirección. Sin embargo, para no estar obligados a tener en cuenta
     * la rotación de la escena en este punto, los desplazamientos se hacen de manera que están
     * sobre el plano XZ, sin que haya mediado ninguna transformación de por medio.
     */
    double deltaDesplacament = 0.01;
    double dzP=deltaDesplacament, dzN=-deltaDesplacament, dxP=deltaDesplacament, dxN=-deltaDesplacament;

    mat4 m;
    Capsa3D capsaBolaB, capBi;

    capsaBolaB = esc->bolaBlanca->calculCapsa3D();

    /*
     * Verificación de la posición de la bola dentro de los límite de Escena. La variable cT contiene
     * la capsa3D con el tamaño de toda la escena, lo cual nos permite calcular si al aplicar un
     * desplazamiento sobre la bola blanca, ésta se saldrá de los límites que impone cT.
     */
    if(cT.pmin.z - cb.pmin.z > - deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dzN) < deltaDesplacament en borde de mesa con z negativos
        dzN = cT.pmin.z - cb.pmin.z;
    if(cT.pmin.z + cT.p - (cb.pmin.z + cb.p) < deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dzP) < deltaDesplacament en borde de mesa con z positivos
        dzP = cT.pmin.z + cT.p - (cb.pmin.z + cb.p);
    if(cT.pmin.x - cb.pmin.x > -deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dxN) < deltaDesplacament en borde de mesa con x negativos
        dxN = cT.pmin.x - cb.pmin.x;
    if(cT.pmin.x + cT.a - (cb.pmin.x + cb.a) < deltaDesplacament)//si bola blanca a distancia <deltaDesplacament ->abs(dxN) < deltaDesplacament en borde de mesa con x positivos
        dxP = cT.pmin.x + cT.a - (cb.pmin.x + cb.a);

    /*
     * Para cada bola de ConjuntBoles, calculamos la posible colisión con la bola blanca, para evitar
     * que la bola blanca atraviese las otras bolas. De producirse, no permitimos que la bola se desplace.
     */
    for (int i=0; i<esc->conjuntBoles->listaConjuntBoles.size(); i++) {//comparamos bola blanca con resto de bolas

        /*
         * Para evitar el tener que calcular constantemente la colisión, de las bolas, miramos antes de hacer cualquier
         * otro cálculo, si la bola está dentro de la capsa3d de la bola con la que va a colisionar
         */
        if(abs((cb.pmin.x+cb.a/2.) - (listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a /2.)) < (cb.a/2. + listaCapsasConjuntBoles[i].a/2.)
                && abs((cb.pmin.z+cb.p/2.) - (listaCapsasConjuntBoles[i].pmin.z+listaCapsasConjuntBoles[i].p/2.)) < cb.p/2. + listaCapsasConjuntBoles[i].p/2.) {

            /*
             * De ser así, para cada tipo de tecla, miramos que en el eje sobre el que se desplaza la bola blanca (eje z si se desplaza arriba o abajo,
             * eje x si se desplaza hacia la izquierda o a la derecha) intenta sobrepasar la capsa3d de la bola i.
             *
             * De intentar hacer un desplazamiento sobre ese eje, en el caso que esté a menor distancia que la distancia por defecto que se aplica,
             * se procederá a calcular la distancia que queda entre la bola blanca y la bola i.
             *
             * Si se intenta ir más allá en dicha dirección, no será posible.
             */
            if(event->key() == Key_Up){
                if((cb.pmin.z + cb.p/2.0) - (listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p/2.0)<= 0.0){//si bola blanca con menor z que la i hay limitacion
                    if(listaCapsasConjuntBoles[i].pmin.z - cb.pmin.z - cb.p < deltaDesplacament){//otherwise yes
                        dzP = listaCapsasConjuntBoles[i].pmin.z - cb.pmin.z - cb.p;
                        if(dzP < 0.005)dzP = 0.0;
                    }
                }else{
                    dzP = deltaDesplacament;//si bola blanca tiene mayor z que la i no hay limitacion
                }
            }

            if(event->key() == Key_Down){
                if((cb.pmin.z + cb.p/2.0) - (listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p/2.0)>= 0.0){//si bola blanca con mayor z que la i, no hay limitacion
                    if(listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p - cb.pmin.z > -deltaDesplacament){//otherwise yes
                         dzN = listaCapsasConjuntBoles[i].pmin.z + listaCapsasConjuntBoles[i].p - cb.pmin.z;
                         if(dzN > -0.005)dzN = 0.0;
                    }   else{
                           dzN = -deltaDesplacament;
                        }
                 }
            }

            if(event->key() == Key_Left){
                if((cb.pmin.x + cb.a/2.)- (listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a/2.)<= 0.0){//si bola blanca a la izquierda no hay limitacion
                    dxN = -deltaDesplacament;
                }else{
                    if(listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a - cb.pmin.x > -deltaDesplacament){//otherwise yes
                        dxN = listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a - cb.pmin.x;
                        if(dxN > -0.002)dxN = 0.0;
                    }
                }
            }

            if(event->key() == Key_Right){
                if((cb.pmin.x + cb.a/2.)- (listaCapsasConjuntBoles[i].pmin.x + listaCapsasConjuntBoles[i].a/2.) >= 0.0){//si bola blanca a la derecha no hay limitacion
                    dxP = deltaDesplacament;
                }else{
                    if(listaCapsasConjuntBoles[i].pmin.x - (cb.pmin.x + cb.a) < deltaDesplacament ){//otherwise yes
                        dxP = listaCapsasConjuntBoles[i].pmin.x - (cb.pmin.x + cb.a);
                        if(dxP < 0.002)dxP = 0.0;
                    }
                }
            }
        }
    }

    /*
     * Una vez ya se han calculado cuales serán los desplazamientos,
     * aplicamos una translación, pero teniendo en cuenta cómo están
     * actualmente los ejes de coordenadas.
     */
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
    /*Capsa3D capsa;
    mat4 m;

    obj->aplicaTG(m220); //m220 precalculada en glwidget.h con escala=2/20
    capsa = obj->calculCapsa3D();
        if (dynamic_cast<TaulaBillar*>(obj)){
            m = Translate(0.0,  -capsa.pmin.y - capsa.h, 0.0);//no es estrictamente necesario
            obj->aplicaTG(m);
        }else if (dynamic_cast<Bola*>(obj)){
            m = Translate(0.0,  -capsa.pmin.y, 0.0);//la base de las 16 bolas quedan en y = 0
            obj->aplicaTG(m);
    }*/
    //la PlaBase estaba en y = 0 por lo que solo necesita el escalado m220
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
    PlaBase *plaBase = new PlaBase();
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
