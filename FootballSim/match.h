#ifndef MATCH_H
#define MATCH_H

#include <QString>
#include <QDateTime>
#include <QVector>
#include "team.h"
#include "player.h"

struct MatchEvent {
    enum EventType { Goal, YellowCard, RedCard, Offside, Corner, Foul, ShotOnTarget, ShotOffTarget };

    int minute;
    Player player;
    EventType type;
};

class Match {
public:
    Match(const Team &local, const Team &visitor, const QDateTime &dateTime, const QString &stadium);

    void addEvent(int minute, const Player &player, MatchEvent::EventType type);
    void setPossession(int localPercent, int visitorPercent);
    void setPassStats(int localTotal, int localCompleted, int visitorTotal, int visitorCompleted);
    void setFouls(int local, int visitor);
    void setOffsides(int local, int visitor);
    void setCorners(int local, int visitor);

    Team getLocalTeam() const;
    Team getVisitorTeam() const;
    QVector<MatchEvent> getEvents() const;
    int getGoalsForTeam(const Team &team) const;

    QString getScore() const;
    QDateTime getDateTime() const;
    QString getStadium() const;

    int getShotsOnTarget(const Team &team) const;
    int getShotsOffTarget(const Team &team) const;
    int getYellowCards(const Team &team) const;
    int getRedCards(const Team &team) const;

    int getPossessionForTeam(const Team &team) const;
    double getPassAccuracyForTeam(const Team &team) const;

    int getFoulsForTeam(const Team& team) const {
        return (team.getName() == localTeam.getName()) ? foulsLocal : foulsVisitor;
    }

    int getOffsidesForTeam(const Team& team) const {
        return (team.getName() == localTeam.getName()) ? offsidesLocal : offsidesVisitor;
    }

    int getCornersForTeam(const Team& team) const {
        return (team.getName() == localTeam.getName()) ? cornersLocal : cornersVisitor;
    }
    void simulateMatch(const Team &local, const Team &visitor);
    int getScore1() const;
    int getScore2() const;

private:
    Team localTeam;
    Team visitorTeam;
    QDateTime matchDateTime;
    QString stadium;

    QVector<MatchEvent> events;

    int possessionLocal, possessionVisitor;
    int passesLocalTotal, passesLocalCompleted;
    int passesVisitorTotal, passesVisitorCompleted;
    int foulsLocal, foulsVisitor;
    int offsidesLocal, offsidesVisitor;
    int cornersLocal, cornersVisitor;
};

#endif // MATCH_H

