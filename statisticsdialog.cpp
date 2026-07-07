#include "statisticsdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFont>
#include <QPainter>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

StatisticsDialog::StatisticsDialog(const VehicleManager &manager, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("统计信息"));
    setMinimumWidth(620);

    Statistics s = manager.getStatistics();

    // 计算各类型平均月费用
    double avgBusCost = 0, avgCarCost = 0, avgTruckCost = 0;
    const auto &vehicles = manager.allVehicles();
    double fuelPrice = manager.fuelPrice();

    for (auto *v : vehicles) {
        double cost = v->monthlyTotalCost(fuelPrice);
        switch (v->vehicleType()) {
        case VehicleType::Bus:   avgBusCost   += cost; break;
        case VehicleType::Car:   avgCarCost   += cost; break;
        case VehicleType::Truck: avgTruckCost += cost; break;
        }
    }
    if (s.busCount   > 0) avgBusCost   /= s.busCount;
    if (s.carCount   > 0) avgCarCost   /= s.carCount;
    if (s.truckCount > 0) avgTruckCost /= s.truckCount;

    setupUI(s, avgBusCost, avgCarCost, avgTruckCost);
}

void StatisticsDialog::setupUI(const Statistics &s,
                                double avgBusCost, double avgCarCost, double avgTruckCost) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);

    // ===== 区域 1：计数摘要 =====
    QGroupBox *numGroup = new QGroupBox(QStringLiteral("车辆数量统计"));
    QHBoxLayout *numLayout = new QHBoxLayout(numGroup);

    auto addCountLabel = [&](const QString &text, int value, const QString &color) {
        QString html = QStringLiteral("<b>%1</b><br><span style='color:%2; font-size:24px;'>%3</span> 辆")
                           .arg(text, color).arg(value);
        QLabel *lbl = new QLabel(html);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setMinimumWidth(120);
        numLayout->addWidget(lbl);
    };

    addCountLabel(QStringLiteral("总计"),   s.total,      QStringLiteral("#333"));
    addCountLabel(QStringLiteral("大客车"), s.busCount,   QStringLiteral("#4CAF50"));
    addCountLabel(QStringLiteral("小轿车"), s.carCount,   QStringLiteral("#2196F3"));
    addCountLabel(QStringLiteral("卡车"),   s.truckCount, QStringLiteral("#FF9800"));

    mainLayout->addWidget(numGroup);

    // ===== 区域 2：饼图（甜甜圈风格） =====
    if (s.total > 0) {
        QGroupBox *pieGroup = new QGroupBox(QStringLiteral("类别占比"));
        QVBoxLayout *pieLayout = new QVBoxLayout(pieGroup);

        QPieSeries *pieSeries = new QPieSeries();
        pieSeries->setHoleSize(0.40);

        auto addSlice = [&](const QString &label, int count, const QColor &color) {
            if (count > 0) {
                QPieSlice *slice = pieSeries->append(label, count);
                slice->setBrush(color);
                slice->setLabelVisible(true);
                slice->setLabelBrush(QBrush(Qt::white));
            }
        };

        addSlice(QStringLiteral("大客车"), s.busCount,   QColor("#4CAF50"));
        addSlice(QStringLiteral("小轿车"), s.carCount,   QColor("#2196F3"));
        addSlice(QStringLiteral("卡车"),   s.truckCount, QColor("#FF9800"));

        QChart *pieChart = new QChart();
        pieChart->addSeries(pieSeries);
        pieChart->setTitle(QStringLiteral("车辆类别分布"));
        pieChart->setTitleFont(QFont(QStringLiteral("Microsoft YaHei"), 12, QFont::Bold));
        pieChart->legend()->setAlignment(Qt::AlignBottom);
        pieChart->legend()->setFont(QFont(QStringLiteral("Microsoft YaHei"), 10));
        pieChart->setAnimationOptions(QChart::SeriesAnimations);

        QChartView *pieView = new QChartView(pieChart);
        pieView->setRenderHint(QPainter::Antialiasing);
        pieView->setMinimumHeight(280);

        pieLayout->addWidget(pieView);
        mainLayout->addWidget(pieGroup);
    }

    // ===== 区域 3：柱状图（数量 + 平均月费用） =====
    if (s.total > 0) {
        QGroupBox *barGroup = new QGroupBox(QStringLiteral("数据对比"));
        QVBoxLayout *barLayout = new QVBoxLayout(barGroup);

        // 柱状图
        QBarSet *setCount = new QBarSet(QStringLiteral("车辆数量"));
        setCount->setBrush(QColor("#42A5F5"));   // 蓝色填充
        setCount->setPen(QPen(QColor("#1E88E5"), 2));  // 深蓝边框
        setCount->setLabelBrush(QBrush(QColor("#333")));  // 深色标签
        *setCount << s.busCount << s.carCount << s.truckCount;

        QBarSet *setCost = new QBarSet(QStringLiteral("平均月费用"));
        setCost->setBrush(QColor("#FF7043"));   // 橙色填充
        setCost->setPen(QPen(QColor("#E64A19"), 2));  // 深橙边框
        setCost->setLabelBrush(QBrush(QColor("#333")));  // 深色标签
        *setCost << avgBusCost << avgCarCost << avgTruckCost;

        QBarSeries *barSeries = new QBarSeries();
        barSeries->append(setCount);
        barSeries->append(setCost);
        barSeries->setLabelsVisible(true);
        barSeries->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
        barSeries->setLabelsFormat(QStringLiteral("@value"));

        QChart *barChart = new QChart();
        barChart->addSeries(barSeries);
        barChart->setTitle(QStringLiteral("各类型数量与平均月费用对比"));
        barChart->setTitleFont(QFont(QStringLiteral("Microsoft YaHei"), 12, QFont::Bold));
        barChart->setAnimationOptions(QChart::SeriesAnimations);

        QStringList categories;
        categories << QStringLiteral("大客车") << QStringLiteral("小轿车") << QStringLiteral("卡车");
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        axisX->setLabelsFont(QFont(QStringLiteral("Microsoft YaHei"), 10));
        barChart->addAxis(axisX, Qt::AlignBottom);
        barSeries->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText(QStringLiteral("数量 / 元"));
        axisY->setLabelFormat(QStringLiteral("%.0f"));
        barChart->addAxis(axisY, Qt::AlignLeft);
        barSeries->attachAxis(axisY);

        barChart->legend()->setAlignment(Qt::AlignBottom);
        barChart->legend()->setFont(QFont(QStringLiteral("Microsoft YaHei"), 10));

        QChartView *barView = new QChartView(barChart);
        barView->setRenderHint(QPainter::Antialiasing);
        barView->setMinimumHeight(280);

        barLayout->addWidget(barView);
        mainLayout->addWidget(barGroup);
    }

    // 无数据提示
    if (s.total == 0) {
        QLabel *emptyLabel = new QLabel(QStringLiteral("暂无车辆数据，请先添加车辆。"));
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("color: #999; font-size: 16px; padding: 40px;");
        mainLayout->addWidget(emptyLabel);
    }

    // ===== 确定按钮 =====
    QDialogButtonBox *btn = new QDialogButtonBox(QDialogButtonBox::Ok);
    btn->button(QDialogButtonBox::Ok)->setText(QStringLiteral("确定"));
    connect(btn, &QDialogButtonBox::accepted, this, &QDialog::accept);
    mainLayout->addWidget(btn);
}
