#include "player.h"

Player::Player(const QString &name, int age, const QString &position, int number, double rating)
    : name(name), age(age), position(position), number(number), rating(rating) {}

QString Player::getName() const {
    return name;
}

int Player::getAge() const {
    return age;
}

QString Player::getPosition() const {
    return position;
}

int Player::getNumber() const {
    return number;
}

double Player::getRating() const {
    return rating;
}

void Player::setName(const QString &name) {
    this->name = name;
}

void Player::setAge(int age) {
    this->age = age;
}

void Player::setPosition(const QString &position) {
    this->position = position;
}

void Player::setNumber(int number) {
    this->number = number;
}

void Player::setRating(double rating) {
    this->rating = rating;
}
