#include "includes/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db.connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openForm()
{
    this->close();
    authorizationForm = new Authorization(this);
    authorizationForm->show();

}

void MainWindow::on_pushButton_clicked()
{
    openForm();
}

