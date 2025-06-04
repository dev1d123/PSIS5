#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "mainwindow.h"
#include "team.h"
#include "player.h"
#include <bits/stdc++.h>
#include "match.h"
using namespace std;



int main(int argc, char *argv[]) {

    cout<<"Comenzando torneo"<<endl;
    QVector<Team> teams;

    QFile file(":/images/teams.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "No se pudo abrir el archivo teams.json";
        return -1;
    }
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Error al parsear JSON:" << parseError.errorString();
        return -1;
    }
    QJsonObject rootObj = doc.object();
    QJsonArray teamArray = rootObj["teams"].toArray();
    for (const QJsonValue &teamValue : teamArray) {
        QJsonObject teamObj = teamValue.toObject();
        QString name = teamObj["name"].toString();
        QString coach = teamObj["coach"].toString();
        QString country = teamObj["country"].toString();

        Team team(name, coach, country);

        QJsonArray playerArray = teamObj["players"].toArray();
        for (const QJsonValue &playerValue : playerArray) {
            QJsonObject playerObj = playerValue.toObject();
            QString pname = playerObj["name"].toString();
            int age = playerObj["age"].toInt();
            QString position = playerObj["position"].toString();
            int number = playerObj["number"].toInt();
            double rating = playerObj["rating"].toDouble();

            Player player(pname, age, position, number, rating);
            team.addPlayer(player);
        }

        teams.append(team);


    }
    std::random_shuffle(teams.begin(), teams.end());



    QApplication a(argc, argv);
    MainWindow w(teams); //aqui se deberian entregar los equipos necesarios
    w.show();
    return a.exec();

}

