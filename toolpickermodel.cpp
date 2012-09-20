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
        rowData << "Finger";
      }
      else if (index.row() == 1){
          rowData << "Pencil";
      }
      else if (index.row() == 2){
          rowData << "Line";
      }
      else if (index.row() == 3){
          rowData << "Rectangle";
      }
      else if (index.row() == 4){
          rowData << "Elipse";
      }
      else {
          // Make rubber tool font color accented
          rowData << "<font color=\"#9B9B9B\">Rubber</font>";
      }
      return QVariant(rowData);
  }

  return QVariant();
}
