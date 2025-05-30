#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        Dialog *d = new Dialog(this);
        connect(d, &Dialog::mensajeEnviado, this, &MainWindow::recibirMensaje);
        d->exec(); // ventana modal
        delete d;
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::recibirMensaje(const QString &msg) {
    QMessageBox::information(this, "Mensaje recibido", msg);
}
