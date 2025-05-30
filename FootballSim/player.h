#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player {
public:
    // Constructor
    Player(const QString &name, int age, const QString &position, int number, double rating);

    // Getters
    QString getName() const;
    int getAge() const;
    QString getPosition() const;
    int getNumber() const;
    double getRating() const;

    // Setters
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
    double rating; // por ejemplo, 0.0 a 10.0
};

#endif // PLAYER_H
