#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include <QGraphicsWidget>
#include <MImageWidget>

class ToolWidget : public MImageWidget
{
    Q_OBJECT
public:
    explicit ToolWidget(int tool, QString id);
    explicit ToolWidget(int tool, const QPixmap &pixmap, const QPixmap &pixmap_dimmed);

    QRectF boundingRect() const;

signals:
    void clicked(int tool);

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

public slots:

private:
    int tool;
    QPixmap pixmap;
    QPixmap pixmap_dimmed;

};

#endif // TOOLWIDGET_H
