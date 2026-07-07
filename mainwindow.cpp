#include "mainwindow.h"
#include "addvehicledialog.h"
#include "editvehicledialog.h"
#include "statisticsdialog.h"
 #include <QInputDialog>
#include <QApplication>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QAction>
#include <QMenu>
#include <QFrame>
#include <QShortcut>
#include <algorithm>
// ---------- 构造 / 析构 ----------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("车辆管理系统"));
    resize(1100, 650);
    setupUI();
    updateStatusBar();
}

MainWindow::~MainWindow() = default;

// ---------- UI 搭建 ----------

void MainWindow::setupUI() {
    setupMenuBar();
    setupToolBar();
    setupCentralWidget();
    setupStatusBar();
}

// ---- 菜单栏 ----

void MainWindow::setupMenuBar() {
    // 文件菜单
    QMenu *fileMenu = menuBar()->addMenu(QStringLiteral("文件(&F)"));
    fileMenu->addAction(QStringLiteral("打开(&O)..."), QKeySequence::Open,
                        this, &MainWindow::onLoadFromFile);
    fileMenu->addAction(QStringLiteral("保存(&S)..."), QKeySequence::Save,
                        this, &MainWindow::onSaveToFile);
    fileMenu->addSeparator();
    fileMenu->addAction(QStringLiteral("退出(&X)"), QKeySequence(Qt::ALT | Qt::Key_F4),
                        this, &QWidget::close);

    // 车辆菜单
    QMenu *vehicleMenu = menuBar()->addMenu(QStringLiteral("操作(&V)"));
    vehicleMenu->addAction(QStringLiteral("添加车辆(&A)..."), this, &MainWindow::onAddVehicle);
    vehicleMenu->addAction(QStringLiteral("编辑车辆(&E)..."), this, &MainWindow::onEditVehicle);
    vehicleMenu->addAction(QStringLiteral("删除车辆(&D)..."), this, &MainWindow::onDeleteVehicle);
    vehicleMenu->addAction(QStringLiteral("批量删除(&B)..."), QKeySequence::Delete,
                            this, &MainWindow::onBatchDelete);
    vehicleMenu->addAction(QStringLiteral("更新油价(&F)..."), this, &MainWindow::onSetFuelPrice);
    vehicleMenu->addSeparator();
    vehicleMenu->addAction(QStringLiteral("显示全部(&R)"), this, &MainWindow::onShowAll);

    // 统计菜单
    QMenu *statsMenu = menuBar()->addMenu(QStringLiteral("统计(&T)"));
    statsMenu->addAction(QStringLiteral("统计信息(&I)..."), this, &MainWindow::onStatistics);

    // 帮助
    QMenu *helpMenu = menuBar()->addMenu(QStringLiteral("帮助(&H)"));
    helpMenu->addAction(QStringLiteral("关于(&A)..."), this, &MainWindow::onAbout);
    helpMenu->addAction(QStringLiteral("版本号(&B)..."), this, &MainWindow::onVersion);
}

// ---- 工具栏 ----

void MainWindow::setupToolBar() {
    QToolBar *toolbar = addToolBar(QStringLiteral("工具栏"));
    toolbar->setMovable(false);

    toolbar->addAction(QStringLiteral("添加"), this, &MainWindow::onAddVehicle);
    toolbar->addAction(QStringLiteral("编辑"), this, &MainWindow::onEditVehicle);
    toolbar->addAction(QStringLiteral("删除"), this, &MainWindow::onDeleteVehicle);
    toolbar->addAction(QStringLiteral("批量删除"), this, &MainWindow::onBatchDelete);
    toolbar->addSeparator();
    toolbar->addAction(QStringLiteral("打开"), this, &MainWindow::onLoadFromFile);
    toolbar->addAction(QStringLiteral("保存"), this, &MainWindow::onSaveToFile);
    toolbar->addSeparator();
    toolbar->addAction(QStringLiteral("统计"), this, &MainWindow::onStatistics);
}

// ---- 中心区域 ----

void MainWindow::setupCentralWidget() {
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // ---------- 查询面板 ----------
    QGroupBox *searchGroupBox = new QGroupBox(QStringLiteral("查询车辆"), central);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchGroupBox);

    searchLayout->addWidget(new QLabel(QStringLiteral("搜索关键字：")));

    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText(QStringLiteral("请输入查询内容..."));
    m_searchEdit->setMinimumWidth(200);
    searchLayout->addWidget(m_searchEdit);

    // 单选按钮组
    m_radioById  = new QRadioButton(QStringLiteral("按编号"));
    m_radioByMfg = new QRadioButton(QStringLiteral("按制造公司"));
    m_radioByType = new QRadioButton(QStringLiteral("按类别"));
    m_radioById->setChecked(true);
    m_searchGroup = new QButtonGroup(this);
    m_searchGroup->addButton(m_radioById,  0);
    m_searchGroup->addButton(m_radioByMfg, 1);
    m_searchGroup->addButton(m_radioByType, 2);
    searchLayout->addWidget(m_radioById);
    searchLayout->addWidget(m_radioByMfg);
    searchLayout->addWidget(m_radioByType);

    m_searchBtn = new QPushButton(QStringLiteral("🔍 查询"));
    m_searchBtn->setDefault(true);
    connect(m_searchBtn, &QPushButton::clicked, this, &MainWindow::onSearch);
    searchLayout->addWidget(m_searchBtn);

    m_showAllBtn = new QPushButton(QStringLiteral("📋 显示全部"));
    connect(m_showAllBtn, &QPushButton::clicked, this, &MainWindow::onShowAll);
    searchLayout->addWidget(m_showAllBtn);

    searchLayout->addStretch();
    mainLayout->addWidget(searchGroupBox);

    // ---------- 排序按钮 ----------
    QHBoxLayout *sortLayout = new QHBoxLayout();
    sortLayout->addWidget(new QLabel(QStringLiteral("排序方式：")));

    m_sortByDateBtn = new QPushButton(QStringLiteral("📅 按购买时间 ↑"));
    connect(m_sortByDateBtn, &QPushButton::clicked,
            this, &MainWindow::onSortByPurchaseDate);
    sortLayout->addWidget(m_sortByDateBtn);

    m_sortByKmBtn = new QPushButton(QStringLiteral("🛣️ 按总里程 ↑"));
    connect(m_sortByKmBtn, &QPushButton::clicked,
            this, &MainWindow::onSortByMileage);
    sortLayout->addWidget(m_sortByKmBtn);

    m_sortByFCBtn = new QPushButton(QStringLiteral("⛽ 按耗油量 ↑"));
    connect(m_sortByFCBtn, &QPushButton::clicked,
            this, &MainWindow::onSortByFC);
    sortLayout->addWidget(m_sortByFCBtn);

    sortLayout->addStretch();

    // 恢复默认排序
    m_resetSortBtn = new QPushButton(QStringLiteral("🔄 恢复默认"));
    connect(m_resetSortBtn, &QPushButton::clicked,
            this, &MainWindow::onResetSort);
    sortLayout->addWidget(m_resetSortBtn);

    mainLayout->addLayout(sortLayout);

    // 按回车触发查询
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearch);

    // ---------- 表格 ----------
    m_table = new QTableWidget(0, 12, central); // 12 列（可根据类型微调）
    QStringList headers = {
        QStringLiteral("编号"),
        QStringLiteral("车牌号"),
        QStringLiteral("制造公司"),
        QStringLiteral("购买时间"),
        QStringLiteral("类别"),
        QStringLiteral("总公里数"),
        QStringLiteral("耗油量/公里"),
        QStringLiteral("维护费(元/月)"),
        QStringLiteral("养路费"),
        QStringLiteral("累计总费用"),
        QStringLiteral("当月总费用"),
        QStringLiteral("特殊属性")
    };
    m_table->setHorizontalHeaderLabels(headers);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);

    // 双击行编辑
    connect(m_table, &QTableWidget::cellDoubleClicked, this, [this](int, int) {
        onEditVehicle();
    });

    // Delete 键触发批量删除
    QShortcut *deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), m_table);
    connect(deleteShortcut, &QShortcut::activated, this, &MainWindow::onBatchDelete);

    mainLayout->addWidget(m_table, 1); // stretch=1 占据剩余空间

    setCentralWidget(central);
}

// ---- 状态栏 ----

void MainWindow::setupStatusBar() {
    m_statusTotal = new QLabel();
    m_statusBus   = new QLabel();
    m_statusCar   = new QLabel();
    m_statusTruck = new QLabel();

    statusBar()->addPermanentWidget(m_statusTotal);
    statusBar()->addPermanentWidget(m_statusBus);
    statusBar()->addPermanentWidget(m_statusCar);
    statusBar()->addPermanentWidget(m_statusTruck);

    updateStatusBar();
}

void MainWindow::updateStatusBar() {
    Statistics s = m_manager.getStatistics();
    m_statusTotal->setText(QStringLiteral("  总计: %1 辆  ").arg(s.total));
    m_statusBus->setText(QStringLiteral("客车: %1 辆  ").arg(s.busCount));
    m_statusCar->setText(QStringLiteral("轿车: %1 辆  ").arg(s.carCount));
    m_statusTruck->setText(QStringLiteral("卡车: %1 辆  ").arg(s.truckCount));
}

// ---------- 表格刷新 ----------

QString MainWindow::vehicleIdAtTableRow(int row) const {
    if (row < 0 || row >= m_displayedVehicles.size())
        return {};
    return m_displayedVehicles[row]->id();
}

int MainWindow::currentSelectedRow() const {
    auto selected = m_table->selectionModel()->selectedRows();
    if (selected.isEmpty()) return -1;
    return selected.first().row();
}

void MainWindow::refreshTable() {
    refreshTable(m_manager.allVehicles());
}

void MainWindow::refreshTable(const QVector<Vehicle *> &vehicles) {
    m_displayedVehicles = vehicles;
    m_table->setRowCount(vehicles.size());

    for (int row = 0; row < vehicles.size(); ++row) {
        Vehicle *v = vehicles[row];

        auto setCell = [&](int col, const QString &text) {
            m_table->setItem(row, col, new QTableWidgetItem(text));
        };

        setCell(0, v->id());
        setCell(1, v->plateNumber());
        setCell(2, v->manufacturer());
        setCell(3, v->purchaseDate().toString("yyyy-MM-dd"));
        setCell(4, v->typeName());
        setCell(5, QString::number(v->totalKm(), 'f', 1));
        setCell(6, QString::number(v->fuelConsumption(), 'f', 4));
        setCell(7, QString::number(v->basicMaintenance(), 'f', 2));
        setCell(8, QString::number(v->roadFee(), 'f', 2));
        setCell(9, QString::number(v->accumulatedCost(), 'f', 2));
        // 当月总费用
        double monthly = v->monthlyTotalCost(m_manager.fuelPrice());
        setCell(10, QString::number(monthly, 'f', 2));

        // 特殊属性
        QString special;
        if (auto *bus = dynamic_cast<Bus *>(v)) {
            special = QStringLiteral("载客量: %1 人").arg(bus->maxPassengers());
        } else if (auto *car = dynamic_cast<Car *>(v)) {
            special = car->trunkTypeName();
        } else if (auto *truck = dynamic_cast<Truck *>(v)) {
            special = QStringLiteral("载重量: %1 吨").arg(truck->maxLoad());
        }
        setCell(11, special);
    }
    updateStatusBar();
}

// ---------- 槽：添加车辆 ----------

void MainWindow::onAddVehicle() {
    // 收集已有编号，供对话框自动生成默认编号
    QStringList existingIds;
    for (auto *v : m_manager.allVehicles())
        existingIds.append(v->id());
    AddVehicleDialog dlg(existingIds, this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    Vehicle *v = dlg.createVehicle();
    if (!v) return;

    QString errorMsg;
    if (m_manager.addVehicle(v, errorMsg)) {
        refreshTable();
        QMessageBox::information(this, QStringLiteral("成功"),
                                 QStringLiteral("车辆 %1 添加成功！").arg(v->id()));
    } else {
        delete v;
        QMessageBox::warning(this, QStringLiteral("添加失败"), errorMsg);
    }
}

// ---------- 槽：编辑车辆 ----------

void MainWindow::onEditVehicle() {
    int row = currentSelectedRow();
    if (row < 0) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("请先在表格中选择一条记录！"));
        return;
    }
    QString id = vehicleIdAtTableRow(row);
    Vehicle *v = m_manager.findById(id);
    if (!v) return;

    EditVehicleDialog dlg(v, this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    Vehicle *newData = dlg.createVehicle();
    if (!newData) return;

    QString errorMsg;
    if (m_manager.updateVehicle(id, newData, errorMsg)) {
        refreshTable();
        QMessageBox::information(this, QStringLiteral("成功"),
                                 QStringLiteral("车辆信息修改成功！"));
    } else {
        QMessageBox::warning(this, QStringLiteral("修改失败"), errorMsg);
    }
    delete newData; // updateVehicle 内部已经拷贝数据
}

// ---------- 槽：删除车辆 ----------

void MainWindow::onDeleteVehicle() {
    if (m_manager.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("车辆信息库为空！"));
        return;
    }
    int row = currentSelectedRow();
    if (row < 0) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("请先在表格中选择一条记录！"));
        return;
    }
    QString id = vehicleIdAtTableRow(row);

    QMessageBox::StandardButton btn = QMessageBox::question(
        this, QStringLiteral("确认删除"),
        QStringLiteral("确定要删除编号为 %1 的车辆吗？").arg(id),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (btn != QMessageBox::Yes) return;

    if (m_manager.removeVehicle(id)) {
        refreshTable();
        QMessageBox::information(this, QStringLiteral("成功"),
                                 QStringLiteral("车辆 %1 已删除！").arg(id));
    } else {
        QMessageBox::warning(this, QStringLiteral("错误"),
                             QStringLiteral("该编号不存在！"));
    }
}

// ---------- 槽：批量删除 ----------

void MainWindow::onBatchDelete() {
    auto selectedRows = m_table->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("请先在表格中勾选要删除的记录！\n"
                                                "提示：可按住 Ctrl 或 Shift 多选。"));
        return;
    }

    int count = selectedRows.size();

    // 收集所有选中行的 ID（必须在删除前收集，否则行索引会变化）
    QStringList ids;
    for (const auto &index : selectedRows) {
        QString id = vehicleIdAtTableRow(index.row());
        if (!id.isEmpty())
            ids.append(id);
    }

    // 确认对话框
    QString msg = QStringLiteral("确定要删除以下 %1 条车辆记录吗？\n\n").arg(count);
    for (int i = 0; i < qMin(ids.size(), 10); ++i)
        msg += QStringLiteral("  • %1\n").arg(ids[i]);
    if (ids.size() > 10)
        msg += QStringLiteral("  …以及其他 %1 条\n").arg(ids.size() - 10);
    msg += QStringLiteral("\n此操作不可撤销！");

    auto btn = QMessageBox::question(this, QStringLiteral("确认批量删除"), msg,
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);
    if (btn != QMessageBox::Yes)
        return;

    // 执行删除
    int deleted = 0;
    for (const auto &id : ids) {
        if (m_manager.removeVehicle(id))
            ++deleted;
    }

    refreshTable();
    QMessageBox::information(this, QStringLiteral("完成"),
                             QStringLiteral("成功删除 %1 条记录！").arg(deleted));
}

// ---------- 槽：查询 ----------

void MainWindow::onSearch() {
    QString keyword = m_searchEdit->text().trimmed();
    if (keyword.isEmpty()) {
        refreshTable();
        return;
    }
    QVector<Vehicle *> result;
    int mode = m_searchGroup->checkedId(); // 0=ID, 1=Mfg, 2=Type

    switch (mode) {
    case 0: { // 按编号
        Vehicle *v = m_manager.findById(keyword);
        if (v) {
            result.append(v);
        } else {
            QMessageBox::information(this, QStringLiteral("查询结果"),
                                     QStringLiteral("该编号不存在！"));
            return;
        }
        break;
    }
    case 1: { // 按制造公司
        result = m_manager.findByManufacturer(keyword);
        if (result.isEmpty()) {
            QMessageBox::information(this, QStringLiteral("查询结果"),
                                     QStringLiteral("该车辆制造公司不存在！"));
            return;
        }
        break;
    }
    case 2: { // 按类别
        QString t = keyword;
        VehicleType vt;
        if (t.contains(QStringLiteral("客车")) || t.contains(QStringLiteral("巴士"))) {
            vt = VehicleType::Bus;
        } else if (t.contains(QStringLiteral("轿车")) || t.contains(QStringLiteral("小轿车"))) {
            vt = VehicleType::Car;
        } else if (t.contains(QStringLiteral("卡车")) || t.contains(QStringLiteral("货车"))) {
            vt = VehicleType::Truck;
        } else {
            QMessageBox::information(this, QStringLiteral("查询结果"),
                                     QStringLiteral("请输入正确类别：大客车 / 小轿车 / 卡车"));
            return;
        }
        result = m_manager.findByType(vt);
        if (result.isEmpty()) {
            QMessageBox::information(this, QStringLiteral("查询结果"),
                                     QStringLiteral("该类别没有车辆！"));
            return;
        }
        break;
    }
    }
    refreshTable(result);
}

// ---------- 槽：显示全部 ----------

void MainWindow::onShowAll() {
    m_searchEdit->clear();
    refreshTable();
}

// ---------- 槽：文件操作 ----------

void MainWindow::onSaveToFile() {
    if (m_manager.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("车辆信息库为空，无需保存！"));
        return;
    }
    QString filePath = QFileDialog::getSaveFileName(
        this, QStringLiteral("保存车辆信息"),
        QStringLiteral("vehicles.json"),
        QStringLiteral("JSON 文件 (*.json);;所有文件 (*)"));
    if (filePath.isEmpty()) return;

    QString errorMsg;
    if (m_manager.saveToFile(filePath, errorMsg)) {
        QMessageBox::information(this, QStringLiteral("成功"),
                                 QStringLiteral("车辆信息已保存至:\n%1").arg(filePath));
    } else {
        QMessageBox::warning(this, QStringLiteral("保存失败"), errorMsg);
    }
}

void MainWindow::onLoadFromFile() {
    QString filePath = QFileDialog::getOpenFileName(
        this, QStringLiteral("打开车辆信息文件"),
        QStringLiteral("vehicles.json"),
        QStringLiteral("JSON 文件 (*.json);;所有文件 (*)"));
    if (filePath.isEmpty()) return;

    QString errorMsg;
    if (m_manager.loadFromFile(filePath, errorMsg)) {
        refreshTable();
        QMessageBox::information(this, QStringLiteral("成功"),
                                 QStringLiteral("已从文件加载 %1 条车辆记录！")
                                     .arg(m_manager.count()));
    } else {
        QMessageBox::warning(this, QStringLiteral("加载失败"), errorMsg);
    }
}

// ---------- 统计 ----------

void MainWindow::onStatistics() {
    StatisticsDialog dlg(m_manager, this);
    dlg.exec();
}

// ---------- 关于 ----------

void MainWindow::onAbout() {
    QMessageBox::about(this, QStringLiteral("关于车辆管理系统"),
                       QStringLiteral("<h3>车辆管理系统</h3>"
                                      "<p>基于 Qt 的车辆信息管理程序</p>"
                                      "<p>仅支持大客车、小轿车、卡车三类车辆的管理</p>"));
}

//版本号
void MainWindow::onVersion() {
    QMessageBox::about(this, QStringLiteral("版本"),
                       QStringLiteral("<h3>v1.0</h3>"
                                      "<p>制作人：shadow</p>"
                                      "<p>更多详见：https://github.com/shadow-bjut/VMS</p>"
                                    ));
}

void MainWindow::onSetFuelPrice() {
    // 需要先 #include <QInputDialog>（放到文件顶部 #include 区域）
    bool ok = false;
    double newPrice = QInputDialog::getDouble(
        this,
        QStringLiteral("修改油价"),
        QStringLiteral("请输入当前油价（元/升）："),
        m_manager.fuelPrice(),   // 默认值：显示当前油价
        0.01,                    // 最小值
        999.99,                  // 最大值
        2,                       // 小数位数
        &ok
        );

    if (ok) {
        m_manager.setFuelPrice(newPrice);
        refreshTable();  // 刷新表格（因为当月总费用依赖油价）
        QMessageBox::information(this, QStringLiteral("成功"),
                                 QStringLiteral("油价已更新为 %1 元/升！").arg(newPrice));
    }
}

// ---------- 槽：按购买时间排序 ----------

void MainWindow::onSortByPurchaseDate() {
    if (m_displayedVehicles.isEmpty()) return;

    m_dateAscending = !m_dateAscending;  // 切换方向

    QVector<Vehicle *> sorted = m_displayedVehicles;
    if (m_dateAscending) {
        std::sort(sorted.begin(), sorted.end(), [](Vehicle *a, Vehicle *b) {
            return a->purchaseDate() < b->purchaseDate();  // 从早到晚
        });
        m_sortByDateBtn->setText(QStringLiteral("📅 按购买时间 ↑"));
    } else {
        std::sort(sorted.begin(), sorted.end(), [](Vehicle *a, Vehicle *b) {
            return a->purchaseDate() > b->purchaseDate();  // 从晚到早
        });
        m_sortByDateBtn->setText(QStringLiteral("📅 按购买时间 ↓"));
    }
    refreshTable(sorted);
}

// ---------- 槽：按总里程排序 ----------

void MainWindow::onSortByMileage() {
    if (m_displayedVehicles.isEmpty()) return;

    m_kmAscending = !m_kmAscending;  // 切换方向

    QVector<Vehicle *> sorted = m_displayedVehicles;
    if (m_kmAscending) {
        std::sort(sorted.begin(), sorted.end(), [](Vehicle *a, Vehicle *b) {
            return a->totalKm() < b->totalKm();  // 从小到大
        });
        m_sortByKmBtn->setText(QStringLiteral("🛣️ 按总里程 ↑"));
    } else {
        std::sort(sorted.begin(), sorted.end(), [](Vehicle *a, Vehicle *b) {
            return a->totalKm() > b->totalKm();  // 从大到小
        });
        m_sortByKmBtn->setText(QStringLiteral("🛣️ 按总里程 ↓"));
    }
    refreshTable(sorted);
}

// ---------- 槽：按耗油量排序 ----------

void MainWindow::onSortByFC() {
    if (m_displayedVehicles.isEmpty()) return;

    m_fcAscending = !m_fcAscending;  // 切换方向

    QVector<Vehicle *> sorted = m_displayedVehicles;
    if (m_fcAscending) {
        std::sort(sorted.begin(), sorted.end(), [](Vehicle *a, Vehicle *b) {
            return a->fuelConsumption() < b->fuelConsumption();  // 从小到大
        });
        m_sortByFCBtn->setText(QStringLiteral("⛽ 按耗油量 ↑"));
    } else {
        std::sort(sorted.begin(), sorted.end(), [](Vehicle *a, Vehicle *b) {
            return a->fuelConsumption() > b->fuelConsumption();  // 从大到小
        });
        m_sortByFCBtn->setText(QStringLiteral("⛽ 按耗油量 ↓"));
    }
    refreshTable(sorted);
}

// ---------- 槽：恢复默认排序 ----------

void MainWindow::onResetSort() {
    // 恢复 manager 内部原始顺序
    refreshTable();

    // 重置方向标记
    m_dateAscending = true;
    m_kmAscending   = true;
    m_fcAscending   = true;

    // 恢复按钮文字
    m_sortByDateBtn->setText(QStringLiteral("📅 按购买时间 ↑"));
    m_sortByKmBtn->setText(QStringLiteral("🛣️ 按总里程 ↑"));
    m_sortByFCBtn->setText(QStringLiteral("⛽ 按耗油量 ↑"));
}



