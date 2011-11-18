/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "opendialogimagewidget.h"

#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>

OpenDialogImageWidget::OpenDialogImageWidget(QGraphicsItem *parent)
    : MImageWidget(parent),
    m_id(),
    opacityAnimation(0)
{
    opacityAnimation = new QPropertyAnimation(this, "opacity", this);
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);
    opacityAnimation->setDuration(300);
    this->setMinimumSize(100, 100);
    this->setMaximumSize(100, 100);
}

void OpenDialogImageWidget::setId(const QString& id)
{
    //this->setGeometry(0, 0, 64, 64);
    showAnimated();
    m_id = id;
}

QString OpenDialogImageWidget::id()
{
    return m_id;
}

void OpenDialogImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mousePressEvent(event);
    event->accept();
}

void OpenDialogImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mouseReleaseEvent(event);
    emit clicked();
}

void OpenDialogImageWidget::showAnimated()
{
    opacityAnimation->start();
}
