#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "team.h"
#include "match.h"  // Asegúrate de incluir el archivo de Match

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QVector<Team> equipos, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void recibirMensaje(const QString &msg);

private:
    void simularPartido(const QString& prefix,const QString& prefix2 ,int equiposNecesarios, int ganadoresNecesarios, const QString& nombreFase);
    void simularEstadisticasPartido(Match& partido);
    void simularEventosPartido(Match& partido);
    void mostrarResumenPartido(const Match& partido, const QString& fase);
    void simularFinal();
    enum Fase { OCTAVOS, CUARTOS, SEMIFINALES, FINAL, COMPLETADO };

    Ui::MainWindow *ui;
    QVector<Team> equipos;
    QVector<Team> equiposSiguienteFase;
    Fase faseActual;
    int partidoActual;
};

#endif // MAINWINDOW_H
