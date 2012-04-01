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
    void storeAutoLoad();
    bool getAutoLoad();
    void storeFeedback();
    bool getFeedback();

    MButton *largeButton;
    MButton *mediumButton;
    MButton *smallButton;
    MButton *switchAutoLoad;
    MButton *switchFeedback;

};

#endif // SETTINGSPAGE_H
