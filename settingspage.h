#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <MApplicationPage>
#include <MButton>
#include <QSettings>

#include "settings.h"

class SettingsPage : public MApplicationPage {
    Q_OBJECT
public:
    SettingsPage(QGraphicsItem *parent = 0);

protected:
    virtual void createContent();
    virtual void dismissEvent(MDismissEvent *event);

signals:
    void settingsChanged();

private slots:
    void fullscreenToggled(bool toggled);
    void autoLoadToggled(bool toggled);
    void feedbackToggled(bool toggled);

private:
    void setImageSize();
    void selectButtonImageSize(int size);

    MButton *largeButton;
    MButton *mediumButton;
    MButton *smallButton;
    MButton *switchAutoLoad;
    MButton *switchFeedback;
    MButton *fullscreenButton;

    Settings *settings;

};

#endif // SETTINGSPAGE_H
