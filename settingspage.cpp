#include <MLinearLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MButton>
#include <MLayout>
#include <MButtonGroup>
#include <MPannableViewport>
#include <MPositionIndicator>
#include <MSeparator>
#include <MApplicationWindow>
#include <MContainer>

#include "settingspage.h"

class ViewHeader : public MWidgetController
{
public:
    ViewHeader(QGraphicsItem *parent = 0) :
        MWidgetController(parent),
        linearLayout(0),
        titleWidget(0)
    {
        setObjectName("CommonHeaderPanel");
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        linearLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
        titleWidget = new MLabel(this);
        titleWidget->setTextElide(true);
        titleWidget->setStyleName("CommonBodyTextInvertedBig");
        titleWidget->setAlignment(Qt::AlignCenter);
        linearLayout->addItem(titleWidget);
    }

    void setTitle(const QString &title)
    {
        titleWidget->setText(title);
    }

private:
    QGraphicsLinearLayout *linearLayout;
    MLabel *titleWidget;

};

SettingsPage::SettingsPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{

}

void SettingsPage::createContent()
{
    this->setStyleName("CommonApplicationPageInverted");
    MLayout *layout = new MLayout;
    MPannableViewport *pannableCentralViewport = new MPannableViewport;
    MLayout *viewportLayout = new MLayout;
    MLinearLayoutPolicy *viewportLayoutPolicy = new MLinearLayoutPolicy(viewportLayout, Qt::Vertical);
    QGraphicsWidget *form = new QGraphicsWidget();

    viewportLayout->setPolicy(viewportLayoutPolicy);
    viewportLayoutPolicy->setContentsMargins(0, 0, 0, 0);
    viewportLayoutPolicy->setSpacing(0);
    viewportLayoutPolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    form->setLayout(viewportLayout);
    pannableCentralViewport->setWidget(form);
    pannableCentralViewport->positionIndicator()->setVisible(false);

    centralWidget()->setLayout(layout);

    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);
    policy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    layout->setPolicy(policy);

    /* Fix: lock page content */
    setPannable(false);
    MPannableViewport *viewport = this->pannableViewport();
    viewport->setAutoRange(false);
    viewport->setRange(QRectF(0,0,0,0));

    ViewHeader *header = new ViewHeader;
    header->setTitle("Settings");

    MContainer *imageSizeBox = new MContainer();
    imageSizeBox->setStyleName("CommonContainerInverted");
    imageSizeBox->setTitle("Image size");
    imageSizeBox->setMaximumHeight(260);

    /* Button group, exclusive for image size selection */
    MLayout *imageSizeLayout = new MLayout;
    MLinearLayoutPolicy *imageSizePolicy = new MLinearLayoutPolicy(imageSizeLayout, Qt::Horizontal);
    imageSizePolicy->setContentsMargins(0, 0, 0, 0);
    imageSizePolicy->setSpacing(0);
    imageSizePolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);

    MLinearLayoutPolicy *imageSizePolicyPortrait = new MLinearLayoutPolicy(imageSizeLayout, Qt::Vertical);
    imageSizePolicyPortrait->setContentsMargins(0, 0, 0, 0);
    imageSizePolicyPortrait->setSpacing(0);
    imageSizePolicyPortrait->setNotifyWidgetsOfLayoutPositionEnabled(true);

    imageSizeLayout->setPortraitPolicy(imageSizePolicyPortrait);
    imageSizeLayout->setLandscapePolicy(imageSizePolicy);

    smallButton = new MButton("Small");
    smallButton->setCheckable(true);
    smallButton->setViewType(MButton::groupType);
    smallButton->setStyleName("CommonTopSplitButtonInverted");
    mediumButton = new MButton("Medium");
    mediumButton->setCheckable(true);
    mediumButton->setViewType(MButton::groupType);
    mediumButton->setStyleName("CommonVerticalCenterSplitButtonInverted");
    largeButton = new MButton("Large");
    largeButton->setCheckable(true);
    largeButton->setViewType(MButton::groupType);
    largeButton->setStyleName("CommonBottomSplitButtonInverted");

    MButtonGroup *imageSizeGroup = new MButtonGroup(this);
    imageSizeGroup->addButton(smallButton);
    imageSizeGroup->addButton(mediumButton);
    imageSizeGroup->addButton(largeButton);

    imageSizePolicy->addItem(smallButton, Qt::AlignCenter);
    imageSizePolicy->addItem(mediumButton, Qt::AlignCenter);
    imageSizePolicy->addItem(largeButton, Qt::AlignCenter);
    imageSizePolicyPortrait->addItem(smallButton, Qt::AlignCenter);
    imageSizePolicyPortrait->addItem(mediumButton, Qt::AlignCenter);
    imageSizePolicyPortrait->addItem(largeButton, Qt::AlignCenter);

    setImageSize(getImageSize());

    QGraphicsLinearLayout *imageSizeLayoutH = new QGraphicsLinearLayout(Qt::Horizontal);
    imageSizeLayoutH->addStretch();
    imageSizeLayoutH->addItem(imageSizeLayout);
    imageSizeLayoutH->addStretch();

    QFont fBold;
    QFont fFont;
    fBold.setBold(true);
    fBold.setPointSize(12);
    fFont.setPointSize(12);
    MLabel *labelInfo = new MLabel("Information:");
    labelInfo->setStyleName("CommonFieldLabelInverted");
    labelInfo->setFont(fBold);
    MLabel *labelNote = new MLabel("   Changes will be visible after creating a new image.");
    labelNote->setStyleName("CommonFieldLabelInverted");
    labelNote->setFont(fFont);

    QGraphicsLinearLayout *imageSizeMasterLayout = new QGraphicsLinearLayout(Qt::Vertical);
    imageSizeMasterLayout->setContentsMargins(0, 0, 0, 0);
    imageSizeMasterLayout->setSpacing(0);
    imageSizeMasterLayout->addItem(imageSizeLayoutH);
    imageSizeMasterLayout->addItem(labelInfo);
    imageSizeMasterLayout->addItem(labelNote);

    QGraphicsWidget *imageContainerWidget = new QGraphicsWidget();
    imageContainerWidget->setLayout(imageSizeMasterLayout);
    imageSizeBox->setCentralWidget(imageContainerWidget);

    /* End of the button group */

    MContainer *otherBox = new MContainer();
    otherBox->setStyleName("CommonContainerInverted");
    otherBox->setTitle("Other");
    QGraphicsLinearLayout *otherLayout = new QGraphicsLinearLayout(Qt::Vertical);

    /* Switch for auto-load functionality */
    QGraphicsLinearLayout *switchLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    switchLayout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    /* Let's see if we have auto-load enabled */
    bool autoload = getAutoLoad();
    switchAutoLoad = new MButton();
    switchAutoLoad->setViewType(MButton::switchType);
    switchAutoLoad->setObjectName("CommonRightSwitchInverted");
    switchAutoLoad->setCheckable(true);
    switchAutoLoad->setChecked(autoload);
    MLabel *labelAutoLoad = new MLabel("Load recently used image at startup");
    labelAutoLoad->setStyleName("CommonFieldLabelInverted");
    switchLayout->addItem(labelAutoLoad);
    switchLayout->addItem(switchAutoLoad);
    switchLayout->setSpacing(0);
    switchLayout->setContentsMargins(0, 0, 0, 0);
    switchLayout->setAlignment(labelAutoLoad, Qt::AlignCenter);
    switchLayout->setAlignment(switchAutoLoad, Qt::AlignCenter);
    /* End of auto-load functionality */

    /* Switch for feedback functionality */
    QGraphicsLinearLayout *switchLayout2 = new QGraphicsLinearLayout(Qt::Horizontal);
    switchLayout2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    /* Let's see if we have feedback enabled */
    bool feedback = getFeedback();
    switchFeedback = new MButton();
    switchFeedback->setViewType(MButton::switchType);
    switchFeedback->setObjectName("CommonRightSwitchInverted");
    switchFeedback->setCheckable(true);
    switchFeedback->setChecked(feedback);
    MLabel *labelFeedback = new MLabel("Enable haptic feedback");
    labelFeedback->setStyleName("CommonFieldLabelInverted");
    switchLayout2->addItem(labelFeedback);
    switchLayout2->addItem(switchFeedback);
    switchLayout2->setSpacing(0);
    switchLayout2->setContentsMargins(0, 0, 0, 0);
    switchLayout2->setAlignment(labelFeedback, Qt::AlignCenter);
    switchLayout2->setAlignment(switchFeedback, Qt::AlignCenter);

    /* Fullscreen mode switch */
    QGraphicsLinearLayout *switchLayout3 = new QGraphicsLinearLayout(Qt::Horizontal);
    switchLayout3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    bool fullscreen = getFullscreen();
    fullscreenButton = new MButton();
    fullscreenButton->setViewType(MButton::switchType);
    fullscreenButton->setObjectName("CommonRightSwitchInverted");
    fullscreenButton->setCheckable(true);
    fullscreenButton->setChecked(fullscreen);
    MLabel *labelFullscreen = new MLabel("Enable fullscreen mode");
    labelFullscreen->setStyleName("CommonFieldLabelInverted");
    switchLayout3->addItem(labelFullscreen);
    switchLayout3->addItem(fullscreenButton);
    switchLayout3->setSpacing(0);
    switchLayout3->setContentsMargins(0, 0, 0, 0);
    switchLayout3->setAlignment(labelFullscreen, Qt::AlignCenter);
    switchLayout3->setAlignment(fullscreenButton, Qt::AlignCenter);

    otherLayout->addItem(switchLayout);
    otherLayout->addItem(switchLayout2);
    otherLayout->addItem(switchLayout3);

    QGraphicsWidget *otherBoxWidget = new QGraphicsWidget();
    otherBoxWidget->setLayout(otherLayout);
    otherBox->setCentralWidget(otherBoxWidget);
    /* End of feedback functionality */

    viewportLayoutPolicy->addItem(imageSizeBox, Qt::AlignCenter);
    viewportLayoutPolicy->addItem(otherBox, Qt::AlignCenter);

    policy->addItem(header);
    policy->addItem(pannableCentralViewport, Qt::AlignCenter);
}

void SettingsPage::dismissEvent(MDismissEvent*)
{
    storeImageSize();
    storeAutoLoad();
    storeFeedback();
    storeFullscreen();
    emit settingsChanged();
}

void SettingsPage::storeFullscreen()
{
    QSettings settings;
    bool value;

    if(fullscreenButton->isChecked())
        value = true;
    else
        value = false;

    settings.setValue("common/fullscreen", value);
}

bool SettingsPage::getFullscreen()
{
    QSettings settings;
    bool value =  settings.value("common/fullscreen", false).toBool();

    return value;
}

void SettingsPage::storeAutoLoad()
{
    QSettings settings;
    bool value;

    if(switchAutoLoad->isChecked())
        value = true;
    else
        value = false;

    settings.setValue("startup/autoload", value);
}

bool SettingsPage::getAutoLoad()
{
    QSettings settings;
    bool value =  settings.value("startup/autoload", true).toBool();

    return value;
}

void SettingsPage::storeFeedback()
{
    QSettings settings;
    bool value;

    if(switchFeedback->isChecked())
        value = true;
    else
        value = false;

    settings.setValue("common/feedback", value);
}

bool SettingsPage::getFeedback()
{
    QSettings settings;
    bool value =  settings.value("common/feedback", true).toBool();

    return value;
}

void SettingsPage::storeImageSize()
{
    QSettings settings;
    int value;

    if(smallButton->isChecked())
        value = 0;
    else if(mediumButton->isChecked())
        value = 1;
    else
        value = 2;

    settings.setValue("image/size", value);
}

int SettingsPage::getImageSize()
{
    // 0-low 1-medium 2-high
    QSettings settings;
    int value =  settings.value("image/size", 1).toInt();
    if(value > 2 || value < 0)
        value = 1;
    return value;
}

void SettingsPage::setImageSize(int size)
{
    if(size == 0)
        smallButton->setChecked(true);
    else if(size == 1)
        mediumButton->setChecked(true);
    else
        largeButton->setChecked(true);
}
