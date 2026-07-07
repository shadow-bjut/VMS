#ifndef BUS_H
#define BUS_H

#include "vehicle.h"

// 大客车：维护费 2000 元/月，附加载客量
class Bus : public Vehicle {
public:
    Bus();

    int maxPassengers() const;
    void setMaxPassengers(int n);

    QString typeName() const override;
    double monthlyTotalCost(double fuelPrice) const override;

    QJsonObject toJson() const override;
    void fromJson(const QJsonObject &obj) override;

private:
    int m_maxPassengers = 0;
};

#endif // BUS_H
