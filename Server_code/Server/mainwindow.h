#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTextEdit>
#include <QHostAddress>
#include <QPushButton>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewConnection();
    void onTcpReadyRead();
    void onUdpReadyRead();
    void sendGetTemp();
    void askSetThreshold();

private:
    Ui::MainWindow *ui;

    // network objects
    QTcpServer *tcpServer{nullptr};
    QTcpSocket *clientTcpSocket{nullptr};
    QUdpSocket *udpSocket{nullptr};
    QHostAddress udpPeer;
    quint16 udpPeerPort{0};

    QTextEdit *logWidget{nullptr};
    int threshold{30};
    QTimer *pollTimer{nullptr};
};
#endif // MAINWINDOW_H
