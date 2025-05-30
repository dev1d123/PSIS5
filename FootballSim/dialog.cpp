#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog) {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        emit mensajeEnviado("¡Hola desde la segunda ventana!");
        this->close();
    });
}

Dialog::~Dialog() {
    delete ui;
}
