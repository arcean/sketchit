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
    explicit ToolWidget(int tool, const QPixmap &pixmap, const QPixmap &pixmap_dimmed, bool toggleable,
                        const QPixmap &pixmap_selected, const QPixmap &pixmap_selected_dimmed);
    QRectF boundingRect() const;

signals:
    void clicked(int tool);

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

public slots:

private:
    int tool;
    bool toggleable;
    bool toggled;
    QPixmap pixmap;
    QPixmap pixmap_dimmed;
    QPixmap pixmap_selected;
    QPixmap pixmap_selected_dimmed;

};

#endif // TOOLWIDGET_H
