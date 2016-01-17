#ifdef __APPLE__
#include <glu.h>
#include <glext.h>
#else
#include <GL/glu.h>
#include <GL/glext.h>
#endif

#include <QtGui>
#include "glwidget.h"
#include "emuthread.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    setAutoFillBackground(false);    
}

void GLWidget::BuffRedraw()
{    
    paintEvent(NULL);    
}

void GLWidget::resizeGL ( int width, int height )
{    
    glScissor(0,0,width,height);

    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    paintEvent(NULL);
}

void GLWidget::paintEvent(QPaintEvent *event)
{
 double ax,ay,asp;
 int xoff,yoff;

    makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ax=((double)width())/640.0;
    ay=((double)height())/480.0;

    if(ax<ay)asp=ax;
    else asp=ay;

    if(keep_aspect)
    {
        xoff=((double)width()-640.0*asp)/2;
        yoff=((double)height()-480.0*asp)/2;
        glPixelZoom(asp,asp);
        glRasterPos2i(xoff, yoff);

    }
    else
    {
         glPixelZoom(ax,ay);
         glRasterPos2i(0,0);
    }

    glDrawPixels(
                    640,
                    480,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    _emulator_fb()
                );

    swapBuffers();

    emit UpdateFrameComplited();

}

void GLWidget::ScreenShot(QString Name)
{
 double ax,ay,asp;
 int xoff,yoff;

    QImage img=this->grabFrameBuffer(false);

    if(keep_aspect)
    {
        ax=((double)width())/640.0;
        ay=((double)height())/480.0;
        if(ax<ay)asp=ax;
        else asp=ay;
        xoff=((double)width()-640.0*asp)/2;
        yoff=((double)height()-480.0*asp)/2;

        img=img.copy(xoff,yoff,asp*640.0,asp*480);

    }
    img.save(Name);
}
