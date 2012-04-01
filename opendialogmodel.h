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

#ifndef GRIDMODEL_H
#define GRIDMODEL_H

#include <QList>
#include <QSize>
#include <QPair>
#include <QDir>
#include <QPixmap>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QAbstractTableModel>
#include <QUrl>
#include <QCryptographicHash>

class Loader;

struct MediaType
{
    enum Type {
        Image = 0
    } type;

    QString path;
    QImage image;
};

Q_DECLARE_METATYPE(MediaType);

class OpenDialogModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Role {
        ImageId = Qt::UserRole + 1,
        Type
    };


    explicit OpenDialogModel(QObject *parent, const QSize &size, const QStringList &dirs, const QString &currentFileName);
    ~OpenDialogModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void pauseLoaders();
    void resumeLoaders(int offset = 0);

private slots:
    void insertImage(QImage pixmap, int index);

private:
    void createItems();
    QUrl parameterToUri(const QString &param);
    QMap<int, QVariant> m_items;

    QList<Loader*> m_loaders;
    QStringList m_dirs;
    QString currentFileName;
};

struct BacklogItem {
    QString path;
    int index;
    bool loaded;
};

class Loader : public QThread
{
    Q_OBJECT
public:
    explicit Loader (const QSize &s)
        : QThread(), mutex(), backlog(), size(s), stopWork(false), loadOffset(0) { }
    virtual ~Loader();
    void pushImage(const QString &path, int index);

    void resume(int offset);
    void stop();

    void scaleImage(QImage& image) const;

    bool hasWork();

signals:
    void imageReady(QImage pixmap, int index);

protected:
    void run();

private:
    QMutex mutex;
    QList<BacklogItem*> backlog;
    QSize size;
    bool stopWork;
    int loadOffset;
};

#endif
