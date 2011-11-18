#include <QStringList>

#include "toolpickermodel.h"

int ToolPickerModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent);
   return 6;
}

QVariant ToolPickerModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
      QStringList rowData;
      if(index.row() == 0) {
        rowData << "Finger"; // first name
      }
      else if (index.row() == 1){
          rowData << "Pencil"; // first name
      }
      else if (index.row() == 2){
          rowData << "Rectangle"; // first name
      }
      else if (index.row() == 3){
          rowData << "Elipse"; // first name
      }
      else if (index.row() == 4){
          rowData << "Line"; // first name
      }
      else {
          rowData << "Rubber";
      }
      return QVariant(rowData);
  }

  return QVariant();
}
