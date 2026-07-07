#ifndef VEHICLE_H
#define VEHICLE_H

#include <QString>
#include <QDate>
#include <QJsonObject>

// 车辆类别枚举
enum class VehicleType {
    Bus,   // 大客车
    Car,   // 小轿车
    Truck  // 卡车
};

// 将 VehicleType 与字符串互转
QString vehicleTypeToString(VehicleType type);
VehicleType stringToVehicleType(const QString &str);

// 抽象基类：车辆
class Vehicle {
public:
    Vehicle();
    virtual ~Vehicle();

    // --- 公共属性 getter/setter ---
    QString id() const;
    void setId(const QString &id);

    QString plateNumber() const;
    void setPlateNumber(const QString &plate);

    QString manufacturer() const;
    void setManufacturer(const QString &mfg);

    QDate purchaseDate() const;
    void setPurchaseDate(const QDate &date);

    VehicleType vehicleType() const;

    double totalKm() const;
    void setTotalKm(double km);

    double fuelConsumption() const;       // 耗油量/公里 (L/km)
    void setFuelConsumption(double fc);

    double basicMaintenance() const;       // 基本维护费用 (元/月)
    void setBasicMaintenance(double fee);

    double roadFee() const;
    void setRoadFee(double fee);

    double accumulatedCost() const;
    void setAccumulatedCost(double cost);

    // 当月总费用 = 油价 * 耗油量/公里 + 基本维护费用
    // 油价作为参数传入，由 VehicleManager 统一管理
    virtual double monthlyTotalCost(double fuelPrice) const = 0;

    // 获取车辆类型名称
    virtual QString typeName() const = 0;

    // JSON 序列化 / 反序列化
    virtual QJsonObject toJson() const;
    virtual void fromJson(const QJsonObject &obj);

protected:
    // 子类通过此构造函数设定类型与默认维护费
    Vehicle(VehicleType type, double defaultMaintenance);

    QString m_id;
    QString m_plateNumber;
    QString m_manufacturer;
    QDate   m_purchaseDate;
    VehicleType m_vehicleType;
    double  m_totalKm = 0;
    double  m_fuelConsumption = 0;    // 耗油量/公里
    double  m_basicMaintenance = 0;
    double  m_roadFee = 0;
    double  m_accumulatedCost = 0;
};

#endif // VEHICLE_H
