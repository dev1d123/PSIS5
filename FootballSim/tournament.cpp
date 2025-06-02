#include "tournament.h"
#include <QRandomGenerator>
#include <algorithm>

Tournament::Tournament(const QVector<Team>& initialTeams) : currentTeams(initialTeams) {}

void Tournament::playTournament() {
    QStringList rounds = { "Octavos de final", "Cuartos de final", "Semifinales", "Final" };

    for (const QString& round : rounds) {
        currentTeams = playRound(currentTeams, round);
    }
}

QVector<Match> Tournament::getMatches() const {
    return allMatches;
}

Team Tournament::getChampion() const {
    if (!currentTeams.isEmpty()) return currentTeams.first();
    return Team("Sin campeón", "Desconocido");
}

QVector<Team> Tournament::playRound(const QVector<Team>& teams, const QString& roundName) {
    QVector<Team> winners;
    for (int i = 0; i < teams.size(); i += 2) {
        Match m = simulateMatch(teams[i], teams[i + 1], roundName);
        allMatches.append(m);
        Team winner = m.getWinner();
        winners.append(winner);
    }
    return winners;
}

Match Tournament::simulateMatch(const Team& teamA, const Team& teamB, const QString& round) {
    Match match(teamA, teamB);
    match.setRound(round);

    // Simulación básica: elige al azar goles y eventos
    int goalsA = QRandomGenerator::global()->bounded(4);
    int goalsB = QRandomGenerator::global()->bounded(4);

    QVector<Player> playersA = teamA.getPlayers();
    QVector<Player> playersB = teamB.getPlayers();

    for (int i = 0; i < goalsA; ++i) {
        int minute = QRandomGenerator::global()->bounded(1, 91);
        Player p = playersA.at(QRandomGenerator::global()->bounded(playersA.size()));
        match.addEvent(MatchEvent(MatchEvent::Goal, minute, p));
    }

    for (int i = 0; i < goalsB; ++i) {
        int minute = QRandomGenerator::global()->bounded(1, 91);
        Player p = playersB.at(QRandomGenerator::global()->bounded(playersB.size()));
        match.addEvent(MatchEvent(MatchEvent::Goal, minute, p));
    }

    return match;
}
