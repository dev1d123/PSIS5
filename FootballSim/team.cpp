#include "team.h"

Team::Team(const QString &name, const QString &coach, const QString &country)
    : name(name), coach(coach), country(country) {}

QString Team::getName() const {
    return name;
}

QString Team::getCoach() const {
    return coach;
}

QString Team::getCountry() const {
    return country;
}

void Team::setName(const QString &name) {
    this->name = name;
}

void Team::setCoach(const QString &coach) {
    this->coach = coach;
}

void Team::setCountry(const QString &country) {
    this->country = country;
}

QVector<Player> Team::getPlayers() const {
    return players;
}

void Team::addPlayer(const Player &player) {
    players.append(player);
}

void Team::removePlayer(int index) {
    if (index >= 0 && index < players.size())
        players.removeAt(index);
}

void Team::clearPlayers() {
    players.clear();
}

double Team::getAverageRating() const {
    if (players.isEmpty())
        return 0.0;

    double sum = 0.0;
    for (const Player &p : players)
        sum += p.getRating();

    return sum / players.size();
}

int Team::getPlayerCount() const {
    return players.size();
}
