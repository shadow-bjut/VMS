#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QStatusBar>
#include <QMenuBar>

#include "vehiclemanager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddVehicle();
    void onEditVehicle();
    void onDeleteVehicle();
    void onBatchDelete();
    void onSearch();
    void onShowAll();
    void onSaveToFile();
    void onLoadFromFile();
    void onStatistics();
    void onAbout();
    void onVersion();
    void onSetFuelPrice();
    void onSortByPurchaseDate();
    void onSortByMileage();
    void onSortByFC();
    void onResetSort();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupCentralWidget();
    void setupStatusBar();

    void refreshTable(const QVector<Vehicle *> &vehicles);
    void refreshTable();              // 显示全部
    void updateStatusBar();
    int  currentSelectedRow() const;  // 返回当前选中行在表格中的索引，-1 表示无

    // 从表格中收集所有 ID => 当前实际在 m_manager 中的顺序
    QString vehicleIdAtTableRow(int row) const;

    // --- 成员 ---
    VehicleManager m_manager;

    // 搜索组件
    QLineEdit    *m_searchEdit    = nullptr;
    QRadioButton *m_radioById     = nullptr;
    QRadioButton *m_radioByMfg    = nullptr;
    QRadioButton *m_radioByType   = nullptr;
    QButtonGroup *m_searchGroup   = nullptr;
    QPushButton  *m_searchBtn     = nullptr;
    QPushButton  *m_showAllBtn    = nullptr;
    QPushButton  *m_sortByDateBtn = nullptr;
    QPushButton  *m_sortByKmBtn   = nullptr;
    QPushButton  *m_sortByFCBtn   = nullptr;
    QPushButton  *m_resetSortBtn  = nullptr;

    // 表格
    QTableWidget *m_table         = nullptr;

    // 状态栏标签
    QLabel *m_statusTotal  = nullptr;
    QLabel *m_statusBus    = nullptr;
    QLabel *m_statusCar    = nullptr;
    QLabel *m_statusTruck  = nullptr;

    // 当前显示在表格里的车辆指针列表（用于刷新/选中映射）
    QVector<Vehicle *> m_displayedVehicles;

    // 排序方向标记（true = 升序，false = 降序）
    bool m_dateAscending = true;
    bool m_kmAscending   = true;
    bool m_fcAscending   = true;
};

#endif // MAINWINDOW_H
