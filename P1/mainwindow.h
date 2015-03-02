#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <glwidget.h>

QT_BEGIN_NAMESPACE
class QSlider;
QT_END_NAMESPACE
//! [0]
//!
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void Quit();
    void newFile();

private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;



};

#endif // MAINWINDOW_H
