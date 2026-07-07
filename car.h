#ifndef CAR_H
#define CAR_H

#include "vehicle.h"

// 小轿车：维护费 1000 元/月，附加箱数（两厢=2 / 三厢=3）
class Car : public Vehicle {
public:
    Car();

    int trunkType() const;          // 2 或 3
    void setTrunkType(int t);

    QString trunkTypeName() const;  // "两厢" / "三厢"

    QString typeName() const override;
    double monthlyTotalCost(double fuelPrice) const override;

    QJsonObject toJson() const override;
    void fromJson(const QJsonObject &obj) override;

private:
    int m_trunkType = 3; // 默认三厢
};

#endif // CAR_H
