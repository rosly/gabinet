#include <qtableview.h>
#include <qtablewidget.h>
#include <qsqltablemodel.h>
#include <qsqlrelationaltablemodel.h>
#include <qsqlrelationaldelegate.h>
#include <qfiledialog.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbmanager.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
//: ui(new Ui::MainWindow)
{
    // TODO remove this later since we dont use the generated UI
    //ui->setupUi(this);

    //main tab widget at stretched to main window
    QTabWidget *mainTab = new QTabWidget;

    //set up the patients view
    QSqlTableModel *patientModel = new QSqlTableModel(this, db.db);
    patientModel->setTable("pacjent");
    patientModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientModel->select();
    //patientModel->setHeaderData(1, Qt::Horizontal, tr("nazwisko"));
    //patientModel->setHeaderData(2, Qt::Horizontal, tr("imie"));

    QTableView *patientView = new QTableView;
    patientView->setModel(patientModel);
    patientView->hideColumn(0); // don't show the ID
    patientView->show();

    mainTab->addTab(patientView, "Pacjenci");

    //set up the services view
    QSqlTableModel *servicesModel = new QSqlTableModel(this, db.db);
    servicesModel->setTable("usluga");
    servicesModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    servicesModel->select();

    QTableView *servicesView = new QTableView;
    servicesView->setModel(servicesModel);
    servicesView->hideColumn(0); // don't show the ID
    servicesView->show();

    mainTab->addTab(servicesView, "Usługi");

    //set up the treatments view
    QSqlRelationalTableModel *treatmentModel = new QSqlRelationalTableModel(this, db.db);
    treatmentModel->setTable("zabieg");
    treatmentModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    treatmentModel->setRelation(treatmentModel->fieldIndex("pacjent"), QSqlRelation("pacjent", "id", "nazwisko"));
    treatmentModel->setRelation(treatmentModel->fieldIndex("usluga"), QSqlRelation("usluga", "id", "nazwa"));

    treatmentModel->select();

    QTableView *treatmentView = new QTableView;
    treatmentView->setModel(treatmentModel);
    treatmentView->hideColumn(0); // don't show the ID
    treatmentView->setItemDelegate(new QSqlRelationalDelegate(treatmentView));
    treatmentView->show();

    mainTab->addTab(treatmentView, "Zabiegi");

    //add load database button
    reloadDbButton = new QPushButton("Load DB");
    connect(reloadDbButton, SIGNAL (released()), this, SLOT (handleReloadDB()));

    //finaly use widget hierarchy
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mainTab);
    mainLayout->addWidget(reloadDbButton);
    QWidget *subWindow = new QWidget();
    subWindow->setLayout(mainLayout);
    setCentralWidget(subWindow);
}

void MainWindow::handleReloadDB()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open database"), "/", tr("sqlite3 files (*.sql3)"));
    db.open(fileName);
}

MainWindow::~MainWindow()
{
    //delete ui;
}
