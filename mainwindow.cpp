#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openForm()
{
    authorizationForm = new Authorization(this);
    authorizationForm->show();
}

void MainWindow::on_pushButton_clicked()
{
    openForm();
}

