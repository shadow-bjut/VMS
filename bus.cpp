#include "bus.h"

Bus::Bus()
    : Vehicle(VehicleType::Bus, 2000.0) {}

int Bus::maxPassengers() const { return m_maxPassengers; }
void Bus::setMaxPassengers(int n) { m_maxPassengers = n; }

QString Bus::typeName() const { return QStringLiteral("大客车"); }

double Bus::monthlyTotalCost(double fuelPrice) const {
    return fuelPrice * m_fuelConsumption + m_basicMaintenance;
}

QJsonObject Bus::toJson() const {
    QJsonObject obj = Vehicle::toJson();
    obj["maxPassengers"] = m_maxPassengers;
    return obj;
}

void Bus::fromJson(const QJsonObject &obj) {
    Vehicle::fromJson(obj);
    m_maxPassengers = obj["maxPassengers"].toInt();
}
