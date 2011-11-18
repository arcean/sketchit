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

signals:
    void clicked(int tool);

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

public slots:

private:
    int tool;

};

#endif // TOOLWIDGET_H
