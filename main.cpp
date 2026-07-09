#include <QApplication>
#include <QFile>
#include <QString>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("VehicleManager"));
    app.setApplicationVersion(QStringLiteral("1.0"));
    app.setOrganizationName(QStringLiteral("SWE25"));

    // 加载全局样式表
    // 优先读取 exe 同目录下的 style.qss（开发时热更新，只需重启应用）
    // 找不到则使用内嵌的默认样式
    QFile styleFile(QCoreApplication::applicationDirPath() + QStringLiteral("/style.qss"));
    if (!styleFile.exists())
        styleFile.setFileName(QStringLiteral(":/style.qss"));  // 内嵌资源

    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        app.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
        styleFile.close();
    }

    MainWindow w;
    w.show();

    return app.exec();
}
