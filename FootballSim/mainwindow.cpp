#include <bits/stdc++.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QMessageBox>
#include <QLabel>
#include <QList>
#include <QDebug>
#include <iostream>
#include <string>
using namespace std;
MainWindow::MainWindow(QVector<Team> equipos, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , equipos(equipos) // guarda el arreglo
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        Dialog *d = new Dialog(this);
        connect(d, &Dialog::mensajeEnviado, this, &MainWindow::recibirMensaje);
        d->exec();
        delete d;
    });

    cout << "=== Equipos recibidos en MainWindow ===" << endl;
    for (const Team &team : equipos) {
        cout << "Equipo: " << team.getName().toStdString() << endl;
        for (const Player &p : team.getPlayers()) {
            cout << "  Jugador: " << p.getName().toStdString()
            << " | Rating: " << p.getRating()
            << endl;
        }
    }
}
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::recibirMensaje(const QString &msg) {
    QMessageBox::information(this, "Mensaje recibido", msg);
}
