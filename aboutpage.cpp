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

#include "aboutpage.h"

#define COPYRIGHT_TEXT_LINES 15

AboutPage::AboutPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{

}

void AboutPage::createContent()
{
    this->setStyleName("CommonApplicationPageInverted");
    MLayout *layout = new MLayout;
    centralWidget()->setLayout(layout);
    MLinearLayoutPolicy *landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);

    MImageWidget *image = new MImageWidget("sketchit");
    MLabel *app_name = new MLabel("SketchIt! 0.9.8");
    app_name->setStyleName("CommonTitleInverted");
    app_name->setAlignment(Qt::AlignCenter);
    QFont label_font;
   // label_font.setBold(true);
    label_font.setPointSize(32);
    app_name->setFont(label_font);

    const QChar CopyrightSymbol(0x00a9);

    QString *copyright_string = new QString("Copyright " + QString(CopyrightSymbol) + QString::fromUtf8(" 2011 Tomasz Pieniążek"));

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


   /* MTextEdit *license = new MTextEdit(MTextEditModel::MultiLine, license_string);
    license->setStyleName("CommonSingleInputFieldInverted");
    license->setReadOnly(true);
    license->setAutoSelectionEnabled(false);
    license->setFont(license_font);*/


    landscapePolicy->addItem(titleSpacer, Qt::AlignCenter);
    landscapePolicy->addItem(image, Qt::AlignCenter);
    landscapePolicy->addItem(app_name, Qt::AlignCenter);
    landscapePolicy->addItem(copyright_title, Qt::AlignCenter);
    //landscapePolicy->addItem(license, Qt::AlignCenter);
    landscapePolicy->addItem(copyrightTextLabel, Qt::AlignCenter);

    portraitPolicy->addItem(titleSpacer, Qt::AlignCenter);
    portraitPolicy->addItem(image, Qt::AlignCenter);
    portraitPolicy->addItem(app_name, Qt::AlignCenter);
    portraitPolicy->addItem(copyright_title, Qt::AlignCenter);
    //portraitPolicy->addItem(license, Qt::AlignCenter);
    portraitPolicy->addItem(copyrightTextLabel, Qt::AlignCenter);

}
