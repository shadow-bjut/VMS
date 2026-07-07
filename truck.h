#ifndef TRUCK_H
#define TRUCK_H

#include "vehicle.h"

// 卡车：维护费 1500 元/月，附加载重量（吨）
class Truck : public Vehicle {
public:
    Truck();

    double maxLoad() const;          // 最大载重量，吨
    void setMaxLoad(double load);

    QString typeName() const override;
    double monthlyTotalCost(double fuelPrice) const override;

    QJsonObject toJson() const override;
    void fromJson(const QJsonObject &obj) override;

private:
    double m_maxLoad = 0;
};

#endif // TRUCK_H
