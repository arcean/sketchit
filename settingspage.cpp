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
#include <MApplication>

#include "settingspage.h"
#include "ViewHeader.h"
#include "Singleton.h"

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

    policy->addItem(header);
    policy->addItem(pannableCentralViewport, Qt::AlignCenter);

    MContainer *imageSizeBox = new MContainer();
    imageSizeBox->setStyleName("CommonContainerInverted");
    imageSizeBox->setTitle("Image size");
    imageSizeBox->setMaximumHeight(260);

    /* Button group, exclusive for image size selection */
    MLayout *imageSizeLayout = new MLayout;
    MLinearLayoutPolicy *imageSizePolicyLandscape = new MLinearLayoutPolicy(imageSizeLayout, Qt::Horizontal);
    imageSizePolicyLandscape->setContentsMargins(0, 0, 0, 0);
    imageSizePolicyLandscape->setSpacing(0);
    imageSizePolicyLandscape->setNotifyWidgetsOfLayoutPositionEnabled(true);

    MLinearLayoutPolicy *imageSizePolicyPortrait = new MLinearLayoutPolicy(imageSizeLayout, Qt::Vertical);
    imageSizePolicyPortrait->setContentsMargins(0, 0, 0, 0);
    imageSizePolicyPortrait->setSpacing(0);
    imageSizePolicyPortrait->setNotifyWidgetsOfLayoutPositionEnabled(true);

    imageSizeLayout->setPortraitPolicy(imageSizePolicyPortrait);
    imageSizeLayout->setLandscapePolicy(imageSizePolicyLandscape);

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

    imageSizePolicyLandscape->addItem(smallButton, Qt::AlignCenter);
    imageSizePolicyLandscape->addItem(mediumButton, Qt::AlignCenter);
    imageSizePolicyLandscape->addItem(largeButton, Qt::AlignCenter);
    imageSizePolicyPortrait->addItem(smallButton, Qt::AlignCenter);
    imageSizePolicyPortrait->addItem(mediumButton, Qt::AlignCenter);
    imageSizePolicyPortrait->addItem(largeButton, Qt::AlignCenter);

    // CREATE SETTINGS CLASS
    settings = &Singleton<Settings>::Instance();

    // Select approperiate button fromt the imageSizeGroup
    selectButtonImageSize(settings->getImageSize());

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
    otherLayout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    otherLayout->setContentsMargins(0, 0, 0, 0);

    /* Switch for auto-load functionality */
    QGraphicsLinearLayout *switchLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    switchLayout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    /* Let's see if we have auto-load enabled */
    bool autoload = settings->getAutoLoad();
    switchAutoLoad = new MButton();
    switchAutoLoad->setViewType(MButton::switchType);
    switchAutoLoad->setObjectName("CommonRightSwitchInverted");
    switchAutoLoad->setCheckable(true);
    switchAutoLoad->setChecked(autoload);
    MLabel *labelAutoLoad = new MLabel("Load recently used image at startup");
    labelAutoLoad->setStyleName("CommonFieldLabelInverted");
    labelAutoLoad->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    switchLayout->addItem(labelAutoLoad);
    switchLayout->addItem(switchAutoLoad);
    switchLayout->setAlignment(switchAutoLoad, Qt::AlignCenter);
    connect(switchAutoLoad, SIGNAL(toggled(bool)), this, SLOT(autoLoadToggled(bool)));
    otherLayout->addItem(switchLayout);
    /* End of auto-load functionality */

    /* Switch for feedback functionality */
    switchLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    switchLayout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    /* Let's see if we have feedback enabled */
    bool feedback = settings->getFeedback();
    switchFeedback = new MButton();
    switchFeedback->setViewType(MButton::switchType);
    switchFeedback->setObjectName("CommonRightSwitchInverted");
    switchFeedback->setCheckable(true);
    switchFeedback->setChecked(feedback);
    MLabel *labelFeedback = new MLabel("Enable haptic feedback");
    labelFeedback->setStyleName("CommonFieldLabelInverted");
    labelFeedback->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    switchLayout->addItem(labelFeedback);
    switchLayout->addItem(switchFeedback);
    switchLayout->setAlignment(switchFeedback, Qt::AlignCenter);
    connect(switchFeedback, SIGNAL(toggled(bool)), this, SLOT(feedbackToggled(bool)));
    otherLayout->addItem(switchLayout);

    /* Fullscreen mode switch */
    switchLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    switchLayout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    bool fullscreen = settings->getFullscreen();
    fullscreenButton = new MButton();
    fullscreenButton->setViewType(MButton::switchType);
    fullscreenButton->setObjectName("CommonRightSwitchInverted");
    fullscreenButton->setCheckable(true);
    fullscreenButton->setChecked(fullscreen);
    MLabel *labelFullscreen = new MLabel("Enable fullscreen mode");
    labelFullscreen->setStyleName("CommonFieldLabelInverted");
    labelFullscreen->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    switchLayout->addItem(labelFullscreen);
    switchLayout->addItem(fullscreenButton);
    switchLayout->setAlignment(fullscreenButton, Qt::AlignCenter);
    connect(fullscreenButton, SIGNAL(toggled(bool)), this, SLOT(fullscreenToggled(bool)));
    otherLayout->addItem(switchLayout);

    QGraphicsWidget *otherBoxWidget = new QGraphicsWidget();
    otherBoxWidget->setLayout(otherLayout);
    otherBox->setCentralWidget(otherBoxWidget);
    /* End of feedback functionality */

    viewportLayoutPolicy->addItem(imageSizeBox, Qt::AlignCenter);
    viewportLayoutPolicy->addItem(otherBox, Qt::AlignCenter);
}

void SettingsPage::dismissEvent(MDismissEvent*)
{
    setImageSize();
    emit settingsChanged();
}

void SettingsPage::autoLoadToggled(bool toggled)
{
    settings->setAutoLoad(toggled);
}

void SettingsPage::feedbackToggled(bool toggled)
{
    settings->setFeedback(toggled);
}

void SettingsPage::fullscreenToggled(bool toggled)
{
    /* Fullscreen mode */
    MWindow *window = MApplication::activeWindow();
    if (toggled)
        window->showFullScreen();
    else
        window->showNormal();

    settings->setFullscreen(toggled);
}

void SettingsPage::setImageSize()
{
    int value = 2;

    if(smallButton->isChecked())
        value = 0;
    else if(mediumButton->isChecked())
        value = 1;

    settings->setImageSize(value);
}

void SettingsPage::selectButtonImageSize(int size)
{
    if(size == 0)
        smallButton->setChecked(true);
    else if(size == 1)
        mediumButton->setChecked(true);
    else
        largeButton->setChecked(true);
}
