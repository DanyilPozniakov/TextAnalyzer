//
// Created by pozni on 1/21/2025.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE

namespace Ui
{
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError socketError);

    void ConnectToServer();
    void DisconnectFromServer();
    void SendMessageToServer();
    void OpenFile();

private:
    Ui::MainWindow* ui;
    QString fileText;

    QTcpSocket socket;
};


#endif //MAINWINDOW_H
