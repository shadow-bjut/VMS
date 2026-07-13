#include "addvehicledialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QPushButton>

AddVehicleDialog::AddVehicleDialog(const QStringList &existingIds, QWidget *parent)
    : QDialog(parent)
    , m_existingIds(existingIds.begin(), existingIds.end())
{
    setWindowTitle(QStringLiteral("添加车辆"));
    setMinimumWidth(480);
    setupUI();

    // 自动生成默认编号
    m_lastAutoId = generateDefaultId(m_typeCombo->currentIndex());
    m_idEdit->setText(m_lastAutoId);
}

void AddVehicleDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ===== 通用信息 =====
    QGroupBox *commonGroup = new QGroupBox(QStringLiteral("通用信息"));
    QFormLayout *form = new QFormLayout(commonGroup);

    m_idEdit = new QLineEdit();
    m_idEdit->setPlaceholderText(QStringLiteral("必填，且不可重复"));
    form->addRow(QStringLiteral("车辆编号*："), m_idEdit);

    m_plateEdit = new QLineEdit();
    m_plateEdit->setPlaceholderText(QStringLiteral("如 京A12345"));
    form->addRow(QStringLiteral("车牌号："), m_plateEdit);

    m_mfgEdit = new QLineEdit();
    m_mfgEdit->setPlaceholderText(QStringLiteral("如 一汽大众"));
    form->addRow(QStringLiteral("制造公司："), m_mfgEdit);

    m_purchaseEdit = new QDateEdit(QDate::currentDate());
    m_purchaseEdit->setCalendarPopup(true);
    m_purchaseEdit->setDisplayFormat("yyyy-MM-dd");
    form->addRow(QStringLiteral("购买时间："), m_purchaseEdit);

    // 类别
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

    // 维护费（通用，默认值跟随类别自动切换）
    m_maintenanceSpin = new QDoubleSpinBox();
    m_maintenanceSpin->setRange(0, 999999);
    m_maintenanceSpin->setDecimals(2);
    m_maintenanceSpin->setSuffix(QStringLiteral(" 元/月"));
    m_maintenanceSpin->setValue(2000);  // 初始默认值（对应第一个类型：客车）
    form->addRow(QStringLiteral("维护费(元/月)："), m_maintenanceSpin);

    mainLayout->addWidget(commonGroup);

    // ===== 类别专属信息（堆叠面板） =====
    QGroupBox *specialGroup = new QGroupBox(QStringLiteral("类别专属信息"));
    QVBoxLayout *specialLayout = new QVBoxLayout(specialGroup);
    m_stacked = new QStackedWidget();

    // 第0页：Bus
    {
        QWidget *page = new QWidget();
        QFormLayout *pf = new QFormLayout(page);
        m_passengerSpin = new QSpinBox();
        m_passengerSpin->setRange(1, 300);
        m_passengerSpin->setValue(50);
        m_passengerSpin->setSuffix(QStringLiteral(" 人"));
        pf->addRow(QStringLiteral("最大载客量："), m_passengerSpin);
        m_stacked->addWidget(page);
    }
    // 第1页：Car
    {
        QWidget *page = new QWidget();
        QFormLayout *pf = new QFormLayout(page);
        m_trunkCombo = new QComboBox();
        m_trunkCombo->addItem(QStringLiteral("三厢"), 3);
        m_trunkCombo->addItem(QStringLiteral("两厢"), 2);
        pf->addRow(QStringLiteral("厢数："), m_trunkCombo);
        m_stacked->addWidget(page);
    }
    // 第2页：Truck
    {
        QWidget *page = new QWidget();
        QFormLayout *pf = new QFormLayout(page);
        m_loadSpin = new QDoubleSpinBox();
        m_loadSpin->setRange(0.1, 500);
        m_loadSpin->setValue(10);
        m_loadSpin->setDecimals(1);
        m_loadSpin->setSuffix(QStringLiteral(" 吨"));
        pf->addRow(QStringLiteral("最大载重量："), m_loadSpin);
        m_stacked->addWidget(page);
    }

    specialLayout->addWidget(m_stacked);
    mainLayout->addWidget(specialGroup);

    // 类型切换
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddVehicleDialog::onTypeChanged);
    onTypeChanged(m_typeCombo->currentIndex()); // 初始同步

    // 按钮
    m_buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_buttonBox->button(QDialogButtonBox::Ok)->setText(QStringLiteral("添加"));
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        // 输入校验
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
                                 QStringLiteral("车牌号必须合法（例如京AXXXXX或京AXXXXXX）！"));
            return;
        }

        accept();
    });
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(m_buttonBox);
}

void AddVehicleDialog::onTypeChanged(int index) {
    m_stacked->setCurrentIndex(index);
    // 切换类别时自动更新维护费默认值
    switch (index) {
    case 0: m_maintenanceSpin->setValue(2000.0); break;  // 客车
    case 1: m_maintenanceSpin->setValue(1000.0); break;  // 轿车
    case 2: m_maintenanceSpin->setValue(1500.0); break;  // 卡车
    }

    // 如果用户未手动修改编号，则自动更新为新类型的默认编号
    if (m_idEdit->text().trimmed() == m_lastAutoId
        || m_idEdit->text().trimmed().isEmpty()) {
        m_lastAutoId = generateDefaultId(index);
        m_idEdit->setText(m_lastAutoId);
    }
}

Vehicle *AddVehicleDialog::createVehicle() const {
    int typeIdx = m_typeCombo->currentIndex();
    Vehicle *v = nullptr;

    switch (typeIdx) {
    case 0: { // Bus
        auto *bus = new Bus();
        bus->setMaxPassengers(m_passengerSpin->value());
        v = bus;
        break;
    }
    case 1: { // Car
        auto *car = new Car();
        car->setTrunkType(m_trunkCombo->currentData().toInt());
        v = car;
        break;
    }
    case 2: { // Truck
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

QString AddVehicleDialog::generateDefaultId(int typeIndex) const {
    // 根据类型确定前缀
    QString prefix;
    switch (typeIndex) {
    case 0: prefix = QStringLiteral("BUS");   break;
    case 1: prefix = QStringLiteral("CAR");   break;
    case 2: prefix = QStringLiteral("TRUCK"); break;
    default: return QStringLiteral("XXX-001");
    }

    // 找出该前缀下已有编号的最大数字
    int maxNum = 0;
    for (const QString &id : m_existingIds) {
        if (id.startsWith(prefix + QStringLiteral("-"))) {
            QString numStr = id.mid(prefix.length() + 1);
            bool ok = false;
            int num = numStr.toInt(&ok);
            if (ok && num > maxNum)
                maxNum = num;
        }
    }

    // 生成下一个编号（3位补零）
    return QStringLiteral("%1-%2").arg(prefix).arg(maxNum + 1, 3, 10, QChar('0'));
}
