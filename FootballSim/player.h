#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player {
public:

    Player(const QString &name, int age, const QString &position, int number, double rating);


    QString getName() const;
    int getAge() const;
    QString getPosition() const;
    int getNumber() const;
    double getRating() const;

    void setName(const QString &name);
    void setAge(int age);
    void setPosition(const QString &position);
    void setNumber(int number);
    void setRating(double rating);

private:
    QString name;
    int age;
    QString position;
    int number;
    double rating;
};

#endif // PLAYER_H
