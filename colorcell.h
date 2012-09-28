#ifndef COLORCELL_H
#define COLORCELL_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QColor>
#include <QPainter>
#include <QVariantAnimation>
#include <QTimer>

#include "VariantAnimation.h"

class ColorCell : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit ColorCell(QColor color, int width = 64, int height = 64, bool isSelect = false, int id = 0, QGraphicsWidget *parent = 0);
    ~ColorCell();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void resizeCell(int width, int height);

    void setSelectedState(bool wasPressed);
    void setNormalState(bool forced);

protected:
    void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    virtual void resizeEvent (QGraphicsSceneResizeEvent * event);

signals:
    void signalClicked(QColor color, int id);
    void signalPressed(int id, bool selectIdToSelect);

public slots:
    void emitSignalPressed(bool selectIdToSelect);
    void emitSignalClicked();

private slots:
    void expandAnimation(const QVariant &value);

#ifdef ENABLE_SHAKE
    void shakeAnimationFunc(const QVariant &value);
    void launchShake();
#endif

private:
    int randInt(int low, int high);

    QColor color;
    int width;
    int height;
    int margin;
    bool isSelect;
    bool closeDialog;
    bool wasPressed;

#ifdef ENABLE_SHAKE
    int dX, dY;
    QTimer *shakeTimer;
    VariantAnimator *shakeAnimation;
#endif
    VariantAnimator *showAnimation;
    int id;

};

#endif // COLORCELL_H
