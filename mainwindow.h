#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qpushbutton.h>

#include <qsqltablemodel.h>
#include <qsqlrelationaltablemodel.h>
#include <qtableview.h>

///namespace Ui {
//class MainWindow;
//}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //Ui::MainWindow *ui;
    QPushButton *reloadDbButton;
    QSqlTableModel *patientModel;
    QSqlTableModel *serviceModel;
    QSqlRelationalTableModel *treatmentModel;
    QTableView *patientView;
    QTableView *serviceView;
    QTableView *treatmentView;
    QSqlDatabase db;
private slots:
    void handleReloadDB();
};

#endif // MAINWINDOW_H
