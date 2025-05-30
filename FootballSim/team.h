#ifndef TEAM_H
#define TEAM_H

#include <QString>
#include <QVector>
#include "player.h"

class Team {
public:
    Team(const QString &name, const QString &coach, const QString &country);

    QString getName() const;
    QString getCoach() const;
    QString getCountry() const;
    QVector<Player> getPlayers() const;

    void setName(const QString &name);
    void setCoach(const QString &coach);
    void setCountry(const QString &country);


    void addPlayer(const Player &player);
    void removePlayer(int index);
    void clearPlayers();

    double getAverageRating() const;
    int getPlayerCount() const;

private:
    QString name;
    QString coach;
    QString country;
    QVector<Player> players;
};

#endif
