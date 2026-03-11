#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // build minimal UI dynamically
    logWidget = new QTextEdit(this);
    logWidget->setReadOnly(true);
    QPushButton *btnGet = new QPushButton(tr("Get Temp"), this);
    QPushButton *btnSet = new QPushButton(tr("Set Threshold"), this);

    QVBoxLayout *layout = new QVBoxLayout(ui->centralwidget);
    layout->addWidget(logWidget);
    layout->addWidget(btnGet);
    layout->addWidget(btnSet);
    ui->centralwidget->setLayout(layout);

    connect(btnGet, &QPushButton::clicked, this, &MainWindow::sendGetTemp);
    connect(btnSet, &QPushButton::clicked, this, &MainWindow::askSetThreshold);

    // start TCP server
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::onNewConnection);
    if (!tcpServer->listen(QHostAddress::Any, 8080)) {
        logWidget->append(tr("TCP server failed to start: %1").arg(tcpServer->errorString()));
    } else {
        logWidget->append(tr("TCP server listening on port 8080"));
    }

    // start UDP socket
    udpSocket = new QUdpSocket(this);
    if (udpSocket->bind(8080)) {
        logWidget->append(tr("UDP socket bound on port 8080"));
        connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::onUdpReadyRead);
    } else {
        logWidget->append(tr("UDP bind failed: %1").arg(udpSocket->errorString()));
    }

    // periodic polling: ask client for temperature every second
    pollTimer = new QTimer(this);
    connect(pollTimer, &QTimer::timeout, this, &MainWindow::sendGetTemp);
    pollTimer->start(1000);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewConnection()
{
    clientTcpSocket = tcpServer->nextPendingConnection();
    logWidget->append(tr("New TCP client connected"));
    QByteArray msg = QByteArray::number(threshold);
    clientTcpSocket->write(msg);
    connect(clientTcpSocket, &QTcpSocket::readyRead, this, &MainWindow::onTcpReadyRead);
}

void MainWindow::onTcpReadyRead()
{
    if (!clientTcpSocket)
        return;
    QByteArray data = clientTcpSocket->readAll();
    logWidget->append(tr("Received (TCP): %1").arg(QString::fromUtf8(data)));
}

void MainWindow::onUdpReadyRead()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        udpPeer = sender;
        udpPeerPort = senderPort;
        logWidget->append(tr("Received (UDP) from %1:%2 - %3")
                          .arg(sender.toString()).arg(senderPort).arg(QString::fromUtf8(datagram)));
        // reply initial threshold or echo value
        QByteArray reply = QByteArray::number(threshold);
        udpSocket->writeDatagram(reply, sender, senderPort);
    }
}

void MainWindow::sendGetTemp()
{
    if (clientTcpSocket && clientTcpSocket->state() == QTcpSocket::ConnectedState) {
        clientTcpSocket->write("get temp");
        logWidget->append(tr("Sent 'get temp' to TCP client"));
    }
    if (!udpPeer.isNull()) {
        udpSocket->writeDatagram("get temp", udpPeer, udpPeerPort);
        logWidget->append(tr("Sent 'get temp' to UDP client %1:%2").arg(udpPeer.toString()).arg(udpPeerPort));
    }
}

void MainWindow::askSetThreshold()
{
    bool ok;
    int newTh = QInputDialog::getInt(this, tr("Set Threshold"), tr("Threshold:"), threshold, 0, 1000, 1, &ok);
    if (!ok)
        return;
    threshold = newTh;
    logWidget->append(tr("Threshold updated to %1").arg(threshold));
    QString cmd = QString("set threshold %1").arg(threshold);
    if (clientTcpSocket && clientTcpSocket->state() == QTcpSocket::ConnectedState) {
        clientTcpSocket->write(cmd.toUtf8());
        logWidget->append(tr("Sent '%1' to TCP client").arg(cmd));
    }
    if (!udpPeer.isNull()) {
        udpSocket->writeDatagram(cmd.toUtf8(), udpPeer, udpPeerPort);
        logWidget->append(tr("Sent '%1' to UDP client %2:%3").arg(cmd).arg(udpPeer.toString()).arg(udpPeerPort));
    }
}
