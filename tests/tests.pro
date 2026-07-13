# VMS 白盒测试项目文件
# 构建: qmake tests.pro && make
# 运行: ./tst_vms          (全部测试)
#       ./tst_vms -vs      (详细输出每个用例)
#       ./tst_vms "tst_vehiclemanager"  (只跑指定类)

QT += widgets charts testlib
CONFIG += c++17 testcase
TARGET = tst_vms

# 绕过 Qt 6.11.1 在 MinGW GCC 上的 noexcept static_assert 兼容性问题
# 原始条件: #if defined(__cpp_lib_three_way_comparison) || !(defined(Q_OS_QNX) || defined(Q_CC_GHS))
# MinGW GCC 触发了断言: "Use *_NON_NOEXCEPT version of the macro"
DEFINES += Q_OS_QNX

# 强制使用 Qt 6.11.1 配套的 GCC 13.1.0 (而非系统默认的老版本 GCC 8.1.0)
QMAKE_CXX = C:/Qt/Tools/mingw1310_64/bin/g++
QMAKE_LINK = C:/Qt/Tools/mingw1310_64/bin/g++
QMAKE_CC = C:/Qt/Tools/mingw1310_64/bin/gcc

# ---------- 被测源文件 ----------
SOURCES += \
    ../vehicle.cpp \
    ../bus.cpp \
    ../car.cpp \
    ../truck.cpp \
    ../vehiclemanager.cpp

HEADERS += \
    ../vehicle.h \
    ../bus.h \
    ../car.h \
    ../truck.h \
    ../vehiclemanager.h

# ---------- 测试文件 ----------
SOURCES += \
    tst_vehicle.cpp \
    tst_vehiclemanager.cpp \
    main.cpp
