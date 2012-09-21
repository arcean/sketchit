#include <QStringList>

#include "linewidthmodel.h"

int LineWidthModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent);
   return 4;
}

QVariant LineWidthModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
      QStringList rowData;
      if(index.row() == 0) {
        rowData << "Small";
      }
      else if (index.row() == 1) {
          rowData << "Medium";
      }
      else if (index.row() == 2) {
          rowData << "Large";
      }
      else if (index.row() == 3) {
          rowData << "Very large";
      }
      return QVariant(rowData);
  }

  return QVariant();
}
