// mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "team.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QVector<Team> equipos, QWidget *parent = nullptr);
    ~MainWindow();

    void recibirMensaje(const QString &msg);

private:
    enum Fase {
        OCTAVOS,
        CUARTOS,
        SEMIFINALES,
        FINAL,
        COMPLETADO
    };

    void simularPartido(const QString &prefix, int equiposNecesarios, int ganadoresNecesarios, const QString &nombreFase);
    void simularFinal();
    Ui::MainWindow *ui;
    QVector<Team> equipos;
    QVector<Team> equiposSiguienteFase;
    Fase faseActual;
    int partidoActual;
};

#endif // MAINWINDOW_H
