#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "team.h"
#include "match.h"
#include <QVector>
#include <QString>

class Tournament {
public:
    Tournament(const QVector<Team>& initialTeams);

    void playTournament(); // Ejecuta todo el torneo
    QVector<Match> getMatches() const;
    Team getChampion() const;

private:
    QVector<Team> currentTeams; // Equipos que siguen en competencia
    QVector<Match> allMatches;

    QVector<Team> playRound(const QVector<Team>& teams, const QString& roundName);
    Match simulateMatch(const Team& teamA, const Team& teamB, const QString& round);
};

#endif // TOURNAMENT_H
