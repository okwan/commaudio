#include "MainWindow.h"
#include "ui_mainwindow.h"
bool udpConnected = false;
bool tcpConnected = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->playPauseButton->setIconSize(QSize(32, 32));
    ui->playPauseButton->setStyleSheet("QPushButton{border: none;outline: none;}");
    ui->playPauseButton->setIcon(QIcon(fname));

    ui->stopButton->setStyleSheet("QPushButton{border: none;outline: none;}");
    ui->stopButton->setIcon(QIcon(fname3));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playPauseButton_clicked(bool checked)
{
    if (!checked && udpConnected) {
        ui->playPauseButton->setIcon(QIcon(fname2));
        ui->playPauseButton->setCheckable(true);

        // QThread for sending
        udpSendWorkerThread = new QThread;
        udpSendWorker = new UDPSendWorker(serverUdp);
        udpSendWorker->moveToThread(udpSendWorkerThread);

        connect(udpSendWorkerThread, SIGNAL(started()), udpSendWorker, SLOT(Run()));
        connect(udpSendWorker, SIGNAL(SentData()), udpSendWorker, SLOT(deleteLater()));
        connect(udpSendWorkerThread, SIGNAL(finished()), udpSendWorkerThread, SLOT(deleteLater()));

        udpSendWorkerThread->start();

    } else {
        ui->playPauseButton->setIcon(QIcon(fname));
        // do Play stuff here
    }
}

void MainWindow::on_buttonTcpConnect_clicked(bool checked) {
    if (!checked && !tcpConnected) {
        ui->buttonTcpConnect->setCheckable(true);

        // TODO: get user input port
        int tcpPort = TCP_DEFAULT_PORT;

        // QThread for TCP control
        tcpControlWorkerThread = new QThread;
        tcpControlWorker = new TCPControlWorker;
        tcpControlWorker->moveToThread(tcpControlWorkerThread);

        // Start TCP control thread and connect signals & slots
        connect(tcpControlWorker, SIGNAL(SignalInitSocket(int)), tcpControlWorker, SLOT(InitSocket(int)));
        connect(tcpControlWorker, SIGNAL(AcceptedClient(int)), this, SLOT(HandleNewClient(int)));
        connect(tcpControlWorker, SIGNAL(ClosedSocket()), tcpControlWorker, SLOT(CloseSocket()));
        connect(tcpControlWorker, SIGNAL(finished()), tcpControlWorkerThread, SLOT(quit()));
        connect(tcpControlWorker, SIGNAL(finished()), tcpControlWorker, SLOT(deleteLater()));
        connect(tcpControlWorkerThread, SIGNAL(finished()), tcpControlWorkerThread, SLOT(deleteLater()));

        tcpControlWorkerThread->start();

        tcpConnected = true;
        emit tcpControlWorker->SignalInitSocket(tcpPort);
    } else if (checked && tcpConnected) {
        tcpControlWorker->finished();
        tcpControlWorker->CloseSocket();
        tcpConnected = false;
    }
}

void MainWindow::on_actionJoin_Multicast_triggered(bool checked) {
    if (!checked && !udpConnected) {
        ui->actionJoin_Multicast->setCheckable(true);
        // Setup UDP
        serverUdp = new ServerUDP();
        // TODO: get user input port
        if (!serverUdp->InitSocket(TIMECAST_PORT)) {
            qDebug() << "InitSocket error, closing socket";
            serverUdp->CloseSocket();
            return;
        } else if (!serverUdp->InitData()) {
            qDebug() << "InitData error, closing socket";
            serverUdp->CloseSocket();
            return;
        } else if(!serverUdp->MulticastSettings(TIMECAST_ADDR)) {
            qDebug() << "MulticastSettings error";
            serverUdp->CloseSocket();
            return;
        }
        udpConnected = true;
    } else if (checked && udpConnected){
        serverUdp->CloseSocket();
        udpConnected = false;
    }
}
