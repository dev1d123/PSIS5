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
            simularPartido("label_CF_", "text_OF_", 16, 8, "octavos de final");
            break;
        case CUARTOS:
            simularPartido("label_SF_", "text_CF_", 8, 4, "cuartos de final");
            break;
        case SEMIFINALES:
            simularPartido("label_F_", "text_SF_", 4, 2, "semifinales");
            break;
        case FINAL:
            simularFinal();
            break;
        default:
            break;
        }
    });
}

void MainWindow::simularPartido(const QString& prefix, const QString& prefix2, int equiposNecesarios, int ganadoresNecesarios, const QString& nombreFase) {
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

    QString name1 = QString("%1%2").arg(prefix2).arg(index1 + 1);
    QString name2 = QString("%1%2").arg(prefix2).arg(index2 + 1);

    QLineEdit* text1 = findChild<QLineEdit*>(name1);
    QLineEdit* text2 = findChild<QLineEdit*>(name2);

    Match partido(equipo1, equipo2, QDateTime::currentDateTime(), "Estadio Central");
    partido.simulateMatch(equipo1, equipo2);

    if (text1) {
        text1->setText(equipo1.getName() + " - " + QString::number(partido.getScore1()));
    } else {
        QMessageBox::warning(this, "Advertencia",
                             QString("No se encontró el campo de texto con nombre '%1' para %2").arg(name1).arg(equipo1.getName()));
    }

    if (text2) {
        text2->setText(equipo2.getName() + " - " + QString::number(partido.getScore2()));
    } else {
        QMessageBox::warning(this, "Advertencia",
                             QString("No se encontró el campo de texto con nombre '%1' para %2").arg(name2).arg(equipo2.getName()));
    }

    Team ganador = (partido.getScore1() > partido.getScore2()) ? equipo1 : equipo2;
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

    mostrarResumenPartido(partido, nombreFase);

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
    Match partido(equipo1, equipo2, QDateTime::currentDateTime(), "Estadio Central");
    partido.simulateMatch(equipo1, equipo2);


    QString name1 = QString("text_F_1");
    QString name2 = QString("text_F_2");

    QLineEdit* text1 = findChild<QLineEdit*>(name1);
    QLineEdit* text2 = findChild<QLineEdit*>(name2);


    if (text1) {
        text1->setText(equipo1.getName() + " - " + QString::number(partido.getScore1()));
    } else {
        QMessageBox::warning(this, "Advertencia",
                             QString("No se encontró el campo de texto con nombre '%1' para %2").arg(name1).arg(equipo1.getName()));
    }

    if (text2) {
        text2->setText(equipo2.getName() + " - " + QString::number(partido.getScore2()));
    } else {
        QMessageBox::warning(this, "Advertencia",
                             QString("No se encontró el campo de texto con nombre '%1' para %2").arg(name2).arg(equipo2.getName()));
    }




    Team campeon = (partido.getScore1() > partido.getScore2()) ? equipo1 : equipo2;

    QString path = ":/images/" + campeon.getName() + ".png";
    QPixmap imagen(path);
    if(imagen.isNull()) {
        imagen = QPixmap(":/images/default.png");
    }

    labelCampeon->setPixmap(imagen.scaled(labelCampeon->size(),
                                          Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation));
    labelCampeon->setAlignment(Qt::AlignCenter);


    faseActual = COMPLETADO;

    mostrarResumenPartido(partido, "CAMPEON");


}
void MainWindow::mostrarResumenPartido(const Match& partido, const QString& fase) {
    QString resumen;
    QTextStream stream(&resumen);

    stream << "<h2>Partido de " << fase << "</h2>";
    stream << "<b>" << partido.getLocalTeam().getName() << " vs "
           << partido.getVisitorTeam().getName() << "</b><br>";
    stream << "<b>Estadio:</b> " << partido.getStadium() << "<br>";
    stream << "<b>Resultado Final:</b> " << partido.getScore() << "<br><br>";

    stream << "<b>Estadísticas:</b><br>";
    stream << "Posesión: " << partido.getPossessionForTeam(partido.getLocalTeam()) << "% - "
           << partido.getPossessionForTeam(partido.getVisitorTeam()) << "%<br>";
    stream << "Precisión de pases: " << QString::number(partido.getPassAccuracyForTeam(partido.getLocalTeam()), 'f', 1)
           << "% - " << QString::number(partido.getPassAccuracyForTeam(partido.getVisitorTeam()), 'f', 1) << "%<br>";
    stream << "Faltas: " << partido.getFoulsForTeam(partido.getLocalTeam()) << " - "
           << partido.getFoulsForTeam(partido.getVisitorTeam()) << "<br>";
    stream << "Offsides: " << partido.getOffsidesForTeam(partido.getLocalTeam()) << " - "
           << partido.getOffsidesForTeam(partido.getVisitorTeam()) << "<br>";
    stream << "Corners: " << partido.getCornersForTeam(partido.getLocalTeam()) << " - "
           << partido.getCornersForTeam(partido.getVisitorTeam()) << "<br><br>";

    stream << "<b>Eventos importantes:</b><br>";
    QVector<MatchEvent> eventos = partido.getEvents();
    std::sort(eventos.begin(), eventos.end(), [](const MatchEvent& a, const MatchEvent& b) {
        return a.minute < b.minute;
    });

    for (const MatchEvent& evento : eventos) {
        stream << evento.minute << "' - ";
        if (evento.type == MatchEvent::Goal) {
            stream << "⚽ <b>GOL!</b> " << evento.player.getName() << "<br>";
        } else if (evento.type == MatchEvent::YellowCard) {
            stream << "🟨 <b>Tarjeta amarilla</b> para " << evento.player.getName() << "<br>";
        } else if (evento.type == MatchEvent::RedCard) {
            stream << "🟥 <b>Tarjeta roja</b> para " << evento.player.getName() << "<br>";
        }
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("Resumen del Partido");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(resumen);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::recibirMensaje(const QString &msg) {
    QMessageBox::information(this, "Mensaje recibido", msg);
}
