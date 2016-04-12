#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtGui>
#include <QMainWindow>
#include <QFile>
#include <QButtonGroup>
#include <QPushButton>
#include <QMediaPlaylist>
#include <QDebug>

#include "playback.h"
#include "recording.h"
#include "playlist.h"
#include "socket/clientudp.h"
#include "socket/udprecvthread.h"
#include "socket/threadhandler.h"
#include "socket/circularbuffer.h"
#include "wavfile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void toggleIcon();

private slots:

    void on_actionPlaylist_triggered();

    void on_actionJoin_Multicast_triggered();

    void on_actionCB_triggered();

    void on_actionCB_Add_triggered();

private:
    Ui::MainWindow *ui;
    int icon;
    Playback *test;
    Recording *test2;
    Playlist *list;
    bool playPauseFlag = false;
    QString fname = QString(":/qss_icons/rc/play-circle1.png");
    QString fname2 = QString(":/qss_icons/rc/pause-circle.png");
    QString fname3 = QString(":/qss_icons/rc/stopButton.png");

    WavFile *wf;
};

#endif // MAINWINDOW_H
