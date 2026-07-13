#include "editvehicledialog.h"
#include "bus.h"
#include "car.h"
#include "truck.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QPushButton>

EditVehicleDialog::EditVehicleDialog(const Vehicle *v, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("编辑车辆"));
    setMinimumWidth(480);
    setupUI(v);
}

void EditVehicleDialog::setupUI(const Vehicle *v) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ===== 通用信息 =====
    QGroupBox *commonGroup = new QGroupBox(QStringLiteral("通用信息"));
    QFormLayout *form = new QFormLayout(commonGroup);

    m_idEdit = new QLineEdit();
    form->addRow(QStringLiteral("车辆编号*："), m_idEdit);

    m_plateEdit = new QLineEdit();
    form->addRow(QStringLiteral("车牌号："), m_plateEdit);

    m_mfgEdit = new QLineEdit();
    form->addRow(QStringLiteral("制造公司："), m_mfgEdit);

    m_purchaseEdit = new QDateEdit();
    m_purchaseEdit->setCalendarPopup(true);
    m_purchaseEdit->setDisplayFormat("yyyy-MM-dd");
    form->addRow(QStringLiteral("购买时间："), m_purchaseEdit);

    m_typeCombo = new QComboBox();
    m_typeCombo->addItem(QStringLiteral("大客车"), static_cast<int>(VehicleType::Bus));
    m_typeCombo->addItem(QStringLiteral("小轿车"), static_cast<int>(VehicleType::Car));
    m_typeCombo->addItem(QStringLiteral("卡车"),   static_cast<int>(VehicleType::Truck));
    form->addRow(QStringLiteral("车辆类别："), m_typeCombo);

    m_totalKmSpin = new QDoubleSpinBox();
    m_totalKmSpin->setRange(0, 9999999);
    m_totalKmSpin->setDecimals(1);
    m_totalKmSpin->setSuffix(QStringLiteral(" km"));
    form->addRow(QStringLiteral("总公里数："), m_totalKmSpin);

    m_fuelConsumptionSpin = new QDoubleSpinBox();
    m_fuelConsumptionSpin->setRange(0, 99.9999);
    m_fuelConsumptionSpin->setDecimals(4);
    m_fuelConsumptionSpin->setSuffix(QStringLiteral(" L/km"));
    m_fuelConsumptionSpin->setSingleStep(0.01);
    form->addRow(QStringLiteral("耗油量/公里："), m_fuelConsumptionSpin);

    m_roadFeeSpin = new QDoubleSpinBox();
    m_roadFeeSpin->setRange(0, 999999);
    m_roadFeeSpin->setDecimals(2);
    m_roadFeeSpin->setSuffix(QStringLiteral(" 元"));
    form->addRow(QStringLiteral("养路费："), m_roadFeeSpin);

    m_accumulatedCostSpin = new QDoubleSpinBox();
    m_accumulatedCostSpin->setRange(0, 99999999);
    m_accumulatedCostSpin->setDecimals(2);
    m_accumulatedCostSpin->setSuffix(QStringLiteral(" 元"));
    form->addRow(QStringLiteral("累计总费用："), m_accumulatedCostSpin);
    m_maintenanceSpin = new QDoubleSpinBox();
    m_maintenanceSpin->setRange(0, 999999);
    m_maintenanceSpin->setDecimals(2);
    m_maintenanceSpin->setSuffix(QStringLiteral(" 元/月"));
    form->addRow(QStringLiteral("维护费(元/月)："), m_maintenanceSpin);
    mainLayout->addWidget(commonGroup);

    // ===== 类别专属 =====
    QGroupBox *specialGroup = new QGroupBox(QStringLiteral("类别专属信息"));
    QVBoxLayout *specialLayout = new QVBoxLayout(specialGroup);
    m_stacked = new QStackedWidget();

    // Bus page
    {
        QWidget *page = new QWidget();
        QFormLayout *pf = new QFormLayout(page);
        m_passengerSpin = new QSpinBox();
        m_passengerSpin->setRange(1, 300);
        m_passengerSpin->setSuffix(QStringLiteral(" 人"));
        pf->addRow(QStringLiteral("最大载客量："), m_passengerSpin);
        m_stacked->addWidget(page);
    }
    // Car page
    {
        QWidget *page = new QWidget();
        QFormLayout *pf = new QFormLayout(page);
        m_trunkCombo = new QComboBox();
        m_trunkCombo->addItem(QStringLiteral("三厢"), 3);
        m_trunkCombo->addItem(QStringLiteral("两厢"), 2);
        pf->addRow(QStringLiteral("厢数："), m_trunkCombo);
        m_stacked->addWidget(page);
    }
    // Truck page
    {
        QWidget *page = new QWidget();
        QFormLayout *pf = new QFormLayout(page);
        m_loadSpin = new QDoubleSpinBox();
        m_loadSpin->setRange(0.1, 500);
        m_loadSpin->setDecimals(1);
        m_loadSpin->setSuffix(QStringLiteral(" 吨"));
        pf->addRow(QStringLiteral("最大载重量："), m_loadSpin);
        m_stacked->addWidget(page);
    }

    specialLayout->addWidget(m_stacked);
    mainLayout->addWidget(specialGroup);

    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EditVehicleDialog::onTypeChanged);

    // 按钮
    m_buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_buttonBox->button(QDialogButtonBox::Ok)->setText(QStringLiteral("保存修改"));
    m_buttonBox->button(QDialogButtonBox::Cancel)->setText(QStringLiteral("取消修改"));
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        if (m_idEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("输入错误"),
                                 QStringLiteral("车辆编号不能为空！"));
            return;
        }

        QString plate = m_plateEdit->text().trimmed();
        if (plate.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("输入错误"),
                                 QStringLiteral("车牌号不能为空！"));
            return;
        }
        if (plate.length() < 7 || plate.length() > 8) {
            QMessageBox::warning(this, QStringLiteral("输入错误"),
                                 QStringLiteral("车牌号必须为7到8个字符！"));
            return;
        }

        accept();
    });
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(m_buttonBox);

    // 填充数据
    populateFields(v);
}

void EditVehicleDialog::populateFields(const Vehicle *v) {
    if (!v) return;

    m_idEdit->setText(v->id());
    m_plateEdit->setText(v->plateNumber());
    m_mfgEdit->setText(v->manufacturer());
    m_purchaseEdit->setDate(v->purchaseDate());
    m_totalKmSpin->setValue(v->totalKm());
    m_fuelConsumptionSpin->setValue(v->fuelConsumption());
    m_roadFeeSpin->setValue(v->roadFee());
    m_accumulatedCostSpin->setValue(v->accumulatedCost());
    m_maintenanceSpin->setValue(v->basicMaintenance());

    // 类型选择
    int idx = 0;
    switch (v->vehicleType()) {
    case VehicleType::Bus:   idx = 0; break;
    case VehicleType::Car:   idx = 1; break;
    case VehicleType::Truck: idx = 2; break;
    }
    m_typeCombo->setCurrentIndex(idx);

    // 专属字段
    if (auto *bus = dynamic_cast<const Bus *>(v)) {
        m_passengerSpin->setValue(bus->maxPassengers());
    } else if (auto *car = dynamic_cast<const Car *>(v)) {
        int trunkIdx = (car->trunkType() == 2) ? 1 : 0;
        m_trunkCombo->setCurrentIndex(trunkIdx);
    } else if (auto *truck = dynamic_cast<const Truck *>(v)) {
        m_loadSpin->setValue(truck->maxLoad());
    }
}

void EditVehicleDialog::onTypeChanged(int index) {
    m_stacked->setCurrentIndex(index);
    // 切换类别时自动更新维护费默认值
    switch (index) {
    case 0: m_maintenanceSpin->setValue(2000.0); break;  // 客车
    case 1: m_maintenanceSpin->setValue(1000.0); break;  // 轿车
    case 2: m_maintenanceSpin->setValue(1500.0); break;  // 卡车
    }
}

Vehicle *EditVehicleDialog::createVehicle() const {
    int typeIdx = m_typeCombo->currentIndex();
    Vehicle *v = nullptr;

    switch (typeIdx) {
    case 0: {
        auto *bus = new Bus();
        bus->setMaxPassengers(m_passengerSpin->value());
        v = bus;
        break;
    }
    case 1: {
        auto *car = new Car();
        car->setTrunkType(m_trunkCombo->currentData().toInt());
        v = car;
        break;
    }
    case 2: {
        auto *truck = new Truck();
        truck->setMaxLoad(m_loadSpin->value());
        v = truck;
        break;
    }
    default:
        return nullptr;
    }

    v->setId(m_idEdit->text().trimmed());
    v->setPlateNumber(m_plateEdit->text().trimmed());
    v->setManufacturer(m_mfgEdit->text().trimmed());
    v->setPurchaseDate(m_purchaseEdit->date());
    v->setTotalKm(m_totalKmSpin->value());
    v->setFuelConsumption(m_fuelConsumptionSpin->value());
    v->setRoadFee(m_roadFeeSpin->value());
    v->setAccumulatedCost(m_accumulatedCostSpin->value());
    v->setBasicMaintenance(m_maintenanceSpin->value());

    return v;
}
