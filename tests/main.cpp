// VMS 测试入口 — 运行全部注册的测试类
#include <QtTest>
#include "tst_vehicle.cpp"
#include "tst_vehiclemanager.cpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // 某些测试需要 QApplication

    int status = 0;

    // 依次运行各测试套件，任一失败则 status 非 0
    status |= QTest::qExec(new TestVehicle(), argc, argv);
    status |= QTest::qExec(new TestVehicleManager(), argc, argv);

    return status;
}
