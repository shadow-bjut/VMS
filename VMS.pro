QT += widgets charts

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addvehicledialog.cpp \
    bus.cpp \
    car.cpp \
    editvehicledialog.cpp \
    main.cpp \
    mainwindow.cpp \
    statisticsdialog.cpp \
    truck.cpp \
    vehicle.cpp \
    vehiclemanager.cpp

HEADERS += \
    addvehicledialog.h \
    bus.h \
    car.h \
    editvehicledialog.h \
    mainwindow.h \
    statisticsdialog.h \
    truck.h \
    vehicle.h \
    vehiclemanager.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
