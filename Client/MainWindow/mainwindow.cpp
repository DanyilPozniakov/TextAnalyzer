//
// Created by pozni on 1/21/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <QFileDialog>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QException>

#include "mainwindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Text Analyzer");

    ui->BSend->setEnabled(false);

    connect(ui->closeConnect, &QPushButton::clicked, this, &MainWindow::DisconnectFromServer);
    connect(ui->openFileButton, &QPushButton::clicked, this, &MainWindow::OpenFile);
    connect(ui->BConnec, &QPushButton::clicked, this, &MainWindow::ConnectToServer);
    connect(ui->BSend, &QPushButton::clicked, this, &MainWindow::SendMessageToServer);
    connect(&socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &MainWindow::onSocketError);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onReadyRead()
{
    QByteArray data = socket.readAll();

    //PARSE JSON
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(!doc.isObject())
    {
        qWarning() << "Invalid JSON";
        ui->textBrowser->append("Incoming data is not a JSON object!");
        return;
    }

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
    qWarning() << "Socket error: " << socketError;
    ui->textBrowser->append("Socket error: " + QString::number(socketError));
}

void MainWindow::ConnectToServer()
{
    QString ip = "127.0.0.1";
    quint16 port = 2525;

    if(socket.state() == QAbstractSocket::ConnectedState)
    {
        qWarning() << "Already connected!";
        ui->textBrowser->append("Already connected!");
        return;
    }

    socket.connectToHost(ip, port);
    if (!socket.waitForConnected(3000))
    {
        qWarning() << "Connection failed!";
        ui->textBrowser->append("Connection failed!");
    }
    qDebug() << "Connected!";

    ui->textBrowser->append("Connected to the server!");
    ui->BSend->setEnabled(true);
}

void MainWindow::DisconnectFromServer()
{
    if(socket.state() == QAbstractSocket::UnconnectedState)
    {
        qWarning() << "Already disconnected!";
        ui->textBrowser->append("Already disconnected!");
        return;
    }
    socket.disconnectFromHost();
    if(!socket.state() == QAbstractSocket::UnconnectedState)
    {
        if (!socket.waitForDisconnected(2000))
        {
            qWarning() << "Disconnection failed!";
            ui->textBrowser->append("Disconnection failed!");
            return;
        }
    }
    qDebug() << "Disconnected!";
}

void MainWindow::SendMessageToServer()
{
    if (fileText.isEmpty())
    {
        qWarning() << "File is empty!";
        ui->textBrowser->append("File is empty!");
        return;
    }
    socket.write(fileText.toUtf8());
    if (!socket.waitForBytesWritten(2000))
    {
        qWarning() << "Message not sent!";
        return;
    }
    qDebug() << "Message sent!";
}

void MainWindow::OpenFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open file",
                                                    QDir::homePath(), "All Files (*.*)"
    );
    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            fileText = in.readAll();
            ui->textBrowser->append("File uploaded!");
            file.close();
        }
    }
}
