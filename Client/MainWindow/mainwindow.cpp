//
// Created by pozni on 1/21/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <QFileDialog>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include "mainwindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->BSend->setEnabled(false);
    ui->lineEdit->setPlaceholderText("Enter your message here or press 'Open file' to select a file...");

    connect(ui->BConnec, &QPushButton::clicked, this, &MainWindow::ConnectToServer);
    connect(ui->BSend, &QPushButton::clicked, this, &MainWindow::SendMessageToServer);
    connect(&socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(&socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
        this, &MainWindow::onSocketError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onConnected()
{
    ui->textBrowser->append("Connected to the server!");
    ui->BSend->setEnabled(true);
}


void MainWindow::onReadyRead()
{
    QByteArray data = socket.readAll();
    int digitToString = 100;

    //PARSE JSON
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QString totalWords = QString::number(obj["totalWords"].toInt());
    QString uniqueWords = QString::number(obj["uniqueWords"].toInt());
    QString sequenceOfUniqueWords = QString::number(obj["sequenceOfUniqueWords"].toInt());
    //VIEW
    ui->textBrowser->append(
        "\n\n------------------------------------\n"
        "Total words:      " + totalWords + "\n" +
        "Unique words:     " + uniqueWords + "\n" +
        "Sequence of unique words: " + sequenceOfUniqueWords);
}

void MainWindow::onSocketError(QAbstractSocket::SocketError socketError)
{
}

void MainWindow::ConnectToServer()
{
    QString ip = "127.0.0.1";
    quint16 port = 2525;

    socket.connectToHost(ip, port);
    if (!socket.waitForConnected(3000))
    {
        qWarning() << "Connection failed!";
        ui->textBrowser->append("Connection failed!");
    }
    qDebug() << "Connected!";
}

void MainWindow::DisconnectFromServer()
{
}

void MainWindow::SendMessageToServer()
{
    QString message = ui->lineEdit->text();
    if (message.isEmpty())
    {
        QByteArray data = fileText.toUtf8();
        socket.write(data);
    }
    else
    {
        QByteArray data = message.toUtf8();
        socket.write(data);
    }

    if (!socket.waitForBytesWritten(2000))
    {
        qWarning() << "Message not sent!";
        return;
    }
    qDebug() << "Message sent!";
}

void MainWindow::OpenFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open file", QDir::homePath());
    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            fileText = stream.readAll();
            file.close();
        }
    }
}
