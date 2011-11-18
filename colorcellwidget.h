#ifndef COLORCELLWIDGET_H
#define COLORCELLWIDGET_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QColor>
#include <QPainter>
#include <MWidget>

class ColorCellWidget : public MWidget
{
    Q_OBJECT
public:
    explicit ColorCellWidget(QColor color, int width = 48, int height = 48, QGraphicsWidget *parent = 0);
    ~ColorCellWidget();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void resizeCell(int width, int height);

protected:
  //  void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    virtual void resizeEvent (QGraphicsSceneResizeEvent * event);

signals:
    void signalClicked(QColor color);
    void signalPressed();

public slots:
    void emitSignalPressed();
    void emitSignalClicked();
    void setColor(QColor color);

private:
    QColor color;
    int width;
    int height;

};

#endif // COLORCELLWIDGET_H
