#include "match.h"
#include <QRandomGenerator>
Match::Match(const Team &local, const Team &visitor, const QDateTime &dateTime, const QString &stadium)
    : localTeam(local), visitorTeam(visitor), matchDateTime(dateTime), stadium(stadium),
    possessionLocal(50), possessionVisitor(50),
    passesLocalTotal(0), passesLocalCompleted(0),
    passesVisitorTotal(0), passesVisitorCompleted(0),
    foulsLocal(0), foulsVisitor(0),
    offsidesLocal(0), offsidesVisitor(0),
    cornersLocal(0), cornersVisitor(0) {}

void Match::addEvent(int minute, const Player &player, MatchEvent::EventType type) {
    events.append({ minute, player, type });
}

void Match::setPossession(int localPercent, int visitorPercent) {
    possessionLocal = localPercent;
    possessionVisitor = visitorPercent;
}

void Match::setPassStats(int localTotal, int localCompleted, int visitorTotal, int visitorCompleted) {
    passesLocalTotal = localTotal;
    passesLocalCompleted = localCompleted;
    passesVisitorTotal = visitorTotal;
    passesVisitorCompleted = visitorCompleted;
}

void Match::setFouls(int local, int visitor) {
    foulsLocal = local;
    foulsVisitor = visitor;
}

void Match::setOffsides(int local, int visitor) {
    offsidesLocal = local;
    offsidesVisitor = visitor;
}

void Match::setCorners(int local, int visitor) {
    cornersLocal = local;
    cornersVisitor = visitor;
}

QVector<MatchEvent> Match::getEvents() const {
    return events;
}

Team Match::getLocalTeam() const { return localTeam; }
Team Match::getVisitorTeam() const { return visitorTeam; }

QDateTime Match::getDateTime() const { return matchDateTime; }
QString Match::getStadium() const { return stadium; }

QString Match::getScore() const {
    int goalsLocal = 0, goalsVisitor = 0;
    for (const auto &e : events) {
        if (e.type == MatchEvent::Goal) {
            if (localTeam.getPlayers().contains(e.player))
                ++goalsLocal;
            else
                ++goalsVisitor;
        }
    }
    return QString("%1 - %2").arg(goalsLocal).arg(goalsVisitor);
}

int Match::getGoalsForTeam(const Team &team) const {
    int count = 0;
    for (const auto &e : events) {
        if (e.type == MatchEvent::Goal && team.getPlayers().contains(e.player))
            ++count;
    }
    return count;
}

int Match::getYellowCards(const Team &team) const {
    int count = 0;
    for (const auto &e : events)
        if (e.type == MatchEvent::YellowCard && team.getPlayers().contains(e.player))
            ++count;
    return count;
}


int Match::getPossessionForTeam(const Team &team) const {
    return (team.getName() == localTeam.getName()) ? possessionLocal : possessionVisitor;
}

double Match::getPassAccuracyForTeam(const Team &team) const {
    if (team.getName() == localTeam.getName()) {
        return passesLocalTotal > 0 ? (100.0 * passesLocalCompleted / passesLocalTotal) : 0.0;
    } else {
        return passesVisitorTotal > 0 ? (100.0 * passesVisitorCompleted / passesVisitorTotal) : 0.0;
    }
}

void Match::simulateMatch(const Team &local, const Team &visitor) {
    localTeam = local;
    visitorTeam = visitor;
    events.clear();

    const int matchDuration = 90;

    // Simular posesión aleatoria entre 40 y 60
    possessionLocal = QRandomGenerator::global()->bounded(40, 61);
    possessionVisitor = 100 - possessionLocal;

    // Simular goles
    int goalsLocal = QRandomGenerator::global()->bounded(0, 5);
    int goalsVisitor = QRandomGenerator::global()->bounded(0, 5);
    if(goalsLocal == goalsVisitor) goalsLocal++;
    for (int i = 0; i < goalsLocal; ++i) {
        int minute = QRandomGenerator::global()->bounded(1, matchDuration + 1);
        Player scorer = local.getPlayers().at(QRandomGenerator::global()->bounded(local.getPlayers().size()));
        addEvent(minute, scorer, MatchEvent::Goal);
    }
    for (int i = 0; i < goalsVisitor; ++i) {
        int minute = QRandomGenerator::global()->bounded(1, matchDuration + 1);
        Player scorer = visitor.getPlayers().at(QRandomGenerator::global()->bounded(visitor.getPlayers().size()));
        addEvent(minute, scorer, MatchEvent::Goal);
    }

    // Simular faltas
    foulsLocal = QRandomGenerator::global()->bounded(5, 16);
    foulsVisitor = QRandomGenerator::global()->bounded(5, 16);

    // Simular offsides
    offsidesLocal = QRandomGenerator::global()->bounded(0, 6);
    offsidesVisitor = QRandomGenerator::global()->bounded(0, 6);

    // Simular corners
    cornersLocal = QRandomGenerator::global()->bounded(0, 10);
    cornersVisitor = QRandomGenerator::global()->bounded(0, 10);

    // Simular pases y precisión
    passesLocalTotal = QRandomGenerator::global()->bounded(100, 600);
    passesVisitorTotal = QRandomGenerator::global()->bounded(100, 600);

    passesLocalCompleted = QRandomGenerator::global()->bounded(
        static_cast<int>(passesLocalTotal * 0.5),
        passesLocalTotal + 1
    );

    passesVisitorCompleted = QRandomGenerator::global()->bounded(
        static_cast<int>(passesVisitorTotal * 0.5),
        passesVisitorTotal + 1
    );

}

int Match::getScore1() const {
    return getGoalsForTeam(localTeam);
}

int Match::getScore2() const {
    return getGoalsForTeam(visitorTeam);
}
