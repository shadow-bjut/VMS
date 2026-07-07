#include "statisticsdialog.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QFrame>

StatisticsDialog::StatisticsDialog(const VehicleManager &manager, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("统计信息"));
    setMinimumWidth(380);
    Statistics s = manager.getStatistics();
    setupUI(s);
}

void StatisticsDialog::setupUI(const Statistics &s) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 统计数字
    QGroupBox *numGroup = new QGroupBox(QStringLiteral("车辆数量统计"));
    QFormLayout *form = new QFormLayout(numGroup);

    auto addRow = [&](const QString &label, int value) {
        QLabel *valLabel = new QLabel(QString::number(value) + QStringLiteral(" 辆"));
        valLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
        form->addRow(label, valLabel);
    };

    addRow(QStringLiteral("总车辆数："), s.total);
    addRow(QStringLiteral("大客车数："), s.busCount);
    addRow(QStringLiteral("小轿车数："), s.carCount);
    addRow(QStringLiteral("卡车数："),   s.truckCount);

    mainLayout->addWidget(numGroup);

    // 占比文本
    QGroupBox *ratioGroup = new QGroupBox(QStringLiteral("类别占比"));
    QVBoxLayout *ratioLayout = new QVBoxLayout(ratioGroup);

    if (s.total > 0) {
        auto addRatio = [&](const QString &name, int count) {
            double pct = 100.0 * count / s.total;
            QString text = QStringLiteral("%1：%2 辆  (%3%)")
                               .arg(name)
                               .arg(count)
                               .arg(pct, 0, 'f', 1);
            QLabel *lbl = new QLabel(text);
            lbl->setStyleSheet("font-size: 13px; margin: 4px 0;");
            ratioLayout->addWidget(lbl);

            // 简易进度条
            QFrame *bar = new QFrame();
            bar->setFixedHeight(16);
            bar->setStyleSheet(
                QStringLiteral("QFrame { background: #e0e0e0; border-radius: 8px; }"));
            QLabel *fill = new QLabel(bar);
            fill->setFixedHeight(16);
            fill->setFixedWidth(static_cast<int>(pct * 3.0)); // 最大 300px
            QString color = (name == QStringLiteral("大客车")) ? "#4CAF50" :
                                (name == QStringLiteral("小轿车")) ? "#2196F3" : "#FF9800";
            fill->setStyleSheet(
                QStringLiteral("background: %1; border-radius: 8px;").arg(color));
            ratioLayout->addWidget(bar);
        };
        addRatio(QStringLiteral("大客车"), s.busCount);
        addRatio(QStringLiteral("小轿车"), s.carCount);
        addRatio(QStringLiteral("卡车"),   s.truckCount);
    } else {
        ratioLayout->addWidget(new QLabel(QStringLiteral("暂无车辆数据")));
    }

    mainLayout->addWidget(ratioGroup);

    // 确定按钮
    QDialogButtonBox *btn = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(btn, &QDialogButtonBox::accepted, this, &QDialog::accept);
    mainLayout->addWidget(btn);
}
