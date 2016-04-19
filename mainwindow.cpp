#include <qtablewidget.h>
#include <qsqlrelationaldelegate.h>
#include <qfiledialog.h>
#include <qdebug.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    db(QSqlDatabase::addDatabase("QSQLITE")) //QSqlDatabase is a static class, does not have new operator
//: ui(new Ui::MainWindow)
{
    QString fileName;

    // TODO remove this later since we dont use the generated UI
    //ui->setupUi(this);

    for(;;) {
        fileName = QFileDialog::getOpenFileName(this,
            tr("Open database"), "/", tr("sqlite3 files (*.sql3)"));

        if (fileName.isNull()) {
            exit(EXIT_FAILURE);
        }

        db.setDatabaseName(fileName);
        if (db.open())
            break; //sucess

        qDebug() << "Error: connection with database fail";
    }

    //main tab widget at stretched to main window
    QTabWidget *mainTab = new QTabWidget;

    //set up the patients view
    patientModel = new QSqlTableModel(this, db);
    patientModel->setTable("pacjent");
    patientModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientModel->select();
    //patientModel->setHeaderData(1, Qt::Horizontal, tr("nazwisko"));
    //patientModel->setHeaderData(2, Qt::Horizontal, tr("imie"));

    patientView = new QTableView;
    patientView->setModel(patientModel);
    patientView->hideColumn(0); // don't show the ID
    patientView->show();

    mainTab->addTab(patientView, "Pacjenci");

    //set up the service view
    serviceModel = new QSqlTableModel(this, db);
    serviceModel->setTable("usluga");
    serviceModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    serviceModel->select();

    serviceView = new QTableView;
    serviceView->setModel(serviceModel);
    serviceView->hideColumn(0); // don't show the ID
    serviceView->show();

    mainTab->addTab(serviceView, "Usługi");

    //set up the treatments view
    treatmentModel = new QSqlRelationalTableModel(this, db);
    treatmentModel->setTable("zabieg");
    treatmentModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    treatmentModel->setRelation(treatmentModel->fieldIndex("pacjent"), QSqlRelation("pacjent", "id", "nazwisko"));
    treatmentModel->setRelation(treatmentModel->fieldIndex("usluga"), QSqlRelation("usluga", "id", "nazwa"));
    treatmentModel->select();

    treatmentView = new QTableView;
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

#include <qsqlquery.h>
#include <qsqlrecord.h>

void MainWindow::handleReloadDB()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open database"), "/", tr("sqlite3 files (*.sql3)"));

    db.setDatabaseName(fileName);
    if (!db.open()) {
      qDebug() << "Error: connection with database fail";
    } else {
      bool ok;

      qDebug() << "Database: connection ok";

      QSqlQuery query("SELECT * FROM pacjent");
      int idName = query.record().indexOf("nazwisko");
      while (query.next()) {
          QString name = query.value(idName).toString();
          qDebug() << name;
      }

#if 0
      patientView->setModel(NULL);
      patientView->setModel(patientModel);
      serviceView->setModel(NULL);
      serviceView->setModel(serviceModel);
      treatmentView->setModel(NULL);
      treatmentView->setModel(treatmentModel);
#endif

      ok = patientModel->select();
      ok &= serviceModel->select();
      ok &= treatmentModel->select();
    }
}

MainWindow::~MainWindow()
{
    //delete ui;
    db.close();
}
