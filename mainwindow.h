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

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onAddVehicle();
    void onEditVehicle();
    void onDeleteVehicle();
    void onDeleteById();
    void onBatchDelete();
    void onSearch();
    void onShowAll();
    void onSave();
    void onSaveToFile();
    void onLoadFromFile();
    void onStatistics();
    void onAbout();
    void onVersion();
    void onSetFuelPrice();
    void onHeaderClicked(int section);
    void onTableContextMenu(const QPoint &pos);

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

    // 表格
    QTableWidget *m_table         = nullptr;

    // 状态栏标签
    QLabel *m_statusTotal  = nullptr;
    QLabel *m_statusBus    = nullptr;
    QLabel *m_statusCar    = nullptr;
    QLabel *m_statusTruck  = nullptr;

    // 当前显示在表格里的车辆指针列表（用于刷新/选中映射）
    QVector<Vehicle *> m_displayedVehicles;

    // 表头排序状态
    int m_sortColumn = -1;           // 当前排序列，-1 表示未排序
    Qt::SortOrder m_sortOrder = Qt::AscendingOrder;

    // 当前打开的文件路径（空表示新文件）
    QString m_currentFilePath;

    // 未保存修改标记
    bool m_unsavedChanges = false;
};

#endif // MAINWINDOW_H
