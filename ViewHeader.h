#ifndef VIEWHEADER_H
#define VIEWHEADER_H

#include <MWidgetController>
#include <QGraphicsLinearLayout>
#include <MLabel>

class ViewHeader : public MWidgetController
{
public:
    ViewHeader(QGraphicsItem *parent = 0) :
        MWidgetController(parent),
        linearLayout(0),
        titleWidget(0)
    {
        setObjectName("CommonHeaderPanel");
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setContentsMargins(0, 0, 0, 0);
        linearLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
        linearLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        linearLayout->setContentsMargins(0, 0, 0, 0);
        linearLayout->setSpacing(0);
        titleWidget = new MLabel(this);
        titleWidget->setWordWrap(true);
        titleWidget->setStyleName("CommonHeaderInverted");
        titleWidget->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        linearLayout->addItem(titleWidget);
        linearLayout->setAlignment(titleWidget, Qt::AlignLeft | Qt::AlignVCenter);
    }

    void setTitle(const QString &title)
    {
        titleWidget->setText(title);
    }

private:
    QGraphicsLinearLayout *linearLayout;
    MLabel *titleWidget;

};

#endif
