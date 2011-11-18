#ifndef COLORCELL_H
#define COLORCELL_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QColor>
#include <QPainter>

class ColorCell : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit ColorCell(QColor color, int width = 64, int height = 64, QGraphicsWidget *parent = 0);
    ~ColorCell();

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

private:
    QColor color;
    int width;
    int height;

};

#endif // COLORCELL_H
