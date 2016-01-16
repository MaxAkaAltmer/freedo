#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QWidget;
QT_END_NAMESPACE

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent);
    void ScreenShot(QString Name);    
    bool keep_aspect;

signals:
    void UpdateFrameComplited();

public slots:
    void BuffRedraw();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeGL ( int width, int height );

private:

};

#endif
