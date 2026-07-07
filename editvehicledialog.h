#ifndef EDITVEHICLEDIALOG_H
#define EDITVEHICLEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QStackedWidget>
#include <QDialogButtonBox>

#include "vehicle.h"

// 编辑车辆对话框（结构与 AddVehicleDialog 类似，但预填数据）
class EditVehicleDialog : public QDialog {
    Q_OBJECT
public:
    // v: 要编辑的车辆（不会被修改，仅读取初始值）
    explicit EditVehicleDialog(const Vehicle *v, QWidget *parent = nullptr);

    Vehicle *createVehicle() const;

private slots:
    void onTypeChanged(int index);

private:
    void setupUI(const Vehicle *v);
    void populateFields(const Vehicle *v);

    QLineEdit      *m_idEdit;
    QLineEdit      *m_plateEdit;
    QLineEdit      *m_mfgEdit;
    QDateEdit      *m_purchaseEdit;
    QDoubleSpinBox *m_totalKmSpin;
    QDoubleSpinBox *m_fuelConsumptionSpin;
    QDoubleSpinBox *m_roadFeeSpin;
    QDoubleSpinBox *m_accumulatedCostSpin;
    QDoubleSpinBox *m_maintenanceSpin;

    QComboBox      *m_typeCombo;
    QStackedWidget *m_stacked;

    // Bus
    QSpinBox       *m_passengerSpin;
    // Car
    QComboBox      *m_trunkCombo;
    // Truck
    QDoubleSpinBox *m_loadSpin;

    QDialogButtonBox *m_buttonBox;
};

#endif // EDITVEHICLEDIALOG_H
