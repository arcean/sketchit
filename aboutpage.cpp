#include <MLinearLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MButton>
#include <MLayout>
#include <MButtonGroup>
#include <MImageWidget>
#include <QDebug>
#include <QFont>
#include <MTextEdit>
#include <MPannableViewport>
#include <MPositionIndicator>

#include "aboutpage.h"
#include "ViewHeader.h"

#define COPYRIGHT_TEXT_LINES 15

AboutPage::AboutPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{

}

void AboutPage::createContent()
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
    header->setTitle("About");

    policy->addItem(header);
    policy->addItem(pannableCentralViewport, Qt::AlignCenter);

    QPixmap logo("/opt/sketchit/data/sketchit128.png");
    MImageWidget *image = new MImageWidget();
    image->setPixmap(logo);
    MLabel *app_name = new MLabel("SketchIt! 1.3.8");
    app_name->setStyleName("CommonTitleInverted");
    app_name->setAlignment(Qt::AlignCenter);
    QFont label_font;
    label_font.setPointSize(32);
    app_name->setFont(label_font);

    const QChar CopyrightSymbol(0x00a9);

    QString *copyright_string = new QString(QString(CopyrightSymbol) + QString::fromUtf8(" 2011, 2012 Tomasz Pieniążek"));

    MLabel *copyright_title = new MLabel(*copyright_string);
    copyright_title->setStyleName("CommonFieldLabelInverted");
    label_font.setPointSize(20);
    copyright_title->setFont(label_font);
    copyright_title->setAlignment(Qt::AlignCenter);

    QString copyrightText = "\nThis program is free software; you can redistribute it and/or "
                            "modify it under the terms of the GNU General Public License "
                            "as published by the Free Software Foundation, either version "
                            "3 of the License, or (at your option) any later version.\n\n"
                            "This package is distributed in the hope that it will be useful, "
                            "but WITHOUT ANY WARRANTY; without even the implied "
                            "warranty of MERCHANTABILITY or FITNESS FOR A "
                            "PARTICULAR PURPOSE. See the GNU General Public "
                            "License for more details.\n\n"
                            "You should have received a copy of the GNU General Public "
                            "License along with this program. If not, see \n"
                            "http://www.gnu.org/licenses";

    QFont license_font;
    license_font.setPointSize(16);

    MLabel *copyrightTextLabel = new MLabel(copyrightText);
    copyrightTextLabel->setStyleName("CommonFieldLabelInverted");
    copyrightTextLabel->setFont(license_font);
    copyrightTextLabel->setWordWrap(true);

    MLabel *titleSpacer = new MLabel(" ");

    viewportLayoutPolicy->addItem(titleSpacer, Qt::AlignCenter);
    viewportLayoutPolicy->addItem(image, Qt::AlignCenter);
    viewportLayoutPolicy->addItem(app_name, Qt::AlignCenter);
    viewportLayoutPolicy->addItem(copyright_title, Qt::AlignCenter);
    viewportLayoutPolicy->addItem(copyrightTextLabel, Qt::AlignCenter);

}
