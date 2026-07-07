#ifndef ADDVEHICLEDIALOG_H
#define ADDVEHICLEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QStackedWidget>
#include <QDialogButtonBox>
#include <QFormLayout>

#include "vehicle.h"
#include "bus.h"
#include "car.h"
#include "truck.h"

// 添加车辆对话框
class AddVehicleDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddVehicleDialog(QWidget *parent = nullptr);

    // 创建车辆对象（调用方拥有所有权）
    Vehicle *createVehicle() const;

private slots:
    void onTypeChanged(int index);

private:
    void setupUI();

    // 通用字段
    QLineEdit      *m_idEdit;
    QLineEdit      *m_plateEdit;
    QLineEdit      *m_mfgEdit;
    QDateEdit      *m_purchaseEdit;
    QDoubleSpinBox *m_totalKmSpin;
    QDoubleSpinBox *m_fuelConsumptionSpin;
    QDoubleSpinBox *m_roadFeeSpin;
    QDoubleSpinBox *m_accumulatedCostSpin;
    QDoubleSpinBox *m_maintenanceSpin;


    // 类别选择
    QComboBox      *m_typeCombo;

    // 堆叠面板：根据类别切换
    QStackedWidget *m_stacked;

    // Bus 专属
    QSpinBox       *m_passengerSpin;   // 载客量

    // Car 专属
    QComboBox      *m_trunkCombo;      // 两厢/三厢

    // Truck 专属
    QDoubleSpinBox *m_loadSpin;        // 载重量

    QDialogButtonBox *m_buttonBox;
};

#endif // ADDVEHICLEDIALOG_H
