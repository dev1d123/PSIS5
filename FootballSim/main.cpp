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

void simularRonda(const QVector<Team> &equipos, QVector<Team> &ganadores, const QString &nombreRonda) {
    cout << "\n=== " << nombreRonda.toStdString() << " ===" << endl;
    for (int i = 0; i < equipos.size(); i += 2) {
        const Team &equipo1 = equipos[i];
        const Team &equipo2 = equipos[i + 1];

        int score1 = rand() % 5; // resultado aleatorio entre 0 y 4
        int score2 = rand() % 5;

        // Desempate si hay empate (para no usar penales en esta versión)
        while (score1 == score2) {
            score1 = rand() % 5;
            score2 = rand() % 5;
        }

        cout << equipo1.getName().toStdString() << " " << score1
             << " - " << score2 << " " << equipo2.getName().toStdString() << endl;

        if (score1 > score2)
            ganadores.append(equipo1);
        else
            ganadores.append(equipo2);
    }

    cout << "\nPresiona ENTER para continuar a la siguiente ronda...";
    cin.ignore();
}

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

        cout << "Equipo cargado: " << name.toStdString() << endl;
        cout << "  Coach: " << coach.toStdString() << endl;
        cout << "  País: " << country.toStdString() << endl;
        cout << "  Jugadores:" << endl;

        for (const Player &p : team.getPlayers()) {
            cout << "    Nombre: " << p.getName().toStdString()
            << " | Edad: " << p.getAge()
            << " | Posición: " << p.getPosition().toStdString()
            << " | Número: " << p.getNumber()
            << " | Rating: " << p.getRating()
            << endl;
        }

    }
    cout<<"Iniciando torneo"<<endl;
    std::random_shuffle(teams.begin(), teams.end());

    // SIMULACIÓN DEL TORNEO
    if (teams.size() != 16) {
        cerr << "Se necesitan exactamente 16 equipos para simular el torneo." << endl;
    } else {
        srand(static_cast<unsigned int>(time(nullptr))); // semilla aleatoria

        QVector<Team> rondaActual = teams;
        std::random_shuffle(rondaActual.begin(), rondaActual.end());

        QVector<Team> ganadores;

        simularRonda(rondaActual, ganadores, "Octavos de Final");

        rondaActual = ganadores;
        ganadores.clear();
        simularRonda(rondaActual, ganadores, "Cuartos de Final");

        rondaActual = ganadores;
        ganadores.clear();
        simularRonda(rondaActual, ganadores, "Semifinales");

        rondaActual = ganadores;
        ganadores.clear();
        simularRonda(rondaActual, ganadores, "Final");

        cout << "\n🏆 ¡Campeón del torneo: "
             << ganadores[0].getName().toStdString()
             << "!\n" << endl;

        cout << "Iniciando torneo gráfico en la ventana..." << endl;
    }

    QApplication a(argc, argv);
    MainWindow w(teams); //aqui se deberian entregar los equipos necesarios
    w.show();
    return a.exec();

}

