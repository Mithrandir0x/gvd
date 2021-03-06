#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <math.h>
#include <escena.h>
#include <bola.h>
#include <conjuntboles.h>

#include <QtWidgets>
#include <QGLWidget>
#include <QTime>

#define ROTATIONSPEED 8

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_COLOR_ATTRIBUTE 1

class QGLShaderProgram;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    bool cameraActual; //true para camara general

public slots:
    void newPlaBase();
    PlaBase* newPlaBs();
    void newObj(QString fichero);
    void newBola();
    void newConjuntBoles();
    void newSalaBillar();

    // Metode per iniciar la dinàmica del joc
    void Play();

    void transition2First();
    void transition2General();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void Zoom (double inOut);
    void Pan(double dx, double dy);


private:
    Escena *esc;

    double xRot;
    double yRot;
    double zRot;
    float  a, p, h;

    QColor clearColor;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;

    bool moviment;

    // Programa de la GPU
    QGLShaderProgram *program;
    mat4 m220 = Scale(2.0/20.0, 2.0/20.0, 2.0/20.0);
    double xRotOld;
    double yRotOld;
    double zRotOld;
    Capsa3D cb;
    Capsa3D cT;
    vec3 ctrB;
    vector<Capsa3D> listaCapsasConjuntBoles;
    QTimer *timer;
    QTimer *timer1;
    double z;

    //  Metode per a carregar de fitxers el vertex i el fragment shader
   void InitShader( const char* vertexShaderFile,
                    const char* fragmentShaderFile );

   // Metode per inicialitzar els shaders de l'aplicacio
    void initShadersGPU();

    // Metode que carrega un nou objecte a l'escena
    void newObjecte(Objecte * obj);

    // Metode per adaptar l'escena al tamany del widget
    void adaptaObjecteTamanyWidget(Objecte *obj);

    void modcamFirst();
};


#endif // GLWIDGET_H
