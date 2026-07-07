#include "vehiclemanager.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

// ---- VehicleManager ----

VehicleManager::VehicleManager() = default;

VehicleManager::~VehicleManager() {
    clearAll();
}

// ---- 增 ----

bool VehicleManager::addVehicle(Vehicle *vehicle, QString &errorMsg) {
    if (!vehicle) {
        errorMsg = QStringLiteral("车辆对象为空！");
        return false;
    }
    if (isFull()) {
        errorMsg = QStringLiteral("车辆信息库已满，无法添加！");
        return false;
    }
    // 检查编号唯一
    if (findById(vehicle->id())) {
        errorMsg = QStringLiteral("编号 ") + vehicle->id() + QStringLiteral(" 已存在，添加失败！");
        return false;
    }
    m_vehicles.append(vehicle);
    return true;
}

// ---- 删 ----

bool VehicleManager::removeVehicle(const QString &id) {
    for (int i = 0; i < m_vehicles.size(); ++i) {
        if (m_vehicles[i]->id() == id) {
            delete m_vehicles[i];
            m_vehicles.removeAt(i);
            return true;
        }
    }
    return false;
}

// ---- 查 ----

Vehicle *VehicleManager::findById(const QString &id) const {
    for (auto *v : m_vehicles) {
        if (v->id() == id)
            return v;
    }
    return nullptr;
}

QVector<Vehicle *> VehicleManager::findByManufacturer(const QString &keyword) const {
    QVector<Vehicle *> result;
    for (auto *v : m_vehicles) {
        if (v->manufacturer().contains(keyword, Qt::CaseInsensitive))
            result.append(v);
    }
    return result;
}

QVector<Vehicle *> VehicleManager::findByType(VehicleType type) const {
    QVector<Vehicle *> result;
    for (auto *v : m_vehicles) {
        if (v->vehicleType() == type)
            result.append(v);
    }
    return result;
}

// ---- 改 ----

bool VehicleManager::updateVehicle(const QString &oldId, Vehicle *newData, QString &errorMsg) {
    if (!newData) {
        errorMsg = QStringLiteral("更新数据为空！");
        return false;
    }
    Vehicle *target = findById(oldId);
    if (!target) {
        errorMsg = QStringLiteral("编号 ") + oldId + QStringLiteral(" 不存在！");
        return false;
    }
    // 如果编号变了，检查新编号是否与其他车重复
    if (oldId != newData->id() && findById(newData->id())) {
        errorMsg = QStringLiteral("新编号 ") + newData->id() + QStringLiteral(" 已被使用！");
        return false;
    }
    // 用 newData 的属性覆盖 target（保持 target 指针不变）
    // 这里用 JSON 中转做一个"深拷贝"
    QJsonObject json = newData->toJson();
    target->fromJson(json);
    return true;
}

// ---- 全部 ----

const QVector<Vehicle *> &VehicleManager::allVehicles() const {
    return m_vehicles;
}

// ---- 统计 ----

Statistics VehicleManager::getStatistics() const {
    Statistics s;
    s.total = m_vehicles.size();
    for (auto *v : m_vehicles) {
        switch (v->vehicleType()) {
        case VehicleType::Bus:   ++s.busCount;   break;
        case VehicleType::Car:   ++s.carCount;   break;
        case VehicleType::Truck: ++s.truckCount; break;
        }
    }
    return s;
}

// ---- 容量 ----

int VehicleManager::count() const { return m_vehicles.size(); }
bool VehicleManager::isEmpty() const { return m_vehicles.isEmpty(); }
int VehicleManager::maxCapacity() const { return m_maxCapacity; }
void VehicleManager::setMaxCapacity(int cap) { m_maxCapacity = cap; }
bool VehicleManager::isFull() const { return m_vehicles.size() >= m_maxCapacity; }

// ---- 油价 ----

double VehicleManager::fuelPrice() const { return m_fuelPrice; }
void VehicleManager::setFuelPrice(double price) { m_fuelPrice = price; }

// ---- 文件 I/O ----

bool VehicleManager::saveToFile(const QString &filePath, QString &errorMsg) const {
    QJsonArray arr;
    for (auto *v : m_vehicles) {
        QJsonObject obj = v->toJson();
        // 确保类型字段正确
        obj["vehicleType"] = vehicleTypeToString(v->vehicleType());
        arr.append(obj);
    }
    QJsonDocument doc(arr);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        errorMsg = QStringLiteral("无法打开文件 ") + filePath + QStringLiteral(" 进行写入！");
        return false;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool VehicleManager::loadFromFile(const QString &filePath, QString &errorMsg) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorMsg = QStringLiteral("无法打开文件 ") + filePath + QStringLiteral(" 进行读取！");
        return false;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        errorMsg = QStringLiteral("JSON 解析失败: ") + parseError.errorString();
        return false;
    }
    if (!doc.isArray()) {
        errorMsg = QStringLiteral("文件格式错误：顶层不是数组！");
        return false;
    }

    clearAll(); // 清除现有数据
    const QJsonArray arr = doc.array();
    for (const QJsonValue &val : arr) {
        if (!val.isObject()) continue;
        QJsonObject obj = val.toObject();
        QString typeStr = obj["vehicleType"].toString();
        VehicleType vt = stringToVehicleType(typeStr);
        Vehicle *v = nullptr;
        switch (vt) {
        case VehicleType::Bus:   v = new Bus();   break;
        case VehicleType::Car:   v = new Car();   break;
        case VehicleType::Truck: v = new Truck(); break;
        }
        if (v) {
            v->fromJson(obj);
            m_vehicles.append(v);
        }
    }
    return true;
}

// ---- 清空 ----

void VehicleManager::clearAll() {
    for (auto *v : m_vehicles) {
        delete v;
    }
    m_vehicles.clear();
}
