#ifndef TOOLPICKERMODEL_H
#define TOOLPICKERMODEL_H

#include <QAbstractListModel>

class ToolPickerModel : public QAbstractListModel
 {
    Q_OBJECT

  public:
    ToolPickerModel(QObject *parent = 0)
       : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

 };

#endif // TOOLPICKERMODEL_H
