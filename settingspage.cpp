#include <MLinearLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MButton>
#include <MLayout>
#include <MButtonGroup>
#include <QDebug>

#include "settingspage.h"

SettingsPage::SettingsPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{

}

void SettingsPage::createContent()
{
    this->setStyleName("CommonApplicationPageInverted");
    MLayout *layout = new MLayout;
    centralWidget()->setLayout(layout);
    MLinearLayoutPolicy *landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);

    MLabel *labelSize = new MLabel("Image size");
    labelSize->setStyleName("CommonFieldLabelInverted");
    //labelSize->setColor("black");

    /* Button group, exclusive for image size selection */
    MLayout *imageSizeLayout = new MLayout;
    MLinearLayoutPolicy *imageSizePolicy = new MLinearLayoutPolicy(imageSizeLayout, Qt::Horizontal);
    imageSizePolicy->setContentsMargins(0, 0, 0, 0);
    imageSizePolicy->setSpacing(0);
    imageSizePolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    smallButton = new MButton("Small");
    smallButton->setCheckable(true);
    smallButton->setViewType(MButton::groupType);
    smallButton->setStyleName("CommonLeftSplitButtonInverted");
    mediumButton = new MButton("Medium");
    mediumButton->setCheckable(true);
    mediumButton->setViewType(MButton::groupType);
    mediumButton->setStyleName("CommonHorizontalCenterSplitButtonInverted");
    largeButton = new MButton("Large");
    largeButton->setCheckable(true);
    largeButton->setViewType(MButton::groupType);
    largeButton->setStyleName("CommonRightSplitButtonInverted");

    MButtonGroup *imageSizeGroup = new MButtonGroup(this);
    imageSizeGroup->addButton(smallButton);
    imageSizeGroup->addButton(mediumButton);
    imageSizeGroup->addButton(largeButton);

    imageSizePolicy->addItem(smallButton, Qt::AlignCenter);
    imageSizePolicy->addItem(mediumButton, Qt::AlignCenter);
    imageSizePolicy->addItem(largeButton, Qt::AlignCenter);

    setImageSize(getImageSize());

    /* End of the button group */

    /* Switch for auto-load functionality */
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
  //  MLabel *labelNote1 = new MLabel("   Changes will be visible after creating");
   // MLabel *labelNote2 = new MLabel("   a new image.");
   // labelNote1->setFont(fFont);
   // labelNote2->setFont(fFont);

    QGraphicsLinearLayout *switchLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    switchLayout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    /* Let's see if we have auto-load enabled */
    bool autoload = getAutoLoad();
    switchAutoLoad = new MButton();
    switchAutoLoad->setViewType(MButton::switchType);
    switchAutoLoad->setStyleName("CommonRightSwitchInverted");
    switchAutoLoad->setCheckable(true);
    switchAutoLoad->setChecked(autoload);
    MLabel *spacer = new MLabel(this);
    MLabel *labelAutoLoad = new MLabel("Load recently used image at startup");
    labelAutoLoad->setStyleName("CommonFieldLabelInverted");
    switchLayout->addItem(switchAutoLoad);
    switchLayout->addItem(labelAutoLoad);
    switchLayout->setSpacing(0);
    switchLayout->setContentsMargins(0, 0, 0, 0);
    switchLayout->setAlignment(labelAutoLoad, Qt::AlignCenter);
    switchLayout->setAlignment(switchAutoLoad, Qt::AlignCenter);
    /* End of auto-load functionality */

    landscapePolicy->addItem(spacer, Qt::AlignCenter);
    landscapePolicy->addItem(labelSize, Qt::AlignCenter);
    landscapePolicy->addItem(imageSizeLayout, Qt::AlignCenter);
    landscapePolicy->addItem(labelInfo, Qt::AlignCenter);
    landscapePolicy->addItem(labelNote, Qt::AlignCenter);
    landscapePolicy->addItem(spacer, Qt::AlignCenter);
    landscapePolicy->addItem(switchLayout, Qt::AlignCenter);

    portraitPolicy->addItem(spacer, Qt::AlignCenter);
    portraitPolicy->addItem(labelSize, Qt::AlignCenter);
    portraitPolicy->addItem(imageSizeLayout, Qt::AlignCenter);
    portraitPolicy->addItem(labelInfo, Qt::AlignCenter);
    portraitPolicy->addItem(labelNote, Qt::AlignCenter);
    portraitPolicy->addItem(spacer, Qt::AlignCenter);
    portraitPolicy->addItem(switchLayout, Qt::AlignCenter);
   // portraitPolicy->addItem(labelNote2, Qt::AlignCenter);
}

void SettingsPage::dismissEvent(MDismissEvent *event)
{
    storeImageSize();
    storeAutoLoad();
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
