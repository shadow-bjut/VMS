#include "vehicle.h"

// ---- VehicleType 与字符串互转 ----

QString vehicleTypeToString(VehicleType type) {
    switch (type) {
    case VehicleType::Bus:   return QStringLiteral("大客车");
    case VehicleType::Car:   return QStringLiteral("小轿车");
    case VehicleType::Truck: return QStringLiteral("卡车");
    }
    return QStringLiteral("未知");
}

VehicleType stringToVehicleType(const QString &str) {
    if (str == "大客车" || str == "Bus")   return VehicleType::Bus;
    if (str == "小轿车" || str == "Car")   return VehicleType::Car;
    if (str == "卡车"   || str == "Truck") return VehicleType::Truck;
    return VehicleType::Car; // fallback
}

// ---- Vehicle ----

Vehicle::Vehicle()
    : m_vehicleType(VehicleType::Car), m_purchaseDate(QDate::currentDate()) {}

Vehicle::Vehicle(VehicleType type, double defaultMaintenance)
    : m_vehicleType(type)
    , m_basicMaintenance(defaultMaintenance)
    , m_purchaseDate(QDate::currentDate()) {}

Vehicle::~Vehicle() = default;

QString Vehicle::id() const { return m_id; }
void Vehicle::setId(const QString &id) { m_id = id; }

QString Vehicle::plateNumber() const { return m_plateNumber; }
void Vehicle::setPlateNumber(const QString &plate) { m_plateNumber = plate; }

QString Vehicle::manufacturer() const { return m_manufacturer; }
void Vehicle::setManufacturer(const QString &mfg) { m_manufacturer = mfg; }

QDate Vehicle::purchaseDate() const { return m_purchaseDate; }
void Vehicle::setPurchaseDate(const QDate &date) { m_purchaseDate = date; }

VehicleType Vehicle::vehicleType() const { return m_vehicleType; }

double Vehicle::totalKm() const { return m_totalKm; }
void Vehicle::setTotalKm(double km) { m_totalKm = km; }

double Vehicle::fuelConsumption() const { return m_fuelConsumption; }
void Vehicle::setFuelConsumption(double fc) { m_fuelConsumption = fc; }

double Vehicle::basicMaintenance() const { return m_basicMaintenance; }
void Vehicle::setBasicMaintenance(double fee) { m_basicMaintenance = fee; }

double Vehicle::roadFee() const { return m_roadFee; }
void Vehicle::setRoadFee(double fee) { m_roadFee = fee; }

double Vehicle::accumulatedCost() const { return m_accumulatedCost; }
void Vehicle::setAccumulatedCost(double cost) { m_accumulatedCost = cost; }

QJsonObject Vehicle::toJson() const {
    QJsonObject obj;
    obj["id"]                = m_id;
    obj["plateNumber"]       = m_plateNumber;
    obj["manufacturer"]      = m_manufacturer;
    obj["purchaseDate"]      = m_purchaseDate.toString(Qt::ISODate);
    obj["vehicleType"]       = vehicleTypeToString(m_vehicleType);
    obj["totalKm"]           = m_totalKm;
    obj["fuelConsumption"]   = m_fuelConsumption;
    obj["basicMaintenance"]  = m_basicMaintenance;
    obj["roadFee"]           = m_roadFee;
    obj["accumulatedCost"]   = m_accumulatedCost;
    return obj;
}

void Vehicle::fromJson(const QJsonObject &obj) {
    m_id               = obj["id"].toString();
    m_plateNumber      = obj["plateNumber"].toString();
    m_manufacturer     = obj["manufacturer"].toString();
    m_purchaseDate     = QDate::fromString(obj["purchaseDate"].toString(), Qt::ISODate);
    m_vehicleType      = stringToVehicleType(obj["vehicleType"].toString());
    m_totalKm          = obj["totalKm"].toDouble();
    m_fuelConsumption  = obj["fuelConsumption"].toDouble();
    m_basicMaintenance = obj["basicMaintenance"].toDouble();
    m_roadFee          = obj["roadFee"].toDouble();
    m_accumulatedCost  = obj["accumulatedCost"].toDouble();
}
