#include "match.h"

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

// Métodos similares para red cards, shots, etc.

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
