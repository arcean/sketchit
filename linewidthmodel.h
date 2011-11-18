#ifndef LINEWIDTHMODEL_H
#define LINEWIDTHMODEL_H

#include <QAbstractListModel>

class LineWidthModel : public QAbstractListModel
 {
    Q_OBJECT

  public:
    LineWidthModel(QObject *parent = 0)
       : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

 };

#endif // LINEWIDTHMODEL_H
