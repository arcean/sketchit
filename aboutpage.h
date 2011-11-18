#ifndef ABOUTPAGE_H
#define ABOUTPAGE_H

#include <MApplicationPage>
#include <MButton>
#include <QSettings>

class AboutPage : public MApplicationPage {
    Q_OBJECT
public:
    AboutPage(QGraphicsItem *parent = 0);

protected:
    virtual void createContent();

private slots:

private:

};

#endif // ABOUTPAGE_H
