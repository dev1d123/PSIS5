#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QMessageBox>
#include <QLabel>
#include <QList>
#include <QDebug>
#include <QRegularExpression>
#include <QRandomGenerator>
#include <QPixmap>
#include <algorithm>

MainWindow::MainWindow(QVector<Team> equipos, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , equipos(equipos)
    , faseActual(OCTAVOS)
    , partidoActual(0)
{
    ui->setupUi(this);

    // Inicializar la interfaz con los equipos en octavos
    QList<QLabel*> ofLabels = this->findChildren<QLabel*>(QRegularExpression("^label_OF_"));
    std::sort(ofLabels.begin(), ofLabels.end(), [](QLabel* a, QLabel* b) {
        return a->objectName() < b->objectName();
    });

    int i = 0;
    for (QLabel* label : ofLabels) {
        if(i >= equipos.size()) {
            qWarning() << "Advertencia: Más labels que equipos";
            break;
        }

        QString name = equipos[i].getName();
        QString path = ":/images/" + name + ".png";
        QPixmap imagen(path);

        if(imagen.isNull()) {
            qWarning() << "No se encontró la imagen:" << path;
            imagen = QPixmap(":/images/default.png");
        }

        label->setPixmap(imagen.scaled(label->size(),
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation));
        label->setAlignment(Qt::AlignCenter);
        i++;
    }

    // Conectar botón de simular
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        if (faseActual == COMPLETADO) {
            QMessageBox::information(this, "Torneo Completo",
                                     "El torneo ha concluido. ¡Reinicia la aplicación para comenzar de nuevo!");
            return;
        }

        switch (faseActual) {
        case OCTAVOS:
            simularPartido("label_CF_", 16, 8, "octavos de final");
            break;
        case CUARTOS:
            simularPartido("label_SF_", 8, 4, "cuartos de final");
            break;
        case SEMIFINALES:
            simularPartido("label_F_", 4, 2, "semifinales");
            break;
        case FINAL:
            simularFinal();
            break;
        default:
            break;
        }
    });
}

void MainWindow::simularPartido(const QString& prefix, int equiposNecesarios, int ganadoresNecesarios, const QString& nombreFase) {
    if(equipos.size() < equiposNecesarios) {
        QMessageBox::warning(this, "Error",
                             QString("No hay suficientes equipos para %1 (se necesitan %2)")
                                 .arg(nombreFase)
                                 .arg(equiposNecesarios));
        return;
    }

    QLabel* currentLabel = findChild<QLabel*>(QString("%1%2").arg(prefix).arg(partidoActual + 1));
    if(!currentLabel) {
        QMessageBox::warning(this, "Error",
                             QString("No se encontró el label para el partido %1 de %2")
                                 .arg(partidoActual + 1)
                                 .arg(nombreFase));
        return;
    }

    int index1 = partidoActual * 2;
    int index2 = partidoActual * 2 + 1;
    Team& equipo1 = equipos[index1];
    Team& equipo2 = equipos[index2];

    bool ganaEquipo1 = QRandomGenerator::global()->bounded(2) == 0;
    Team ganador = ganaEquipo1 ? equipo1 : equipo2;
    equiposSiguienteFase.append(ganador);

    QString path = ":/images/" + ganador.getName() + ".png";
    QPixmap imagen(path);
    if(imagen.isNull()) {
        imagen = QPixmap(":/images/default.png");
    }

    currentLabel->setPixmap(imagen.scaled(currentLabel->size(),
                                          Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation));
    currentLabel->setAlignment(Qt::AlignCenter);

    QMessageBox::information(this, QString("Partido de %1").arg(nombreFase),
                             QString("Partido %1 completado:\n%2 vs %3\nGanador: %4")
                                 .arg(partidoActual + 1)
                                 .arg(equipo1.getName())
                                 .arg(equipo2.getName())
                                 .arg(ganador.getName()));

    partidoActual++;

    if(partidoActual >= equiposNecesarios / 2) {
        equipos = equiposSiguienteFase;
        equiposSiguienteFase.clear();
        partidoActual = 0;

        if(faseActual == OCTAVOS) {
            faseActual = CUARTOS;
        } else if(faseActual == CUARTOS) {
            faseActual = SEMIFINALES;
        } else if(faseActual == SEMIFINALES) {
            faseActual = FINAL;
        }
    }
}

void MainWindow::simularFinal() {
    if(equipos.size() < 2) {
        QMessageBox::warning(this, "Error", "No hay suficientes equipos para la final (se necesitan 2)");
        return;
    }

    QLabel* labelCampeon = findChild<QLabel*>("label_W");
    if(!labelCampeon) {
        QMessageBox::warning(this, "Error", "No se encontró el label para el campeón (label_W)");
        return;
    }

    Team& equipo1 = equipos[0];
    Team& equipo2 = equipos[1];

    bool ganaEquipo1 = QRandomGenerator::global()->bounded(2) == 0;
    Team campeon = ganaEquipo1 ? equipo1 : equipo2;

    QString path = ":/images/" + campeon.getName() + ".png";
    QPixmap imagen(path);
    if(imagen.isNull()) {
        imagen = QPixmap(":/images/default.png");
    }

    labelCampeon->setPixmap(imagen.scaled(labelCampeon->size(),
                                          Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation));
    labelCampeon->setAlignment(Qt::AlignCenter);

    QMessageBox::information(this, "Final completada",
                             QString("Final:\n%1 vs %2\n\n¡%3 es el campeón del torneo!")
                                 .arg(equipo1.getName())
                                 .arg(equipo2.getName())
                                 .arg(campeon.getName()));

    faseActual = COMPLETADO;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::recibirMensaje(const QString &msg) {
    QMessageBox::information(this, "Mensaje recibido", msg);
}
