// ============================================================
// 白盒测试: VehicleManager 业务逻辑层
//
// 覆盖目标:
//   - 语句覆盖: 每个 public 方法至少执行一次
//   - 分支覆盖: addVehicle 4 个 if 各取 true/false
//   - 路径覆盖: updateVehicle 的 id变了×plate变了 共 4 条路径
//   - 边界覆盖: 容量 = 0, 1, 100, 101
//   - 数据驱动: 多次保存/加载往返
// ============================================================

#include <QtTest>
#include <QFile>
#include <QTemporaryFile>
#include "../vehiclemanager.h"
#include "../car.h"
#include "../bus.h"
#include "../truck.h"

class TestVehicleManager : public QObject {
    Q_OBJECT

private:
    VehicleManager *mgr = nullptr;

private slots:
    // ---- 每个测试用例前/后执行 ----
    void init() {
        // 每个测试前新建一个空管理器
        mgr = new VehicleManager();
    }

    void cleanup() {
        delete mgr;
        mgr = nullptr;
    }

    // ============================================================
    // addVehicle — 白盒分支覆盖
    // ============================================================

    void test_addVehicle_success() {
        // 路径: 通过全部 4 道校验
        auto *c = new Car();
        c->setId("CAR-001");
        QString err;
        QVERIFY(mgr->addVehicle(c, err));
        QCOMPARE(mgr->count(), 1);
        QVERIFY(err.isEmpty());
    }

    void test_addVehicle_null() {
        // 分支: if (!vehicle) → true
        QString err;
        QVERIFY(!mgr->addVehicle(nullptr, err));
        QVERIFY(!err.isEmpty());
        QCOMPARE(mgr->count(), 0);
    }

    void test_addVehicle_full() {
        // 分支: if (isFull()) → true
        mgr->setMaxCapacity(1);
        auto *c1 = new Car(); c1->setId("C1");
        QString err;
        mgr->addVehicle(c1, err);

        auto *c2 = new Car(); c2->setId("C2");
        QVERIFY(!mgr->addVehicle(c2, err)); // 容量满
        QVERIFY(err.contains("已满"));
        delete c2; // 添加失败需手动释放
        QCOMPARE(mgr->count(), 1);
    }

    void test_addVehicle_duplicateId() {
        // 分支: if (findById()) → true (编号重复)
        auto *c1 = new Car(); c1->setId("CAR-001");
        QString err;
        mgr->addVehicle(c1, err);

        auto *c2 = new Car(); c2->setId("CAR-001"); // 相同 ID
        QVERIFY(!mgr->addVehicle(c2, err));
        QVERIFY(err.contains("已存在"));
        delete c2;
        QCOMPARE(mgr->count(), 1);
    }

    void test_addVehicle_duplicatePlate() {
        // 分支: 车牌非空 && findByPlateNumber() → true
        auto *c1 = new Car(); c1->setId("C1");
        c1->setPlateNumber("京A12345");
        QString err;
        mgr->addVehicle(c1, err);

        auto *c2 = new Car(); c2->setId("C2");
        c2->setPlateNumber("京A12345"); // 相同车牌
        QVERIFY(!mgr->addVehicle(c2, err));
        QVERIFY(err.contains("已被使用"));
        delete c2;
    }

    void test_addVehicle_emptyPlate() {
        // 边界: 空车牌 → 跳过车牌校验
        auto *c1 = new Car(); c1->setId("C1");
        c1->setPlateNumber(""); // 空车牌
        QString err;
        QVERIFY(mgr->addVehicle(c1, err));

        // 再添加一辆空车牌 → 应该成功 (空车牌不参与唯一性检查)
        auto *c2 = new Car(); c2->setId("C2");
        c2->setPlateNumber("");
        QVERIFY(mgr->addVehicle(c2, err));
        QCOMPARE(mgr->count(), 2);
    }

    void test_addVehicle_multiple() {
        // 添加 3 种类型各一辆
        auto *bus = new Bus();   bus->setId("BUS-001");
        auto *car = new Car();   car->setId("CAR-001");
        auto *truck = new Truck(); truck->setId("TRUCK-001");
        QString err;
        QVERIFY(mgr->addVehicle(bus, err));
        QVERIFY(mgr->addVehicle(car, err));
        QVERIFY(mgr->addVehicle(truck, err));
        QCOMPARE(mgr->count(), 3);
    }

    // ============================================================
    // removeVehicle
    // ============================================================

    void test_removeVehicle_exists() {
        auto *c = new Car(); c->setId("X");
        QString err;
        mgr->addVehicle(c, err);

        QVERIFY(mgr->removeVehicle("X"));
        QCOMPARE(mgr->count(), 0);
    }

    void test_removeVehicle_notExists() {
        QVERIFY(!mgr->removeVehicle("NONEXISTENT"));
    }

    void test_removeVehicle_thenFind() {
        // 路径: 删除后 findById → nullptr
        auto *c = new Car(); c->setId("DEL-001");
        QString err;
        mgr->addVehicle(c, err);
        mgr->removeVehicle("DEL-001");
        QVERIFY(mgr->findById("DEL-001") == nullptr);
    }

    // ============================================================
    // updateVehicle — 路径覆盖 (id变/不变 × plate变/不变)
    // ============================================================

    void test_updateVehicle_null() {
        // 分支: newData == nullptr → true
        QString err;
        QVERIFY(!mgr->updateVehicle("A", nullptr, err));
        QVERIFY(!err.isEmpty());
    }

    void test_updateVehicle_notFound() {
        // 分支: findById → nullptr
        auto *newData = new Car(); newData->setId("X");
        QString err;
        QVERIFY(!mgr->updateVehicle("NONEXISTENT", newData, err));
        QVERIFY(err.contains("不存在"));
        delete newData;
    }

    void test_updateVehicle_sameId_samePlate() {
        // 路径: id 不变 + plate 不变 → 直接深拷贝
        auto *c = new Car(); c->setId("U-001");
        c->setPlateNumber("P001");
        c->setManufacturer("旧制造商");
        QString err;
        mgr->addVehicle(c, err);

        auto *newData = new Car(); newData->setId("U-001");
        newData->setPlateNumber("P001");
        newData->setManufacturer("新制造商"); // 只改这个
        QVERIFY(mgr->updateVehicle("U-001", newData, err));

        Vehicle *updated = mgr->findById("U-001");
        QCOMPARE(updated->manufacturer(), QString("新制造商"));
        QCOMPARE(updated->plateNumber(), QString("P001")); // 没变
        delete newData;
    }

    void test_updateVehicle_changeId_success() {
        // 路径: id 改变 + 无冲突
        auto *c = new Car(); c->setId("OLD");
        QString err;
        mgr->addVehicle(c, err);

        auto *newData = new Car(); newData->setId("NEW");
        QVERIFY(mgr->updateVehicle("OLD", newData, err));

        QVERIFY(mgr->findById("OLD") == nullptr); // 旧 ID 不可查
        QVERIFY(mgr->findById("NEW") != nullptr); // 新 ID 可查
        delete newData;
    }

    void test_updateVehicle_changeId_conflict() {
        // 路径: id 改变 + 新 ID 已被占用
        auto *c1 = new Car(); c1->setId("A");
        auto *c2 = new Car(); c2->setId("B");
        QString err;
        mgr->addVehicle(c1, err);
        mgr->addVehicle(c2, err);

        auto *newData = new Car(); newData->setId("B"); // 想改成已存在的 B
        QVERIFY(!mgr->updateVehicle("A", newData, err));
        QVERIFY(err.contains("已被使用"));
        QVERIFY(mgr->findById("A") != nullptr); // A 没被动到
        delete newData;
    }

    void test_updateVehicle_changePlate_conflict() {
        // 路径: plate 改变 + 新车牌冲突
        auto *c1 = new Car(); c1->setId("V1");
        c1->setPlateNumber("PLATE-A");
        auto *c2 = new Car(); c2->setId("V2");
        c2->setPlateNumber("PLATE-B");
        QString err;
        mgr->addVehicle(c1, err);
        mgr->addVehicle(c2, err);

        auto *newData = new Car(); newData->setId("V1");
        newData->setPlateNumber("PLATE-B"); // 想改成已存在的车牌
        QVERIFY(!mgr->updateVehicle("V1", newData, err));
        QVERIFY(err.contains("已被使用"));
        delete newData;
    }

    void test_updateVehicle_pointerRemains() {
        // 路径: 更新后 target 指针地址不变
        auto *c = new Car(); c->setId("PTR");
        QString err;
        mgr->addVehicle(c, err);
        Vehicle *originalPtr = mgr->findById("PTR");

        auto *newData = new Car(); newData->setId("PTR");
        newData->setManufacturer("变更");
        mgr->updateVehicle("PTR", newData, err);

        Vehicle *afterPtr = mgr->findById("PTR");
        QCOMPARE(originalPtr, afterPtr); // 同一地址!
        delete newData;
    }

    // ============================================================
    // 查找操作 — 5 种策略
    // ============================================================

    void test_findById_exact() {
        auto *c = new Car(); c->setId("FIND-ME");
        QString err;
        mgr->addVehicle(c, err);

        QVERIFY(mgr->findById("FIND-ME") != nullptr);
        QVERIFY(mgr->findById("find-me") == nullptr); // 大小写敏感
        QVERIFY(mgr->findById("OTHER") == nullptr);
    }

    void test_findByPlateNumber_exact() {
        auto *c = new Car(); c->setId("P1");
        c->setPlateNumber("京A12345");
        QString err;
        mgr->addVehicle(c, err);

        QVERIFY(mgr->findByPlateNumber("京A12345") != nullptr);
        QVERIFY(mgr->findByPlateNumber("京A67890") == nullptr);
    }

    void test_findByPlateNumberFuzzy() {
        auto *c = new Car(); c->setId("F1");
        c->setPlateNumber("京A12345");
        QString err;
        mgr->addVehicle(c, err);

        auto result = mgr->findByPlateNumberFuzzy("京A");
        QCOMPARE(result.size(), 1);
        QCOMPARE(result[0]->id(), QString("F1"));
    }

    void test_findByPlateNumberFuzzy_caseInsensitive() {
        auto *c = new Car(); c->setId("F2");
        c->setPlateNumber("ABC123");
        QString err;
        mgr->addVehicle(c, err);

        auto result = mgr->findByPlateNumberFuzzy("abc"); // 小写查大写
        QCOMPARE(result.size(), 1);
    }

    void test_findByManufacturer_fuzzy() {
        auto *c = new Car(); c->setId("M1");
        c->setManufacturer("一汽大众汽车有限公司");
        QString err;
        mgr->addVehicle(c, err);

        auto result = mgr->findByManufacturer("大众"); // 部分匹配
        QCOMPARE(result.size(), 1);
    }

    void test_findByType() {
        auto *bus = new Bus();     bus->setId("B1");
        auto *car = new Car();     car->setId("C1");
        auto *truck = new Truck(); truck->setId("T1");
        QString err;
        mgr->addVehicle(bus, err);
        mgr->addVehicle(car, err);
        mgr->addVehicle(truck, err);

        QCOMPARE(mgr->findByType(VehicleType::Bus).size(),   1);
        QCOMPARE(mgr->findByType(VehicleType::Car).size(),   1);
        QCOMPARE(mgr->findByType(VehicleType::Truck).size(), 1);
    }

    void test_search_emptyResult() {
        // 边界: 空容器中搜索
        QVERIFY(mgr->findByManufacturer("不存在").isEmpty());
        QVERIFY(mgr->findByType(VehicleType::Bus).isEmpty());
    }

    // ============================================================
    // 统计
    // ============================================================

    void test_statistics_empty() {
        Statistics s = mgr->getStatistics();
        QCOMPARE(s.total, 0);
        QCOMPARE(s.busCount, 0);
        QCOMPARE(s.carCount, 0);
        QCOMPARE(s.truckCount, 0);
    }

    void test_statistics_mixed() {
        QString err;
        auto *b1 = new Bus();   b1->setId("B1"); mgr->addVehicle(b1, err);
        auto *b2 = new Bus();   b2->setId("B2"); mgr->addVehicle(b2, err);
        auto *c1 = new Car();   c1->setId("C1"); mgr->addVehicle(c1, err);
        auto *t1 = new Truck(); t1->setId("T1"); mgr->addVehicle(t1, err);

        Statistics s = mgr->getStatistics();
        QCOMPARE(s.total, 4);
        QCOMPARE(s.busCount, 2);
        QCOMPARE(s.carCount, 1);
        QCOMPARE(s.truckCount, 1);
    }

    // ============================================================
    // 文件 I/O — 往返测试
    // ============================================================

    void test_saveLoad_roundtrip() {
        // 路径: 构造数据 → 保存 → 清空 → 加载 → 比对
        QString err;
        auto *car = new Car();
        car->setId("CAR-001");
        car->setPlateNumber("京A88888");
        car->setManufacturer("比亚迪");
        car->setTrunkType(2);
        mgr->addVehicle(car, err);

        auto *bus = new Bus();
        bus->setId("BUS-001");
        bus->setMaxPassengers(45);
        mgr->addVehicle(bus, err);

        mgr->setFuelPrice(8.5);
        mgr->setMaxCapacity(50);

        QString testFile = "test_roundtrip.json";

        // 保存
        QVERIFY(mgr->saveToFile(testFile, err));

        // 清空
        mgr->clearAll();
        QCOMPARE(mgr->count(), 0);

        // 加载
        QVERIFY(mgr->loadFromFile(testFile, err));

        // 验证
        QCOMPARE(mgr->count(), 2);
        QCOMPARE(mgr->fuelPrice(), 8.5);
        QCOMPARE(mgr->maxCapacity(), 50);

        Vehicle *loaded = mgr->findById("CAR-001");
        QVERIFY(loaded != nullptr);
        QCOMPARE(loaded->plateNumber(), QString("京A88888"));
        QCOMPARE(loaded->manufacturer(), QString("比亚迪"));

        // 验证子类字段
        Car *loadedCar = dynamic_cast<Car *>(loaded);
        QVERIFY(loadedCar != nullptr);
        QCOMPARE(loadedCar->trunkType(), 2);

        Bus *loadedBus = dynamic_cast<Bus *>(mgr->findById("BUS-001"));
        QVERIFY(loadedBus != nullptr);
        QCOMPARE(loadedBus->maxPassengers(), 45);

        // 清理
        QFile::remove(testFile);
    }

    void test_loadFromFile_notExist() {
        QString err;
        QVERIFY(!mgr->loadFromFile("__nonexistent_file__.json", err));
        QVERIFY(!err.isEmpty());
    }

    void test_saveToFile_empty() {
        // 边界: 空库保存
        QString err;
        QString testFile = "test_empty.json";
        QVERIFY(mgr->saveToFile(testFile, err));

        // 加载回来 → 0 条
        mgr->loadFromFile(testFile, err);
        QCOMPARE(mgr->count(), 0);
        QFile::remove(testFile);
    }

    // 测试旧 JSON 格式兼容性
    void test_loadFromFile_oldFormat() {
        QString testFile = "test_oldformat.json";
        QFile f(testFile);
        f.open(QIODevice::WriteOnly | QIODevice::Text);
        // 旧格式: 顶层为纯数组，无 fuelPrice/maxCapacity
        f.write(R"([
            {"id":"OLD-001","plateNumber":"京A11111","manufacturer":"测试","purchaseDate":"2024-01-01","vehicleType":"小轿车","totalKm":1000,"fuelConsumption":0.05,"basicMaintenance":1000,"roadFee":100,"accumulatedCost":5000,"trunkType":3},
            {"id":"OLD-002","plateNumber":"京B22222","manufacturer":"宇通","purchaseDate":"2023-06-15","vehicleType":"大客车","totalKm":50000,"fuelConsumption":0.2,"basicMaintenance":2000,"roadFee":500,"accumulatedCost":30000,"maxPassengers":40}
        ])");
        f.close();

        QString err;
        QVERIFY(mgr->loadFromFile(testFile, err));
        QCOMPARE(mgr->count(), 2);
        QCOMPARE(mgr->fuelPrice(), 7.5);   // 旧格式 → 默认油价
        QCOMPARE(mgr->maxCapacity(), 100);  // 旧格式 → 默认容量

        QFile::remove(testFile);
    }

    void test_loadFromFile_corrupted() {
        QString testFile = "test_corrupt.json";
        QFile f(testFile);
        f.open(QIODevice::WriteOnly | QIODevice::Text);
        f.write("这不是 JSON {{{");
        f.close();

        QString err;
        QVERIFY(!mgr->loadFromFile(testFile, err));
        QVERIFY(err.contains("解析失败"));

        QFile::remove(testFile);
    }

    // ============================================================
    // 容量与配置管理
    // ============================================================

    void test_capacity_default() {
        QCOMPARE(mgr->maxCapacity(), 100);
    }

    void test_capacity_setValid() {
        mgr->setMaxCapacity(50);
        QCOMPARE(mgr->maxCapacity(), 50);
    }

    void test_capacity_belowCount() {
        // 边界: 设置容量小于当前车辆数 → 自动修正
        QString err;
        for (int i = 0; i < 5; i++) {
            auto *c = new Car();
            c->setId(QString("CAP-%1").arg(i));
            mgr->addVehicle(c, err);
        }
        mgr->setMaxCapacity(3); // 尝试设成 3，但已有 5 辆
        QCOMPARE(mgr->maxCapacity(), 5); // 自动修正为当前数量
    }

    void test_capacity_belowOne() {
        // 边界: 设置 <1 → 修正为 1
        mgr->setMaxCapacity(0);
        QCOMPARE(mgr->maxCapacity(), 1);
        mgr->setMaxCapacity(-5);
        QCOMPARE(mgr->maxCapacity(), 1);
    }

    void test_isFull_isEmpty() {
        QVERIFY(mgr->isEmpty());
        QVERIFY(!mgr->isFull());

        mgr->setMaxCapacity(2);
        QString err;
        auto *c1 = new Car(); c1->setId("A"); mgr->addVehicle(c1, err);
        QVERIFY(!mgr->isEmpty());
        QVERIFY(!mgr->isFull());

        auto *c2 = new Car(); c2->setId("B"); mgr->addVehicle(c2, err);
        QVERIFY(mgr->isFull());
    }

    void test_fuelPrice_default() {
        QCOMPARE(mgr->fuelPrice(), 7.5);
    }

    void test_fuelPrice_setter() {
        mgr->setFuelPrice(9.25);
        QCOMPARE(mgr->fuelPrice(), 9.25);
    }

    void test_clearAll() {
        QString err;
        auto *c1 = new Car(); c1->setId("A"); mgr->addVehicle(c1, err);
        auto *c2 = new Car(); c2->setId("B"); mgr->addVehicle(c2, err);
        QCOMPARE(mgr->count(), 2);

        mgr->clearAll();
        QCOMPARE(mgr->count(), 0);
        QVERIFY(mgr->isEmpty());
    }
};

#include "tst_vehiclemanager.moc"
