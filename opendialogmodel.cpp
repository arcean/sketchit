#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

#include "opendialogmodel.h"

#define THREAD_COUNT 5

OpenDialogModel::OpenDialogModel(QObject *parent, const QSize &size, const QStringList &dirs, const QString &currentFileName)
    : QAbstractTableModel(parent),
      m_dirs(dirs)
{
    qRegisterMetaType<MediaType>("MediaType");

    for (int i = 0; i < THREAD_COUNT; i++) {
        Loader *loader = new Loader(size);
        loader->setObjectName("loader");
        connect(loader, SIGNAL(imageReady(QImage, int)), this, SLOT(insertImage(QImage, int)));
        m_loaders.append(loader);
    }

    this->currentFileName = currentFileName;
    createItems();
}

OpenDialogModel::~OpenDialogModel()
{
    for (int i = 0; i < THREAD_COUNT; i++) {
        Loader *loader = m_loaders[i];
        loader->stop();
        loader->wait();
    }
    qDeleteAll(m_loaders);
    m_loaders.clear();
}

int OpenDialogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_items.size();
}

int OpenDialogModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}

QVariant OpenDialogModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() < m_items.size());


    MediaType m;

    if (m_items[ index.row() ].canConvert<MediaType>())
        m = m_items[ index.row() ].value<MediaType>();

    if (role == Qt::DisplayRole)
        return m_items[index.row()];
    else if (role == OpenDialogModel::Type)
        return m.type;
    else if (role == OpenDialogModel::ImageId)
        return m.path;

    return QVariant();
}

void OpenDialogModel::pauseLoaders()
{
    for (int i = 0; i < THREAD_COUNT; i++) {
        Loader *loader = m_loaders[i];
        if (loader->isRunning())
            loader->stop();
    }
}

void OpenDialogModel::resumeLoaders(int offset)
{
    if (offset < 0)
        offset = 0;
    else
        offset = offset / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        Loader *loader = m_loaders[i];
        if (!loader->isRunning() && loader->hasWork())
            loader->resume(offset);
    }
}

QUrl OpenDialogModel::parameterToUri(const QString &param) {
    QString tmp(param);
    if(tmp.startsWith("file://")) return QUrl(tmp);
    if(tmp.startsWith("/")) return QUrl(QString("file://") + tmp);
    return QUrl(QString("file://") + QDir::currentPath() + QDir::separator() + tmp);
}

void OpenDialogModel::createItems()
{
    int index = 0;

    foreach (const QString &dir, m_dirs) {
        QDir mediaDir(dir);
        mediaDir.setFilter(QDir::Files | QDir::NoSymLinks);
        mediaDir.setSorting(QDir::Name);
        mediaDir.setNameFilters(QStringList() << "*.png");
        const QFileInfoList fileList = mediaDir.entryInfoList();

        foreach (const QFileInfo & file, fileList) {
            const QString path = file.absoluteFilePath();
            MediaType m;
            m.type = MediaType::Image;
            m.path = path;
            m.image = QImage();
            Loader *loader = m_loaders[index % THREAD_COUNT];

            if (currentFileName.compare(path) != 0) {
                QUrl uri = parameterToUri(path);
                QString hash = QCryptographicHash::hash((uri.toString().toLatin1()),QCryptographicHash::Md5).toHex().constData();
                QString file = "/home/user/.thumbnails/grid/" + hash + ".jpeg";
                if (QFile(file).exists()) {
                    loader->pushImage(file,index);
                }
                else {
                    loader->pushImage(path,index);
                }
            }
            else {
                loader->pushImage(path,index);
            }
            m_items[index] = QVariant::fromValue(m);
            index++;
        }
    }
    resumeLoaders();
}

void OpenDialogModel::insertImage(QImage image, int index)
{
    if (m_items[index].canConvert<MediaType>()) {
        MediaType m = m_items[index].value<MediaType>();
        m.image = image;
        m_items[index] = QVariant::fromValue(m);

        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

Loader::~Loader()
{
    qDeleteAll(backlog);
    backlog.clear();
}

void Loader::run()
{
    while (true) {
        mutex.lock();
        if (stopWork) {
            mutex.unlock();
            return;
        }

        int offset = loadOffset;

        BacklogItem *backlogItem = NULL;
        if (offset >= backlog.count())
            offset = backlog.count() - 1;

        backlogItem = backlog.at(offset);

        // Try to find next item in backlog to load
        int nextOffset = offset;
        while (backlogItem->loaded && nextOffset < backlog.count())
            backlogItem = backlog.at(nextOffset++);

        // If we did not find, try to load previous in backlog
        int prevOffset = offset;
        while (backlogItem->loaded && prevOffset > 0)
            backlogItem = backlog.at(prevOffset--);

        // Everything is loaded, stop.
        if (backlogItem->loaded) {
            mutex.unlock();
            stop();
            return;
        }

        backlogItem->loaded = true;
        const QFileInfo file(backlogItem->path);
        int index = backlogItem->index;
        mutex.unlock();

        QImage image(file.absoluteFilePath());

        if (!image.isNull()) {
            scaleImage(image);
            emit imageReady(image, index);
            // Sleep for 8ms to let other threads do their job.
            msleep(8);
        }
    }
}

void Loader::scaleImage(QImage& image) const
{
    QSize new_size;
    new_size.setWidth(100);
    new_size.setHeight(100);
    image = image.scaled(new_size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    if (image.size() != new_size) {
        QPoint start((image.width() - new_size.width()) / 2, (image.height() - new_size.height()) / 2);
        image = image.copy(QRect(start, new_size));
    }
}

bool Loader::hasWork()
{
    mutex.lock();
    bool result = false;
    int i = 0;
    while (!result && i < backlog.count())
        result = (!backlog.at(i++)->loaded);
    mutex.unlock();
    return result;
}

void Loader::pushImage(const QString &path, int index)
{
    BacklogItem *backlogItem = new BacklogItem;
    backlogItem->path = path;
    backlogItem->index = index;
    backlogItem->loaded = false;

    mutex.lock();
    backlog.append(backlogItem);
    mutex.unlock();
}

void Loader::resume(int offset)
{
    loadOffset = offset;
    stopWork = false;
    start(QThread::LowestPriority);
}

void Loader::stop()
{
    mutex.lock();
    stopWork = true;
    mutex.unlock();
}
