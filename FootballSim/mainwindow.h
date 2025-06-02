#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "team.h"
#include <QVector>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QVector<Team> equipos, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void recibirMensaje(const QString &msg); // slot para recibir mensajes

private:
    QVector<Team> equipos;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
