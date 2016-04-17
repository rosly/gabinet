#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dbmanager.h"
#include <qsqltablemodel.h>
#include <qtableview.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DbManager db("gabinet.sql3");
    db.listPacjents();

    QSqlTableModel *model = new QSqlTableModel(this, db.db);
    model->setTable("pacjent");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    //model->setHeaderData(1, Qt::Horizontal, tr("nazwisko"));
    //model->setHeaderData(2, Qt::Horizontal, tr("imie"));

    QTableView *view = new QTableView;
    view->setModel(model);
    view->hideColumn(0); // don't show the ID
    view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
