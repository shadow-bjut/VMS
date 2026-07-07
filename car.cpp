#include "car.h"

Car::Car()
    : Vehicle(VehicleType::Car, 1000.0) {}

int Car::trunkType() const { return m_trunkType; }
void Car::setTrunkType(int t) { m_trunkType = (t == 2) ? 2 : 3; }

QString Car::trunkTypeName() const {
    return (m_trunkType == 2) ? QStringLiteral("两厢") : QStringLiteral("三厢");
}

QString Car::typeName() const { return QStringLiteral("小轿车"); }

double Car::monthlyTotalCost(double fuelPrice) const {
    return fuelPrice * m_fuelConsumption + m_basicMaintenance;
}

QJsonObject Car::toJson() const {
    QJsonObject obj = Vehicle::toJson();
    obj["trunkType"] = m_trunkType;
    return obj;
}

void Car::fromJson(const QJsonObject &obj) {
    Vehicle::fromJson(obj);
    m_trunkType = obj["trunkType"].toInt(3);
}
