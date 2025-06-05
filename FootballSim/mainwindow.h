#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "team.h"
#include "match.h"
// Estructura para estadísticas de jugador
struct PlayerStats {
    QString name;
    int goals = 0;
};

// Estructura para estadísticas de equipo
struct TeamStats {
    QString name;
    int wins = 0;
    int draws = 0;
    int losses = 0;
    int goalsFor = 0;
    int goalsAgainst = 0;
    PlayerStats topScorer;
    QVector<PlayerStats> players;
};
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
    void mostrarEstadisticasFinales() ;

    enum Fase { OCTAVOS, CUARTOS, SEMIFINALES, FINAL, COMPLETADO };
    QMap<QString, TeamStats> teamStats; // Para llevar registro de las estadísticas
    Ui::MainWindow *ui;
    QVector<Team> equipos;
    QVector<Team> equiposSiguienteFase;
    Fase faseActual;
    int partidoActual;
};

#endif // MAINWINDOW_H
