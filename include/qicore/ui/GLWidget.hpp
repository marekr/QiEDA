#ifndef GLWIDGET_H_
#define GLWIDGET_H_

#include <QtOpenGL>
#include "qicore/qicore.hpp"

//forward decl as we dont want to expose nanovg
struct NVGcontext;

namespace qicore {
namespace ui {
    class QICORE_EXPORT GLWidget : public QOpenGLWidget {
    //todo fix this once we get MOC working...
    //Q_OBJECT

    public:
        GLWidget(QWidget *parent): QOpenGLWidget(parent) {}
        ~GLWidget();
        
    protected:
        void initializeGL() override;
        void resizeGL(int width, int height) override;
        void paintGL() override;
        
        struct NVGcontext* nanovg_;
    };
}
}
#endif