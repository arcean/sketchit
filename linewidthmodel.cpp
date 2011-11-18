#include <QStringList>

#include "linewidthmodel.h"

int LineWidthModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent);
   return 3;
}

QVariant LineWidthModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
      QStringList rowData;
      if(index.row() == 0) {
        rowData << "Small"; // first name
        //rowData << "Joli"; // last name
      }
      else if (index.row() == 1){
          rowData << "Medium"; // first name
         // rowData << "Joli2"; // last name
      }
      else {
          rowData << "Large";
      }
      return QVariant(rowData);
  }

  return QVariant();
}
