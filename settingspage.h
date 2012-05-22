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

signals:
    void settingsChanged();

private:
    int getImageSize();
    void setImageSize(int size);
    void storeImageSize();
    void storeFullscreen();
    bool getFullscreen();
    void storeAutoLoad();
    bool getAutoLoad();
    void storeFeedback();
    bool getFeedback();

    MButton *largeButton;
    MButton *mediumButton;
    MButton *smallButton;
    MButton *switchAutoLoad;
    MButton *switchFeedback;
    MButton *fullscreenButton;

};

#endif // SETTINGSPAGE_H
