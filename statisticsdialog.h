#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>
#include <QLabel>
#include "vehiclemanager.h"

// 统计信息对话框
class StatisticsDialog : public QDialog {
    Q_OBJECT
public:
    explicit StatisticsDialog(const VehicleManager &manager, QWidget *parent = nullptr);

private:
    void setupUI(const Statistics &s);
};

#endif // STATISTICSDIALOG_H
