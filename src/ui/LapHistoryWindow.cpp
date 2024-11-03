/*
    GUI CONSTRUCTION UNDER PROGRESS

#include "LapHistoryWindow.h"
#include <QHeaderView>
#include <qboxlayout.h>
#include <qtablewidget.h>


LapHistoryWindow::LapHistoryWindow(const DriverData& driver, QWidget *parent)
    : QWidget(parent) {

    setWindowTitle(QString("Driver %1 Lap History").arg(QString::fromStdString(driver.driverName)));
    resize(400,600);

    mainLayout = new QVBoxLayout(this);

    // Set das title
    titleLabel = new QLabel(QString("Driver %1").arg(QString::fromStdString(driver.driverName)), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Table to display the lap history
    lapHistoryTable = new QTableWidget(this);
    lapHistoryTable->setColumnCount(4);
    lapHistoryTable->setHorizontalHeaderLabels({"Lap", "Position", "Lap Time", "Sector Times"});
    lapHistoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    lapHistoryTable->verticalHeader()->setVisible(false);
    lapHistoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addWidget(lapHistoryTable);

    // Populate the table with ze data
    // TODO COMPLETE THIS
    int row = 0;
    for (int i = driver.lapTimes.size() - 1; i >= 0; i-- ) {
        
    }
}
*/
