#include "truck.h"

Truck::Truck()
    : Vehicle(VehicleType::Truck, 1500.0) {}

double Truck::maxLoad() const { return m_maxLoad; }
void Truck::setMaxLoad(double load) { m_maxLoad = load; }

QString Truck::typeName() const { return QStringLiteral("卡车"); }

double Truck::monthlyTotalCost(double fuelPrice) const {
    return fuelPrice * m_fuelConsumption + m_basicMaintenance;
}

QJsonObject Truck::toJson() const {
    QJsonObject obj = Vehicle::toJson();
    obj["maxLoad"] = m_maxLoad;
    return obj;
}

void Truck::fromJson(const QJsonObject &obj) {
    Vehicle::fromJson(obj);
    m_maxLoad = obj["maxLoad"].toDouble();
}
