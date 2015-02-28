#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSlider;
QT_END_NAMESPACE
//! [0]
class GLWidget;


class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QSlider *createSlider();

    GLWidget *glWidget;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;
};

#endif // MAINWINDOW_H
