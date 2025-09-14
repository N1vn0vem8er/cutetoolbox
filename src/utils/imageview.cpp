#include "imageview.h"
#include <qevent.h>
#include <qgraphicsitem.h>

ImageView::ImageView(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    group = new QGraphicsItemGroup();
    item = new QGraphicsPixmapItem();
    group->addToGroup(item);
    scene->addItem(group);
    setScene(scene);
    ensureVisible(scene->sceneRect());
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void ImageView::setImage(const QImage &image)
{
    item->setPixmap(QPixmap::fromImage(image));
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier)
    {
        const ViewportAnchor archor = transformationAnchor();
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        int angle = event->angleDelta().y();
        double factor;
        if(angle > 0)
        {
            factor = 1.1;
        }
        else
        {
            factor = 0.9;
        }
        scale(factor, factor);
        setTransformationAnchor(archor);
    }
    else
        QGraphicsView::wheelEvent(event);
}
