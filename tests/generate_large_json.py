#!/usr/bin/env python3
"""生成 500+ 条 VMS 测试用 JSON 文件"""

import json
import random
import datetime

# ---- 基础数据池 ----
MANUFACTURERS = [
    "一汽大众", "上汽通用", "比亚迪汽车", "吉利汽车", "长城汽车",
    "奇瑞汽车", "长安汽车", "东风日产", "广汽丰田", "北京现代",
    "宇通客车", "金龙客车", "中通客车", "福田汽车", "江淮汽车",
    "华晨宝马", "北京奔驰", "一汽奥迪", "沃尔沃亚太", "特斯拉中国",
]

BUS_MFG = ["宇通客车", "金龙客车", "中通客车", "福田汽车", "安凯客车"]

PLATE_PREFIXES = [
    "京A", "京B", "京C", "沪A", "沪B", "粤A", "粤B", "粤C",
    "苏A", "苏B", "浙A", "浙B", "鲁A", "鲁B", "豫A", "豫B",
    "川A", "川B", "鄂A", "鄂B", "湘A", "湘B", "闽A", "闽B",
]

CAR_MODELS_2BOX = 2   # 两厢
CAR_MODELS_3BOX = 3   # 三厢


def random_date():
    y = random.randint(2015, 2026)
    m = random.randint(1, 12)
    d = random.randint(1, 28)
    return f"{y:04d}-{m:02d}-{d:02d}"


def random_plate():
    prefix = random.choice(PLATE_PREFIXES)
    digits = "".join([str(random.randint(0, 9)) for _ in range(5)])
    return f"{prefix}{digits}"


def make_bus(idx):
    return {
        "id": f"BUS-{idx:04d}",
        "plateNumber": random_plate(),
        "manufacturer": random.choice(BUS_MFG),
        "purchaseDate": random_date(),
        "vehicleType": "大客车",
        "totalKm": round(random.uniform(5000, 300000), 1),
        "fuelConsumption": round(random.uniform(0.15, 0.40), 4),
        "basicMaintenance": 2000.0,
        "roadFee": round(random.uniform(300, 800), 2),
        "accumulatedCost": round(random.uniform(10000, 200000), 2),
        "maxPassengers": random.choice([30, 35, 40, 45, 50, 55, 60]),
    }


def make_car(idx):
    return {
        "id": f"CAR-{idx:04d}",
        "plateNumber": random_plate(),
        "manufacturer": random.choice(MANUFACTURERS),
        "purchaseDate": random_date(),
        "vehicleType": "小轿车",
        "totalKm": round(random.uniform(1000, 200000), 1),
        "fuelConsumption": round(random.uniform(0.04, 0.15), 4),
        "basicMaintenance": 1000.0,
        "roadFee": round(random.uniform(100, 300), 2),
        "accumulatedCost": round(random.uniform(5000, 100000), 2),
        "trunkType": random.choice([2, 3]),
    }


def make_truck(idx):
    return {
        "id": f"TRK-{idx:04d}",
        "plateNumber": random_plate(),
        "manufacturer": random.choice(MANUFACTURERS),
        "purchaseDate": random_date(),
        "vehicleType": "卡车",
        "totalKm": round(random.uniform(10000, 500000), 1),
        "fuelConsumption": round(random.uniform(0.20, 0.55), 4),
        "basicMaintenance": 1500.0,
        "roadFee": round(random.uniform(400, 1200), 2),
        "accumulatedCost": round(random.uniform(20000, 300000), 2),
        "maxLoad": round(random.uniform(2, 40), 1),
    }


def main():
    vehicles = []

    # 生成 200 辆轿车
    for i in range(1, 201):
        vehicles.append(make_car(i))

    # 生成 180 辆卡车
    for i in range(1, 181):
        vehicles.append(make_truck(i))

    # 生成 160 辆客车
    for i in range(1, 161):
        vehicles.append(make_bus(i))

    # 总计 540 辆
    random.shuffle(vehicles)

    data = {
        "fuelPrice": 7.5,
        "maxCapacity": 1000,
        "vehicles": vehicles,
    }

    output_path = "test_540_vehicles.json"
    with open(output_path, "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=2)

    bus_count = sum(1 for v in vehicles if v['vehicleType'] == '大客车')
    car_count = sum(1 for v in vehicles if v['vehicleType'] == '小轿车')
    truck_count = sum(1 for v in vehicles if v['vehicleType'] == '卡车')
    file_size_kb = len(json.dumps(data, ensure_ascii=False, indent=2)) / 1024

    print(f"[OK] Generated {len(vehicles)} vehicles -> {output_path}")
    print(f"     Bus: {bus_count}, Car: {car_count}, Truck: {truck_count}")
    print(f"     File size: {file_size_kb:.1f} KB")


if __name__ == "__main__":
    main()
