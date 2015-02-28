#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "cub.h"

#include <QtOpenGL>
#include <QGLWidget>
#include <QtGui>


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

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    cub *c;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;


    QGLShaderProgram *program;

    // Programa de la GPU
    //GLuint program;

    //  Helper function to load vertex and fragment shader files
    void InitShader( const char* vertexShaderFile,
                       const char* fragmentShaderFile );

    void initShadersGPU();

};


#endif // GLWIDGET_H
