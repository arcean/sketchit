#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <MApplicationPage>
#include <MButton>
#include <QSettings>

class SettingsPage : public MApplicationPage {
    Q_OBJECT
public:
    SettingsPage(QGraphicsItem *parent = 0);

protected:
    virtual void createContent();
    virtual void dismissEvent(MDismissEvent *event);

private slots:


private:
    int getImageSize();
    void setImageSize(int size);
    void storeImageSize();
    void storeAutoLoad();
    bool getAutoLoad();

    MButton *largeButton;
    MButton *mediumButton;
    MButton *smallButton;
    MButton *switchAutoLoad;

};

#endif // SETTINGSPAGE_H
