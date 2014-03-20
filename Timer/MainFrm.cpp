/*
 * Copyright (c) 2009-2012 WaddleSplash
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
*/

#include "MainFrm.h"
#include "ui_MainFrm.h"

#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QDesktopWidget>

#ifdef Q_OS_WIN
#   include <QtWinExtras>
#endif

QTimer *timer;

bool countingUp = false;
bool showWarn = true;

bool didDoOvrTenMbox = false;
bool didDoOvrFiftMbox = false;
bool didDoOvrFiveMbox = false;
bool didDoFiveMbox = false;
bool didDo30sMbox = false;

int origMins = 1;

int minLeft = 1;
int secLeft = 0;

QDateTime timeStarted;

MainFrm::MainFrm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainFrm)
{
    ui->setupUi(this);
    ui->startBtn->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
}

MainFrm::~MainFrm()
{
    delete ui;
}

void MainFrm::on_startBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    setWindowTitle(tr("Timing"));
    origMins = ui->minSelect->value();
    minLeft = origMins;
    ui->timeLbl->setText(QString::number(origMins) + " : 00");
    showWarn = !ui->showWarnChk->isChecked();

    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(false);
    connect(timer,SIGNAL(timeout()),this,SLOT(time()));
    timer->start();
    timeStarted = QDateTime::currentDateTime();

#ifdef Q_OS_WIN
    taskbarButton = new QWinTaskbarButton(this);
    taskbarButton->setWindow(windowHandle());
    taskbarProgress = taskbarButton->progress();
    taskbarProgress->show();
    taskbarProgress->resume();
    taskbarProgress->setValue(100);
#endif
}

void MainFrm::on_aboutSTBtn_clicked()
{
    QMessageBox *msg = new QMessageBox(this);
    msg->setWindowTitle(tr("About SimpleTimer"));
    msg->setText(tr("<b>SimpleTimer</b>"));
    msg->setInformativeText(tr("<i>Version 1.3</i><br>&copy; 2009-2014 waddlesplash<br><a href=\"https://github.com/waddlesplash/simpletimer\">View on GitHub</a>"));
    msg->setIconPixmap(this->windowIcon().pixmap(32,32));
    msg->exec();
}

void MainFrm::time()
{
    int minPassed = 0;
    int secPassed = 0;
    QDateTime curTime;

    curTime = QDateTime::currentDateTime();
    secPassed = timeStarted.secsTo(curTime);
    timeStarted = timeStarted.addSecs(secPassed);

    // make sure that the seconds are less than 60
    while(secPassed >= 60)
    {
        minPassed += 1;
        secPassed -= 60;
    }
    if(!countingUp)
    {
        minLeft -= minPassed;
        secLeft -= secPassed;
        // Get rid of any negatives on the seconds
        while(secLeft <= -1)
        {
            minLeft -= 1;
            secLeft += 60;
        }
        if(((minLeft <= 0) && (secLeft <= 0)) || (minLeft <= -1))
        {
            timer->stop();
            modalMsgBox(tr("Time's up!"), tr("Your time is up!"), true);
            secLeft = 0;
            minLeft = 0;
            ui->timeLbl->setText(QString::number(minLeft) + " : " + QString::number(secLeft));
            countingUp = true;
#ifdef Q_OS_WIN
            taskbarProgress->stop();
            taskbarProgress->setValue(100);
#endif
            this->setStyleSheet("background-color: rgb(255,0,0);");
            ui->pauseBtn->setText(tr("Pause")); // How would it be possible to be otherwise anyway?
            timer->start();
            return;
        }
        if((minLeft <= 4) && (origMins >= 5) && (!didDoFiveMbox))
        {
            didDoFiveMbox = true;
            if(showWarn)
            { modalMsgBox(tr("5 minutes left!"), tr("You have 5 minutes left.")); }
        }
        if((minLeft <= 0) && (secLeft <= 30) && (!didDo30sMbox))
        {
            didDo30sMbox = true;
            if(showWarn)
            { modalMsgBox(tr("30 seconds left!"),tr("You have 30 seconds left.")); }
        }
        ui->timeLbl->setText(QString::number(minLeft) + " : " + QString::number(secLeft));
#ifdef Q_OS_WIN
        int percTot = ((minLeft+((secLeft/60.0)))/origMins)*100;
        taskbarProgress->setValue(percTot);
#endif
    }
    else if(countingUp)
    {
        minLeft += minPassed;
        secLeft += secPassed;
        // Get rid of any overflows on the seconds
        if(secLeft >= 60)
        {
            minLeft += 1;
            secLeft -= 60;
        }
        if((minLeft >= 5) && (!didDoOvrFiveMbox))
        {
            didDoOvrFiveMbox = true;
            modalMsgBox(tr("5 minutes overtime!"), tr("You are 5 minutes overtime!"), true);
        }
        if((minLeft >= 10) && (!didDoOvrTenMbox))
        {
            didDoOvrTenMbox = true;
            modalMsgBox(tr("10 minutes overtime!"), tr("You are 10 minutes overtime!"), true);
        }
        if((minLeft >= 15) && (!didDoOvrFiftMbox))
        {
            didDoOvrFiftMbox = true;
            modalMsgBox(tr("15 minutes overtime!"), tr("You are 15 minutes overtime!"), true);
        }
        ui->timeLbl->setText(QString::number(minLeft) + " : " + QString::number(secLeft));
    }
    if(secLeft <= 9)
    {
        ui->timeLbl->setText(QString::number(minLeft) + " : 0" + QString::number(secLeft));
    }
}

int MainFrm::modalMsgBox(QString tagline, QString text, bool critical)
{
    QDesktopWidget *desktop = QApplication::desktop();
    QMessageBox *msg = new QMessageBox(desktop);
    msg->setWindowTitle(tr("SimpleTimer"));
    msg->setInformativeText(tagline);
    msg->setText("<big><b>" + text + "</b></big>");
    msg->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    if(critical) {
        msg->setIcon(QMessageBox::Critical);
    } else {
        msg->setIcon(QMessageBox::Information);
    }

#ifdef Q_WS_X11
    /* Center window in current screen.
     *   From http://www.qtcentre.org/threads/14772-how-to-show-window-in-the-center-of-the-screen
     * Unnecessary for Windows because it automatically centers the window. */
    int screenWidth, width;
    int screenHeight, height;
    int x, y;
    QSize windowSize;
    screenWidth = desktop->width();
    screenHeight = desktop->height();
    windowSize = msg->size();
    width = windowSize.width();
    height = windowSize.height();
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    msg->setGeometry(x, y, msg->geometry().width(),msg->geometry().height());
#endif
    return msg->exec();
}

void MainFrm::on_pauseBtn_clicked()
{
    if(timer->isActive())
    {
        timer->stop();
        ui->pauseBtn->setText(tr("Resume"));
#ifdef Q_OS_WIN
        taskbarProgress->resume();
        taskbarProgress->pause();
#endif
    }
    else
    {
        timer->start();
        timeStarted = QDateTime::currentDateTime();
        ui->pauseBtn->setText(tr("Pause"));
#ifdef Q_OS_WIN
        if(countingUp)
        { taskbarProgress->stop(); }
        else
        { taskbarProgress->resume(); }
#endif
    }
}

void MainFrm::on_resetBtn_clicked()
{
    timer->stop();
    this->setStyleSheet("");
    minLeft = origMins;
    secLeft = 0;
    ui->pauseBtn->setText(tr("Pause"));
    ui->timeLbl->setText(QString::number(origMins) + " : 00");
    countingUp = false;
#ifdef Q_OS_WIN
    taskbarProgress->resume();
    taskbarProgress->setValue(100);
#endif
    timer->start();
    timeStarted = QDateTime::currentDateTime();
}
