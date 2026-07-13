
# 面向对象程序设计（C++）课程设计报告

---

## 2025-2026学年 第2学期

---

**题目：基于Qt的车辆管理系统（VMS）的设计与实现**

| 学生姓名 | ____________ |
| 学 号 | ____________ |
| 班 级 | ____________ |
| 指导教师 | ____________ |
| 提交日期 | 2026.7.17 |

---

## 目录

1. [引言](#1-引言)
2. [需求分析](#2-需求分析)
   - 2.1 功能需求
     - 2.1.1 功能性需求
     - 2.1.2 非功能性需求
   - 2.2 开发环境与技术选型
3. [系统设计](#3-系统设计)
   - 3.1 面向对象设计（UML类图等）
   - 3.2 数据存储设计
   - 3.3 界面设计
4. [功能实现](#4-功能实现)
   - 4.1 关键功能模块的实现
   - 4.2 关键技术难点与解决方案
5. [总结](#5-总结)
   - 5.1 结论与分析
   - 5.2 收获与体会
6. [参考文献](#6-参考文献)

---

## 1 引言

随着社会经济的快速发展，各类企事业单位拥有的车辆数量不断增加，车辆管理工作日趋复杂。传统的纸质记录或简单的电子表格方式已难以满足现代车辆管理的需求——数据查询不便、统计困难、信息容易丢失等问题日益突出。

基于此背景，本课程设计项目开发了一款**车辆管理系统（Vehicle Management System，VMS）**，旨在为企事业单位提供一套高效、便捷的车辆信息管理解决方案。系统涵盖大客车、小轿车、卡车三类常见车辆，支持车辆信息的增删改查、多维度组合查询、数据统计可视化、JSON 文件持久化存储等功能。

本项目采用 **C++** 语言开发，基于 **Qt 6** 框架构建图形用户界面，综合运用了面向对象程序设计的核心概念——**封装、继承、多态、抽象类**，并结合 **STL 算法** 与 **Qt Charts** 图表库实现数据统计的可视化展示。

---

## 2 需求分析

### 2.1 功能需求

#### 2.1.1 功能性需求

通过对车辆管理实际业务场景的分析，本系统需具备以下核心功能：

1. **车辆信息管理**
   - 支持添加车辆：录入车辆编号、车牌号、制造公司、购买时间、总公里数、耗油量/公里、养路费、累计总费用等通用信息，并根据车辆类别录入专属信息（大客车的载客量、小轿车的厢数、卡车的载重量）。
   - 支持编辑车辆：选中某条车辆记录后可修改其全部字段。
   - 支持删除车辆：提供三种删除方式——选中行删除、按编号精确删除、批量多选删除。
   - 车辆编号自动生成：根据车辆类别自动生成 BUS-001、CAR-001、TRUCK-001 格式的唯一编号，且自动检测同类编号中最大数字以避免冲突。

2. **车辆查询功能**
   - 支持按**编号**精确查询。
   - 支持按**车牌号**模糊查询（不区分大小写）。
   - 支持按**制造公司**模糊查询。
   - 支持按**类别**（大客车/小轿车/卡车）分类查询。
   - 支持"显示全部"一键恢复所有车辆列表。

3. **数据排序**
   - 支持点击表头按任意列升序/降序排列，包括编号、车牌号、制造公司、购买时间、类别、总公里数、耗油量、维护费、养路费、累计总费用、当月总费用及特殊属性。

4. **统计功能**
   - 显示各类车辆的数量统计（总计/大客车/小轿车/卡车）。
   - 以**饼图（甜甜圈图）** 展示车辆类别分布占比。
   - 以**柱状图**对比各类型车辆数量与平均月费用。

5. **数据持久化**
   - 支持将全部车辆信息（含油价和容量上限）保存为 **JSON 格式文件**。
   - 支持从 JSON 文件加载数据，兼容新旧两种文件格式（顶层数组/顶层对象）。
   - 关闭窗口时检测未保存修改并提示用户保存。

6. **油价管理**
   - 支持动态设置当前油价（元/升），表格中当月总费用实时联动更新。

7. **容量管理**
   - 支持设置车辆信息库的最大容量上限，添加车辆时自动校验是否已满。

8. **其他便捷功能**
   - 右键上下文菜单（添加、编辑、删除、复制编号、复制车牌号）。
   - 双击行快速编辑。
   - Delete 快捷键批量删除。
   - 状态栏实时显示车辆容量和各类车辆数量。

#### 2.1.2 非功能性需求

1. **界面友好性**：采用 Qt 框架构建直观的图形用户界面，表格交替行色便于阅读，状态栏实时反馈数据概况。
2. **数据完整性**：添加和编辑车辆时进行输入校验（编号非空、车牌号长度合法性检查），确保数据质量。
3. **操作安全性**：删除操作均需二次确认，关闭窗口时检测未保存修改并提示。
4. **可扩展性**：采用面向对象的多态架构设计，新增车辆类型只需添加新的派生类，无需修改核心管理逻辑。
5. **代码规范性**：遵循 C++17 标准，使用 `QStringLiteral` 优化字符串性能，合理的注释和命名规范。

### 2.2 开发环境与技术选型

| 项目 | 内容 |
|------|------|
| 操作系统 | Windows 11 |
| 编程语言 | C++17 |
| GUI 框架 | Qt 6.2（含 Qt Charts 模块） |
| 构建工具 | qmake / Qt Creator |
| 数据存储 | JSON 格式文件（基于 QJsonDocument） |
| 版本控制 | Git |

---

## 3 系统设计

### 3.1 面向对象设计（UML类图）

#### 3.1.1 整体架构设计

系统采用**三层架构**设计：

```
┌─────────────────────────────────┐
│        表示层 (UI Layer)         │
│  MainWindow / AddVehicleDialog  │
│  EditVehicleDialog /            │
│  StatisticsDialog               │
├─────────────────────────────────┤
│       业务逻辑层 (BLL)           │
│       VehicleManager            │
├─────────────────────────────────┤
│       数据模型层 (Model)         │
│  Vehicle / Bus / Car / Truck    │
└─────────────────────────────────┘
```

- **数据模型层**：定义车辆的核心数据结构，通过继承实现多态。
- **业务逻辑层**：`VehicleManager` 负责所有车辆的增删改查、统计运算和文件 I/O。
- **表示层**：`MainWindow` 为主窗口，`AddVehicleDialog`、`EditVehicleDialog`、`StatisticsDialog` 为各类对话框。

#### 3.1.2 车辆类体系（核心继承树）

系统定义了一个抽象基类 `Vehicle` 和三个具体派生类 `Bus`、`Car`、`Truck`，构成了典型的**多态继承**体系。

**Vehicle（抽象基类）**

```
class Vehicle {
protected:
    QString m_id;                // 车辆编号
    QString m_plateNumber;       // 车牌号
    QString m_manufacturer;      // 制造公司
    QDate   m_purchaseDate;      // 购买日期
    VehicleType m_vehicleType;   // 车辆类别枚举
    double  m_totalKm;           // 总公里数
    double  m_fuelConsumption;   // 耗油量/公里 (L/km)
    double  m_basicMaintenance;  // 基本维护费 (元/月)
    double  m_roadFee;           // 养路费
    double  m_accumulatedCost;   // 累计总费用
public:
    // 纯虚函数 —— 定义子类必须实现的接口
    virtual double monthlyTotalCost(double fuelPrice) const = 0;
    virtual QString typeName() const = 0;

    // 虚函数 —— JSON 序列化/反序列化，子类可重写扩展
    virtual QJsonObject toJson() const;
    virtual void fromJson(const QJsonObject &obj);

    // 各属性的 getter/setter ...
};
```

**设计要点**：
- `monthlyTotalCost()` 和 `typeName()` 声明为**纯虚函数**，强制所有派生类实现各自的计费逻辑和类型标识。
- `toJson()` 和 `fromJson()` 声明为**虚函数**，基类处理通用字段的序列化，派生类重写时调用基类版本并追加专属字段。
- 构造函数设为 `protected`，仅允许派生类通过指定 `VehicleType` 和默认维护费来构造。

**派生类 Bus（大客车）**

```
class Bus : public Vehicle {
private:
    int m_maxPassengers = 0;     // 专属属性：最大载客量
public:
    Bus();                        // 默认维护费 2000 元/月
    int maxPassengers() const;
    QString typeName() const override;       // 返回 "大客车"
    double monthlyTotalCost(double fuelPrice) const override;
    QJsonObject toJson() const override;     // 追加 maxPassengers
    void fromJson(const QJsonObject &obj) override;
};
```

**派生类 Car（小轿车）**

```
class Car : public Vehicle {
private:
    int m_trunkType = 3;         // 专属属性：厢数（2=两厢, 3=三厢）
public:
    Car();                        // 默认维护费 1000 元/月
    QString trunkTypeName() const;           // 返回 "两厢"/"三厢"
    QString typeName() const override;       // 返回 "小轿车"
    double monthlyTotalCost(double fuelPrice) const override;
    QJsonObject toJson() const override;     // 追加 trunkType
    void fromJson(const QJsonObject &obj) override;
};
```

**派生类 Truck（卡车）**

```
class Truck : public Vehicle {
private:
    double m_maxLoad = 0;        // 专属属性：最大载重量（吨）
public:
    Truck();                      // 默认维护费 1500 元/月
    double maxLoad() const;
    QString typeName() const override;       // 返回 "卡车"
    double monthlyTotalCost(double fuelPrice) const override;
    QJsonObject toJson() const override;     // 追加 maxLoad
    void fromJson(const QJsonObject &obj) override;
};
```

**当月总费用计算公式**：

所有车辆类型的当月总费用统一按此公式计算：

$$MonthlyCost = FuelPrice \times FuelConsumption + BasicMaintenance$$

其中油价（FuelPrice）由 `VehicleManager` 统一管理，作为参数传入，确保了计费逻辑的**灵活性**——油价变动时无需修改每辆车的内部数据，仅需修改管理器的油价属性。

#### 3.1.3 VehicleManager（业务逻辑核心）

```
class VehicleManager {
private:
    QVector<Vehicle*> m_vehicles;     // 车辆指针容器（多态存储）
    int     m_maxCapacity = 100;      // 最大容量
    double  m_fuelPrice = 7.5;        // 油价（元/升）
public:
    // CRUD 操作
    bool addVehicle(Vehicle *vehicle, QString &errorMsg);
    bool removeVehicle(const QString &id);
    Vehicle* findById(const QString &id) const;
    QVector<Vehicle*> findByPlateNumberFuzzy(const QString &keyword) const;
    QVector<Vehicle*> findByManufacturer(const QString &keyword) const;
    QVector<Vehicle*> findByType(VehicleType type) const;
    bool updateVehicle(const QString &oldId, Vehicle *newData, QString &errorMsg);

    // 统计
    Statistics getStatistics() const;

    // 文件 I/O
    bool saveToFile(const QString &filePath, QString &errorMsg) const;
    bool loadFromFile(const QString &filePath, QString &errorMsg);
};
```

**设计要点**：
- 使用 `QVector<Vehicle*>` 存储基类指针，借助**运行时多态**统一管理不同类型的车辆。
- 添加车辆时校验编号和车牌号的**唯一性**。
- 更新车辆通过 JSON 中转实现深层拷贝，保持原对象指针不变。
- `loadFromFile()` 支持**新旧格式兼容**：旧格式为纯 JSON 数组，新格式为包含 `fuelPrice`、`maxCapacity` 和 `vehicles` 的对象。

#### 3.1.4 GUI 类设计

- **MainWindow**：主窗口，包含菜单栏、工具栏、搜索面板、车辆表格、状态栏。管理 `VehicleManager` 实例，响应用户操作。
- **AddVehicleDialog**：添加车辆对话框，使用 `QStackedWidget` 根据车辆类型动态切换专属字段面板。
- **EditVehicleDialog**：编辑车辆对话框，结构与添加对话框类似，额外支持预填当前数据。
- **StatisticsDialog**：统计对话框，展示数量摘要、饼图和柱状图（依赖 Qt Charts 模块）。

### 3.2 数据存储设计

本系统采用 **JSON** 作为数据持久化格式。相比传统的关系型数据库，JSON 文件具有**轻量级、易于人工编辑和调试、无需额外数据库软件**的优势，适合课程设计规模的应用。

**JSON 文件结构（新格式）**：

```json
{
    "fuelPrice": 7.5,
    "maxCapacity": 100,
    "vehicles": [
        {
            "id": "BUS-001",
            "plateNumber": "京A12345",
            "manufacturer": "宇通客车",
            "purchaseDate": "2024-01-15",
            "vehicleType": "大客车",
            "totalKm": 50000.0,
            "fuelConsumption": 0.25,
            "basicMaintenance": 2000.0,
            "roadFee": 600.0,
            "accumulatedCost": 35000.0,
            "maxPassengers": 55
        },
        {
            "id": "CAR-001",
            "plateNumber": "京B67890",
            "manufacturer": "一汽大众",
            "purchaseDate": "2023-06-20",
            "vehicleType": "小轿车",
            "totalKm": 32000.0,
            "fuelConsumption": 0.08,
            "basicMaintenance": 1000.0,
            "roadFee": 400.0,
            "accumulatedCost": 18000.0,
            "trunkType": 3
        }
    ]
}
```

**设计要点**：
- 每个车辆对象包含通用字段和类型专属字段，通过 `vehicleType` 字段区分。
- 油价和容量上限作为全局配置存储在顶层。
- 加载时根据 `vehicleType` 字符串动态创建对应的派生类对象（Bus/Car/Truck），体现了**工厂模式**的思想。

### 3.3 界面设计

系统主界面由以下区域构成：

```
┌──────────────────────────────────────────┐
│  菜单栏：文件 | 操作 | 统计 | 帮助        │
├──────────────────────────────────────────┤
│  工具栏：添加 编辑 删除 ... 统计          │
├──────────────────────────────────────────┤
│  搜索面板：[搜索框] ○按编号 ○按车牌号...  │
│            [🔍 查询] [📋 显示全部]        │
├──────────────────────────────────────────┤
│                                           │
│  车辆信息表格 (QTableWidget)              │
│  ┌────┬──────┬──────┬────┬───┬──────┐   │
│  │编号│车牌号│制造..│购买│..│特殊属性│   │
│  ├────┼──────┼──────┼────┼───┼──────┤   │
│  │    │      │      │    │   │      │   │
│  └────┴──────┴──────┴────┴───┴──────┘   │
│                                           │
├──────────────────────────────────────────┤
│  状态栏：容量: 3/100  总计: 3辆 客车:1... │
└──────────────────────────────────────────┘
```

- **菜单栏**：提供文件操作（打开/保存/另存为）、车辆操作（增删改查/油价/容量）、统计、帮助等全部功能入口。
- **工具栏**：高频操作的快捷入口。
- **搜索面板**：组合框 + 单选按钮，支持四种查询模式。
- **车辆表格**：12列表格，支持交替行色、整行选中、表头点击排序、右键菜单、双击编辑。
- **状态栏**：实时显示容量使用情况和各类车辆数量。

---

## 4 功能实现

### 4.1 关键功能模块的实现

#### 4.1.1 车辆基类的多态设计

**功能描述**：定义统一的车辆抽象接口，使上层代码无需关心具体车辆类型即可完成通用操作。

**实现方式**：

```cpp
// vehicle.h - 抽象基类定义
class Vehicle {
public:
    // 纯虚函数：强制子类实现
    virtual double monthlyTotalCost(double fuelPrice) const = 0;
    virtual QString typeName() const = 0;

    // 虚函数：提供默认实现，子类可选重写
    virtual QJsonObject toJson() const;
    virtual void fromJson(const QJsonObject &obj);

protected:
    // 构造函数为 protected，禁止直接实例化基类
    Vehicle(VehicleType type, double defaultMaintenance);
    // ... 成员变量
};
```

**设计说明**：
- 将 `monthlyTotalCost()` 和 `typeName()` 设为纯虚函数，确保每个派生类都实现自己的计费和标识逻辑。若未来新增车辆类型（如摩托车、工程车），编译器会强制要求实现这两个函数，避免遗漏。
- `toJson()` 和 `fromJson()` 使用**虚函数而非纯虚函数**，因为基类已处理了通用字段（id、plateNumber 等），派生类只需调用 `Vehicle::toJson()` 后追加自己的专属字段即可，避免代码重复。

#### 4.1.2 VehicleManager 增删改查实现

**添加车辆**：

```cpp
bool VehicleManager::addVehicle(Vehicle *vehicle, QString &errorMsg) {
    if (!vehicle) {
        errorMsg = "车辆对象为空！";
        return false;
    }
    if (isFull()) {
        errorMsg = "车辆信息库已满，无法添加！";
        return false;
    }
    // 编号唯一性检查
    if (findById(vehicle->id())) {
        errorMsg = "编号 " + vehicle->id() + " 已存在，添加失败！";
        return false;
    }
    // 车牌号唯一性检查（空车牌号允许重复）
    if (!vehicle->plateNumber().isEmpty()
        && findByPlateNumber(vehicle->plateNumber())) {
        errorMsg = "车牌号 " + vehicle->plateNumber() + " 已被使用，添加失败！";
        return false;
    }
    m_vehicles.append(vehicle);
    return true;
}
```

**设计要点**：
- 添加前进行**三重校验**：空指针检查、容量检查、编号/车牌唯一性检查。
- 使用 `QString &errorMsg` 输出参数返回详细错误信息，便于 UI 层展示。

**查询功能（四种模式）**：

```cpp
// 按编号精确查询 → 返回单个对象或 nullptr
Vehicle *VehicleManager::findById(const QString &id) const;

// 按车牌号模糊查询 → 使用 contains() 实现不区分大小写的子串匹配
QVector<Vehicle *> VehicleManager::findByPlateNumberFuzzy(
    const QString &keyword) const;

// 按制造公司模糊查询
QVector<Vehicle *> VehicleManager::findByManufacturer(
    const QString &keyword) const;

// 按类别精确查询 → 基于 VehicleType 枚举匹配
QVector<Vehicle *> VehicleManager::findByType(VehicleType type) const;
```

**更新车辆**：

```cpp
bool VehicleManager::updateVehicle(const QString &oldId,
                                    Vehicle *newData, QString &errorMsg) {
    Vehicle *target = findById(oldId);
    if (!target) { /* 错误处理 */ }
    // 若编号变更，检查新编号唯一性
    if (oldId != newData->id() && findById(newData->id())) { /* 错误处理 */ }
    // 若车牌变更，检查新车牌唯一性
    // 通过 JSON 中转实现深拷贝
    QJsonObject json = newData->toJson();
    target->fromJson(json);
    return true;
}
```

**设计亮点**：更新操作通过 JSON 中转实现"深拷贝"，而非直接替换指针。这样做的好处是：表格中持有的 `Vehicle*` 指针保持不变，无需重建 UI 数据。

**删除车辆**：

```cpp
bool VehicleManager::removeVehicle(const QString &id) {
    for (int i = 0; i < m_vehicles.size(); ++i) {
        if (m_vehicles[i]->id() == id) {
            delete m_vehicles[i];       // 释放堆内存
            m_vehicles.removeAt(i);     // 从容器中移除
            return true;
        }
    }
    return false;
}
```

#### 4.1.3 JSON 文件持久化

**保存到文件**：

```cpp
bool VehicleManager::saveToFile(const QString &filePath,
                                 QString &errorMsg) const {
    QJsonArray arr;
    for (auto *v : m_vehicles) {
        QJsonObject obj = v->toJson();   // 多态调用
        obj["vehicleType"] = vehicleTypeToString(v->vehicleType());
        arr.append(obj);
    }

    QJsonObject root;
    root["fuelPrice"] = m_fuelPrice;
    root["maxCapacity"] = m_maxCapacity;
    root["vehicles"] = arr;

    QJsonDocument doc(root);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        errorMsg = "无法打开文件进行写入！";
        return false;
    }
    file.write(doc.toJson(QJsonDocument::Indented)); // 缩进格式化
    file.close();
    return true;
}
```

**从文件加载（兼容新旧格式）**：

```cpp
bool VehicleManager::loadFromFile(const QString &filePath,
                                   QString &errorMsg) {
    // ... 读取文件、解析 JSON ...
    clearAll();  // 清空现有数据

    // 兼容旧格式（纯数组）和新格式（对象含 fuelPrice）
    QJsonArray arr;
    if (doc.isArray()) {
        arr = doc.array();               // 旧格式
    } else if (doc.isObject()) {
        QJsonObject root = doc.object();
        m_fuelPrice = root["fuelPrice"].toDouble(m_fuelPrice);
        m_maxCapacity = root["maxCapacity"].toInt(m_maxCapacity);
        arr = root["vehicles"].toArray(); // 新格式
    }

    for (const QJsonValue &val : arr) {
        QString typeStr = obj["vehicleType"].toString();
        VehicleType vt = stringToVehicleType(typeStr);
        Vehicle *v = nullptr;
        switch (vt) {                     // 根据类型创建对应派生类
        case VehicleType::Bus:   v = new Bus();   break;
        case VehicleType::Car:   v = new Car();   break;
        case VehicleType::Truck: v = new Truck(); break;
        }
        if (v) {
            v->fromJson(obj);             // 多态反序列化
            m_vehicles.append(v);
        }
    }
    return true;
}
```

**设计亮点**：加载时根据 `vehicleType` 字段动态创建派生类对象，体现了**简单工厂模式**的思路；兼容新旧两种 JSON 格式，保证了用户数据的向前兼容性。

#### 4.1.4 表头点击排序

系统支持点击任意表头进行升序/降序排序，使用 C++ STL 的 `std::sort` 配合 **Lambda 表达式**实现：

```cpp
void MainWindow::onHeaderClicked(int section) {
    // 切换升降序状态
    if (section == m_sortColumn) {
        m_sortOrder = (m_sortOrder == Qt::AscendingOrder)
                          ? Qt::DescendingOrder : Qt::AscendingOrder;
    } else {
        m_sortColumn = section;
        m_sortOrder = Qt::AscendingOrder;
    }

    QVector<Vehicle *> sorted = m_displayedVehicles;
    bool asc = (m_sortOrder == Qt::AscendingOrder);

    switch (section) {
    case 0: // 按编号排序
        std::sort(sorted.begin(), sorted.end(),
            [asc](Vehicle *a, Vehicle *b) {
                return asc ? a->id() < b->id() : a->id() > b->id();
            });
        break;
    case 5: // 按总公里数排序（数值列）
        std::sort(sorted.begin(), sorted.end(),
            [asc](Vehicle *a, Vehicle *b) {
                return asc ? a->totalKm() < b->totalKm()
                           : a->totalKm() > b->totalKm();
            });
        break;
    case 11: // 按特殊属性排序 —— 需 dynamic_cast 获取派生类专属数据
        std::sort(sorted.begin(), sorted.end(),
            [asc](Vehicle *a, Vehicle *b) {
                auto special = [](Vehicle *v) -> QString {
                    if (auto *bus = dynamic_cast<Bus *>(v))
                        return QString("载客量: %1 人").arg(bus->maxPassengers());
                    if (auto *car = dynamic_cast<Car *>(v))
                        return car->trunkTypeName();
                    if (auto *truck = dynamic_cast<Truck *>(v))
                        return QString("载重量: %1 吨").arg(truck->maxLoad());
                    return QString();
                };
                return asc ? special(a) < special(b) : special(a) > special(b);
            });
        break;
    // ... 其他列类似
    }
    refreshTable(sorted);
}
```

**设计要点**：对于"特殊属性"列，由于不同派生类的专属属性类型不同，需要借助 `dynamic_cast` 进行**运行时类型识别（RTTI）**。Lambda 辅助函数将不同类型车辆的专属属性统一转换为 `QString`，再用字符串比较实现排序，是一种简洁实用的处理方式。

#### 4.1.5 统计图表可视化

`StatisticsDialog` 使用 **Qt Charts** 模块绘制两类图表：

- **饼图（甜甜圈风格）**：展示大客车/小轿车/卡车的数量占比，设置 `setHoleSize(0.40)` 实现中空圆环效果，颜色区分使用 Material Design 风格配色（绿/蓝/橙）。
- **柱状图（分组对比）**：两组 BarSet 分别表示"车辆数量"和"平均月费用"，横向按三个车型分组展示，直观对比各车型的成本差异。

```cpp
// 饼图
QPieSeries *pieSeries = new QPieSeries();
pieSeries->setHoleSize(0.40);  // 甜甜圈风格
// 添加各类型切片并配置颜色和标签...

// 柱状图
QBarSet *setCount = new QBarSet("车辆数量");
*setCount << s.busCount << s.carCount << s.truckCount;
QBarSet *setCost = new QBarSet("平均月费用");
*setCost << avgBusCost << avgCarCost << avgTruckCost;
// 组装为 QBarSeries，配置坐标轴...
```

### 4.2 关键技术难点与解决方案

#### 难点一：多态体系下的派生类专属属性访问

**问题描述**：系统使用 `QVector<Vehicle*>` 统一存储所有车辆，但不同派生类有各自的专属属性（载客量、厢数、载重量）。在表格渲染、数据排序和统计时，需要访问这些专属属性，但基类指针无法直接调用派生类方法。

**解决方案**：使用 C++ 的 **`dynamic_cast`** 进行安全的向下转型：

```cpp
// 渲染特殊属性列
if (auto *bus = dynamic_cast<Bus *>(v)) {
    special = QStringLiteral("载客量: %1 人").arg(bus->maxPassengers());
} else if (auto *car = dynamic_cast<Car *>(v)) {
    special = car->trunkTypeName();
} else if (auto *truck = dynamic_cast<Truck *>(v)) {
    special = QStringLiteral("载重量: %1 吨").arg(truck->maxLoad());
}
```

`dynamic_cast` 在运行时检查类型安全性，转换失败返回 `nullptr`，避免了 `static_cast` 可能导致的未定义行为。在 `AddVehicleDialog` 和 `EditVehicleDialog` 中同样使用了这一技术来读取/写入专属字段。

#### 难点二：JSON 多态序列化与反序列化

**问题描述**：保存数据时需要将不同的派生类对象序列化为 JSON，加载时需要根据 JSON 中的类型字段重建正确的派生类对象。基类的序列化逻辑不能丢失派生类的额外数据。

**解决方案**：采用**虚函数链式调用**模式：

- **序列化**：派生类的 `toJson()` 先调用基类的 `Vehicle::toJson()` 获取通用字段，再追加自己的专属字段：

```cpp
QJsonObject Bus::toJson() const {
    QJsonObject obj = Vehicle::toJson();   // 基类通用字段
    obj["maxPassengers"] = m_maxPassengers; // 派生类专属字段
    return obj;
}
```

- **反序列化**：加载时使用 switch-case 根据 `vehicleType` 字符串创建正确的派生类对象，再调用各自重写的 `fromJson()`。同样，派生类的 `fromJson()` 先调用基类版本恢复通用字段，再读取专属字段。

#### 难点三：QStackedWidget 动态切换类型专属面板

**问题描述**：添加/编辑对话框中，根据车辆类型（客车/轿车/卡车）需要展示不同的输入字段。如果放置所有字段并控制显隐，代码会非常冗长且难以维护。

**解决方案**：使用 `QStackedWidget` 创建三个独立的页面，通过 `QComboBox` 的 `currentIndexChanged` 信号联动切换：

```cpp
// 类型切换时同步页面和维护费默认值
void AddVehicleDialog::onTypeChanged(int index) {
    m_stacked->setCurrentIndex(index);  // 切换专属字段页面
    switch (index) {
    case 0: m_maintenanceSpin->setValue(2000.0); break; // 客车
    case 1: m_maintenanceSpin->setValue(1000.0); break; // 轿车
    case 2: m_maintenanceSpin->setValue(1500.0); break; // 卡车
    }
    // 同步切换编号前缀...
}
```

每个页面只包含对应类型的专属控件，结构清晰，也方便未来扩展新车型。

#### 难点四：批量删除时的索引安全

**问题描述**：用户在表格中多选后执行批量删除。如果边遍历选中行边删除，行索引会随着删除操作动态变化，导致删错对象或越界崩溃。

**解决方案**：**先收集 ID，后批量删除**。删除操作前遍历选中行提取所有目标 ID 存入 `QStringList`，再统一遍历 ID 列表进行删除。由于 `VehicleManager::removeVehicle()` 是按编号（而非索引）删除的，不会受到表格行索引变化的影响。

```cpp
// 1. 先收集所有选中行的 ID
QStringList ids;
for (const auto &index : selectedRows) {
    QString id = vehicleIdAtTableRow(index.row());
    if (!id.isEmpty()) ids.append(id);
}
// 2. 确认后批量删除
for (const auto &id : ids) {
    if (m_manager.removeVehicle(id)) ++deleted;
}
```

#### 难点五：关闭窗口时的未保存检测

**问题描述**：用户可能在修改数据后直接关闭窗口，如果没有提醒机制，会导致数据丢失。

**解决方案**：重写 `QMainWindow::closeEvent()`，使用 `m_unsavedChanges` 标记追踪修改状态。任何增删改操作（包括修改油价和容量上限）都会将该标记置为 `true`；成功保存后置为 `false`。关闭时若标记为 `true`，弹出三选一对话框：

```
┌──────────────────────────┐
│  未保存的修改              │
│  当前有未保存的修改，      │
│  是否保存后再退出？        │
│                           │
│  [保存] [不保存] [再想想]  │
└──────────────────────────┘
```

---

## 5 总结

### 5.1 结论与分析

本项目成功开发了一个功能完整的**车辆管理系统（VMS）**，系统具备以下核心能力：

1. **完整的车辆生命周期管理**：支持大客车、小轿车、卡车三类车辆的信息录入、修改、查询和删除，覆盖了车辆管理的主要业务场景。
2. **灵活的查询与排序**：提供按编号、车牌号、制造公司、类别的多维度查询，支持全表头升序/降序排列，满足不同使用场景的数据检索需求。
3. **直观的数据统计可视化**：通过饼图和柱状图展示车辆分布的占比与费用对比，使数据洞察更加直观。
4. **可靠的数据持久化**：基于 JSON 格式的文件存储，兼容新旧格式，通过关闭时未保存提醒机制防止数据丢失。

从技术角度看，本项目较好地践行了面向对象设计的核心原则：

- **封装**：每个类的内部数据通过 private/protected 隐藏，通过 public 接口访问，保证了数据安全性。
- **继承**：Bus/Car/Truck 继承自 Vehicle，复用了通用属性和方法，实现了代码重用。
- **多态**：通过基类指针和虚函数机制，上层代码（VehicleManager、MainWindow）无需区分具体车辆类型即可完成表格渲染、JSON 序列化、统计等操作。
- **抽象**：Vehicle 作为抽象基类定义了统一的接口规范，降低了模块间的耦合度。

系统仍存在可改进的方面：目前仅支持 JSON 文件存储，未来可扩展为 SQLite 数据库以支持更大数据量；可增加导出 Excel 报表功能；可引入日志系统记录操作历史等。

### 5.2 收获与体会

通过本次课程设计，我在以下几个方面有了显著的能力提升：

1. **面向对象分析与设计的实践能力**：从实际业务需求出发，抽象出 Vehicle 基类→派生类的继承体系，并合理设计 VehicleManager 作为业务逻辑中枢。这个过程让我深刻体会到"高内聚、低耦合"的设计价值。

2. **Qt 框架的系统性掌握**：从最初只会使用 Qt Designer 拖拽控件，到能够纯代码搭建完整的 UI 布局、处理信号与槽的连接、管理界面生命周期，对 Qt 的信号-槽机制有了更深的理解。

3. **C++17 现代特性的应用**：在实践中大量使用了 `auto` 类型推导、Lambda 表达式、`dynamic_cast` 运行时类型识别、范围 for 循环、强类型枚举（`enum class`）等现代 C++ 特性，感受到了它们对代码简洁性和安全性的提升。

4. **问题驱动的学习方式**：开发过程中遇到了多个技术难点——多态序列化、文本框动态切换、批量删除索引安全等。通过查阅 Qt 官方文档、调试分析和反复试验，逐一找到了解决方案，这个过程极大地锻炼了独立解决问题的能力。

5. **工程化思维的形成**：体会到了软件工程中"设计先行"的重要性——在编码前花时间做好 UML 类图设计，大大减少了后期的返工量。同时也认识到了版本控制（Git）在项目管理中的重要性。

---

## 6 参考文献

[1] Bjarne Stroustrup. C++程序设计语言（第1-3部分）[M]. 机械工业出版社, 2016.

[2] Qt Company. Qt 6.10 Documentation[EB/OL]. https://doc.qt.io/qt-6/index.html.

[3] Stanley B. Lippman, Josée Lajoie, Barbara E. Moo. C++ Primer（第5版）[M]. 电子工业出版社, 2013.

[4] 谭浩强. C++程序设计（第4版）[M]. 清华大学出版社, 2021.

---
