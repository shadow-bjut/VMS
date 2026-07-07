#ifndef VEHICLEMANAGER_H
#define VEHICLEMANAGER_H

#include <QVector>
#include <QString>
#include "vehicle.h"
#include "bus.h"
#include "car.h"
#include "truck.h"

// 查询结果结构体
struct Statistics {
    int total = 0;
    int busCount = 0;
    int carCount = 0;
    int truckCount = 0;
};

class VehicleManager {
public:
    VehicleManager();
    ~VehicleManager();

    // --- 增删改查 ---

    // 添加车辆，成功返回 true；编号重复或满则返回 false + errorMsg
    bool addVehicle(Vehicle *vehicle, QString &errorMsg);

    // 按编号删除，成功返回 true
    bool removeVehicle(const QString &id);

    // 按编号查找，找不到返回 nullptr（不转移所有权）
    Vehicle *findById(const QString &id) const;

    // 按制造公司查找（模糊匹配）
    QVector<Vehicle *> findByManufacturer(const QString &keyword) const;

    // 按类别查找
    QVector<Vehicle *> findByType(VehicleType type) const;

    // 更新车辆信息（保持同一对象，但校验新编号不与其他车重复）
    bool updateVehicle(const QString &oldId, Vehicle *newData, QString &errorMsg);

    // 全部车辆（只读）
    const QVector<Vehicle *> &allVehicles() const;

    // --- 统计 ---
    Statistics getStatistics() const;

    // --- 容量 ---
    int count() const;
    bool isEmpty() const;
    int maxCapacity() const;
    void setMaxCapacity(int cap);
    bool isFull() const;

    // --- 油价 ---
    double fuelPrice() const;
    void setFuelPrice(double price);

    // --- 文件 I/O ---
    bool saveToFile(const QString &filePath, QString &errorMsg) const;
    bool loadFromFile(const QString &filePath, QString &errorMsg);

    // 清空
    void clearAll();

private:
    QVector<Vehicle *> m_vehicles;
    int m_maxCapacity = 100;
    double m_fuelPrice = 7.5;  // 油价，默认 7.5 元/升
};

#endif // VEHICLEMANAGER_H
