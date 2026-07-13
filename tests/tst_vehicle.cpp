// ============================================================
// 白盒测试: Vehicle 数据模型层
//
// 覆盖目标:
//   - 语句覆盖: 每个 getter/setter/toJson/fromJson 至少执行一次
//   - 分支覆盖: VehicleType 枚举转换的所有 case
//   - 边界覆盖: trunkType 非法值自动修正
//   - 路径覆盖: toJson → fromJson 往返路径
// ============================================================

#include <QtTest>
#include "../vehicle.h"
#include "../car.h"
#include "../bus.h"
#include "../truck.h"

class TestVehicle : public QObject {
    Q_OBJECT

private slots:
    // ==================== VehicleType 枚举转换 ====================

    void test_enumToString() {
        // 分支覆盖: 每个 case + default
        QCOMPARE(vehicleTypeToString(VehicleType::Bus),   QString("大客车"));
        QCOMPARE(vehicleTypeToString(VehicleType::Car),   QString("小轿车"));
        QCOMPARE(vehicleTypeToString(VehicleType::Truck), QString("卡车"));
    }

    void test_stringToEnum_chinese() {
        // 路径: 中文输入
        QCOMPARE(stringToVehicleType("大客车"), VehicleType::Bus);
        QCOMPARE(stringToVehicleType("小轿车"), VehicleType::Car);
        QCOMPARE(stringToVehicleType("卡车"),   VehicleType::Truck);
    }

    void test_stringToEnum_english() {
        // 路径: 英文输入 (兼容性)
        QCOMPARE(stringToVehicleType("Bus"),   VehicleType::Bus);
        QCOMPARE(stringToVehicleType("Car"),   VehicleType::Car);
        QCOMPARE(stringToVehicleType("Truck"), VehicleType::Truck);
    }

    void test_stringToEnum_fallback() {
        // 边界: 非法输入 → fallback 为 Car
        QCOMPARE(stringToVehicleType("飞机"),   VehicleType::Car);
        QCOMPARE(stringToVehicleType(""),       VehicleType::Car);
        QCOMPARE(stringToVehicleType("invalid"), VehicleType::Car);
    }

    // ==================== Vehicle 基类 ====================

    void test_vehicle_getterSetter() {
        // 语句覆盖: 每个 getter/setter 至少一次
        Car v;
        v.setId("CAR-001");
        v.setPlateNumber("京A12345");
        v.setManufacturer("一汽大众");
        v.setTotalKm(50000.5);
        v.setFuelConsumption(0.08);
        v.setRoadFee(200.0);
        v.setAccumulatedCost(15000.0);
        v.setBasicMaintenance(1200.0);

        QCOMPARE(v.id(),                QString("CAR-001"));
        QCOMPARE(v.plateNumber(),       QString("京A12345"));
        QCOMPARE(v.manufacturer(),      QString("一汽大众"));
        QCOMPARE(v.totalKm(),           50000.5);
        QCOMPARE(v.fuelConsumption(),   0.08);
        QCOMPARE(v.roadFee(),           200.0);
        QCOMPARE(v.accumulatedCost(),   15000.0);
        QCOMPARE(v.basicMaintenance(),  1200.0);
    }

    void test_vehicle_purchaseDate_default() {
        // 默认构造: purchaseDate = 今天
        Car v;
        QCOMPARE(v.purchaseDate(), QDate::currentDate());
    }

    void test_vehicle_purchaseDate() {
        Car v;
        QDate d(2024, 3, 15);
        v.setPurchaseDate(d);
        QCOMPARE(v.purchaseDate(), d);
    }

    // ==================== Car 小轿车 ====================

    void test_car_typeName() {
        Car c;
        QCOMPARE(c.typeName(), QString("小轿车"));
    }

    void test_car_vehicleType() {
        Car c;
        QCOMPARE(c.vehicleType(), VehicleType::Car);
    }

    void test_car_trunkType_default() {
        Car c;
        QCOMPARE(c.trunkType(), 3); // 默认三厢
    }

    void test_car_trunkType_valid() {
        // 等价类: 有效值
        Car c;
        c.setTrunkType(2);
        QCOMPARE(c.trunkType(), 2);
        c.setTrunkType(3);
        QCOMPARE(c.trunkType(), 3);
    }

    void test_car_trunkType_invalid() {
        // 边界: 非法值 → 自动修正为 3
        Car c;
        c.setTrunkType(1);  // 不是 2 或 3
        QCOMPARE(c.trunkType(), 3);
        c.setTrunkType(99);
        QCOMPARE(c.trunkType(), 3);
    }

    void test_car_trunkTypeName() {
        Car c;
        c.setTrunkType(2);
        QCOMPARE(c.trunkTypeName(), QString("两厢"));
        c.setTrunkType(3);
        QCOMPARE(c.trunkTypeName(), QString("三厢"));
    }

    void test_car_monthlyTotalCost() {
        // 路径: fuelPrice × fuelConsumption + 1000
        Car c;
        c.setFuelConsumption(0.1);
        QCOMPARE(c.monthlyTotalCost(7.5), 7.5 * 0.1 + 1000.0);
        QCOMPARE(c.monthlyTotalCost(10.0), 10.0 * 0.1 + 1000.0);
        QCOMPARE(c.monthlyTotalCost(0.0), 0.0 + 1000.0);
    }

    // ==================== Bus 大客车 ====================

    void test_bus_typeName() {
        Bus b;
        QCOMPARE(b.typeName(), QString("大客车"));
    }

    void test_bus_vehicleType() {
        Bus b;
        QCOMPARE(b.vehicleType(), VehicleType::Bus);
    }

    void test_bus_maxPassengers() {
        Bus b;
        QCOMPARE(b.maxPassengers(), 0); // 默认 0
        b.setMaxPassengers(50);
        QCOMPARE(b.maxPassengers(), 50);
    }

    void test_bus_monthlyTotalCost() {
        Bus b;
        b.setFuelConsumption(0.25);
        QCOMPARE(b.monthlyTotalCost(7.5), 7.5 * 0.25 + 2000.0);
    }

    // ==================== Truck 卡车 ====================

    void test_truck_typeName() {
        Truck t;
        QCOMPARE(t.typeName(), QString("卡车"));
    }

    void test_truck_vehicleType() {
        Truck t;
        QCOMPARE(t.vehicleType(), VehicleType::Truck);
    }

    void test_truck_maxLoad() {
        Truck t;
        QCOMPARE(t.maxLoad(), 0.0); // 默认 0
        t.setMaxLoad(15.5);
        QCOMPARE(t.maxLoad(), 15.5);
    }

    void test_truck_monthlyTotalCost() {
        Truck t;
        t.setFuelConsumption(0.35);
        QCOMPARE(t.monthlyTotalCost(7.5), 7.5 * 0.35 + 1500.0);
    }

    // ==================== JSON 序列化往返 ====================

    void test_car_json_roundtrip() {
        // 路径: toJson → fromJson 往返一致性
        Car original;
        original.setId("CAR-001");
        original.setPlateNumber("京A88888");
        original.setManufacturer("比亚迪");
        original.setPurchaseDate(QDate(2025, 1, 1));
        original.setTotalKm(30000.0);
        original.setFuelConsumption(0.06);
        original.setRoadFee(150.0);
        original.setAccumulatedCost(8000.0);
        original.setBasicMaintenance(1100.0);
        original.setTrunkType(2); // 两厢

        QJsonObject json = original.toJson();
        Car restored;
        restored.fromJson(json);

        QCOMPARE(restored.id(),                original.id());
        QCOMPARE(restored.plateNumber(),       original.plateNumber());
        QCOMPARE(restored.manufacturer(),      original.manufacturer());
        QCOMPARE(restored.purchaseDate(),      original.purchaseDate());
        QCOMPARE(restored.totalKm(),           original.totalKm());
        QCOMPARE(restored.fuelConsumption(),   original.fuelConsumption());
        QCOMPARE(restored.roadFee(),           original.roadFee());
        QCOMPARE(restored.accumulatedCost(),   original.accumulatedCost());
        QCOMPARE(restored.basicMaintenance(),  original.basicMaintenance());
        QCOMPARE(restored.trunkType(),         original.trunkType());
        QCOMPARE(restored.trunkTypeName(),     original.trunkTypeName());
    }

    void test_bus_json_roundtrip() {
        Bus original;
        original.setId("BUS-005");
        original.setMaxPassengers(55);

        QJsonObject json = original.toJson();
        Bus restored;
        restored.fromJson(json);

        QCOMPARE(restored.id(),             original.id());
        QCOMPARE(restored.maxPassengers(),  original.maxPassengers());
    }

    void test_truck_json_roundtrip() {
        Truck original;
        original.setId("TRUCK-010");
        original.setMaxLoad(20.5);

        QJsonObject json = original.toJson();
        Truck restored;
        restored.fromJson(json);

        QCOMPARE(restored.id(),       original.id());
        QCOMPARE(restored.maxLoad(),  original.maxLoad());
    }

    // ==================== 多态测试 ====================

    void test_polymorphic_typeName() {
        // 分支覆盖: 通过基类指针调用 → 运行时分发到子类
        Car car;
        Bus bus;
        Truck truck;

        Vehicle *v1 = &car;
        Vehicle *v2 = &bus;
        Vehicle *v3 = &truck;

        QCOMPARE(v1->typeName(), QString("小轿车"));
        QCOMPARE(v2->typeName(), QString("大客车"));
        QCOMPARE(v3->typeName(), QString("卡车"));
    }

    void test_polymorphic_monthlyCost() {
        Car car; car.setFuelConsumption(0.1);
        Bus bus; bus.setFuelConsumption(0.2);
        Truck truck; truck.setFuelConsumption(0.3);

        Vehicle *v1 = &car;
        Vehicle *v2 = &bus;
        Vehicle *v3 = &truck;

        QCOMPARE(v1->monthlyTotalCost(7.5), 7.5 * 0.1 + 1000.0);
        QCOMPARE(v2->monthlyTotalCost(7.5), 7.5 * 0.2 + 2000.0);
        QCOMPARE(v3->monthlyTotalCost(7.5), 7.5 * 0.3 + 1500.0);
    }
};

// 注册测试类 (Qt Test 宏)
// QTEST_MAIN(TestVehicle)
// 注意: 因为统一入口在 main.cpp, 这里不用 QTEST_MAIN
#include "tst_vehicle.moc"
