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
#ifdef Q_OS_WIN
    m_taskbarInterface = NULL;

    // Compute the value for the TaskbarButtonCreated message
    m_IDTaskbarButtonCreated = RegisterWindowMessage(L"TaskbarButtonCreated");
#endif
}

MainFrm::~MainFrm()
{
    delete ui;
}

#ifdef Q_OS_WIN
bool MainFrm::winEvent(MSG * message, long * result)
{
    if (message->message == m_IDTaskbarButtonCreated) {
        initTaskbarButton();
    }
    return false;
}
#endif

void MainFrm::on_startBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    this->setWindowTitle(tr("Timing"));
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
    updateProgBarState(2);
    updateProgBarVal(100);
#endif
}

void MainFrm::on_aboutQtBtn_clicked()
{
    QApplication::aboutQt();
}

void MainFrm::on_aboutSTBtn_clicked()
{
    QMessageBox *msg = new QMessageBox(this);
    msg->setWindowTitle(tr("About SimpleTimer"));
#ifdef Q_OS_MAC
    msg->setText(tr("SimpleTimer"));
#else
    msg->setText(tr("<b><big>SimpleTimer</big></b>"));
#endif
    msg->setInformativeText(tr("<i>Version 1.3</i><br>© 2009-2012 WaddleSplash<br>http://bsimpletimer.sourceforge.net/"));
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
            stMsgBox(tr("Time's up!"),tr("Your time is up!"),tr("SimpleTimer"),true);
            secLeft = 0;
            minLeft = 0;
            ui->timeLbl->setText(QString::number(minLeft) + " : " + QString::number(secLeft));
            countingUp = true;
#ifdef Q_OS_WIN
            updateProgBarState(3);
            updateProgBarVal(100);
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
            { stMsgBox(tr("5 minutes left!"),tr("You have 5 minutes left."),tr("SimpleTimer")); }
        }
        if((minLeft <= 0) && (secLeft <= 30) && (!didDo30sMbox))
        {
            didDo30sMbox = true;
            if(showWarn)
            { stMsgBox(tr("30 seconds left!"),tr("You have 30 seconds left."),tr("SimpleTimer")); }
        }
        ui->timeLbl->setText(QString::number(minLeft) + " : " + QString::number(secLeft));
#ifdef Q_OS_WIN
        int percTot = ((minLeft+((secLeft/60.0)))/origMins)*100;
        updateProgBarVal(percTot);
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
            stMsgBox(tr("5 minutes overtime!!!"), tr("You are 5 minutes overtime!"), tr("Timer - Overtime!!!"), true);
        }
        if((minLeft >= 10) && (!didDoOvrTenMbox))
        {
            didDoOvrTenMbox = true;
            stMsgBox(tr("10 minutes overtime!!!"), tr("You are 10 minutes overtime!"), tr("Timer - Overtime!!!"), true);
        }
        if((minLeft >= 15) && (!didDoOvrFiftMbox))
        {
            didDoOvrFiftMbox = true;
            stMsgBox(tr("15 minutes overtime!!!"), tr("You are 15 minutes overtime!"), tr("Timer - Overtime!!!"), true);
        }
        ui->timeLbl->setText(QString::number(minLeft) + " : " + QString::number(secLeft));
    }
    if(secLeft <= 9)
    {
        ui->timeLbl->setText(QString::number(minLeft) + " : 0" + QString::number(secLeft));
    }
}

int MainFrm::stMsgBox(QString txt, QString infoTxt, QString windTitle, bool isCritical)
{
    QDesktopWidget *deskWidg = QApplication::desktop();
    QMessageBox *msg = new QMessageBox(deskWidg);
    msg->setWindowTitle(windTitle);
    msg->setInformativeText(infoTxt);
#ifdef Q_OS_MAC
    msg->setText(txt);
#else
    msg->setText("<big><b>" + txt + "</b></big>");
#endif
    msg->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    if(isCritical) { msg->setIcon(QMessageBox::Critical); }
    else { msg->setIcon(QMessageBox::Information); }
#ifdef Q_WS_X11
    // Center window -- example from http://www.qtcentre.org/threads/14772-how-to-show-window-in-the-center-of-the-screen
    // Unnecessary for Win because it automatically centers the window
    int screenWidth, width;
    int screenHeight, height;
    int x, y;
    QSize windowSize;
    screenWidth = deskWidg->width();
    screenHeight = deskWidg->height();
    windowSize = msg->size();
    width = windowSize.width();
    height = windowSize.height();
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    msg->setGeometry(x,y,msg->geometry().width(),msg->geometry().height());
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
        updateProgBarState(4);
#endif
    }
    else
    {
        timer->start();
        timeStarted = QDateTime::currentDateTime();
        ui->pauseBtn->setText(tr("Pause"));
#ifdef Q_OS_WIN
        if(countingUp)
        { updateProgBarState(3); }
        else
        { updateProgBarState(2); }
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
    updateProgBarState(2);
    updateProgBarVal(100);
#endif
    timer->start();
    timeStarted = QDateTime::currentDateTime();
}

#ifdef Q_OS_WIN
// From an online example of how to do progressbars in the taskbar via Qt
void MainFrm::updateProgBarState(int state) {
    if (m_taskbarInterface) {
        switch(state) {
        case 1:
            m_taskbarInterface->SetProgressState(winId(), TBPF_INDETERMINATE);
            break;

        case 2:
            m_taskbarInterface->SetProgressState(winId(), TBPF_NORMAL);
            break;

        case 3:
            m_taskbarInterface->SetProgressState(winId(), TBPF_ERROR);
            break;

        case 4:
            m_taskbarInterface->SetProgressState(winId(), TBPF_PAUSED);
            break;

        default:
            m_taskbarInterface->SetProgressState(winId(), TBPF_NOPROGRESS);
            break;
        }
    }

}

void MainFrm::updateProgBarVal(int value) {
    if (m_taskbarInterface) {
        m_taskbarInterface->SetProgressValue(winId(), value, 99);
    }
}

void MainFrm::initTaskbarButton() {
    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, reinterpret_cast<void**> (&(m_taskbarInterface)));

    if (SUCCEEDED(hr)) {
        hr = m_taskbarInterface->HrInit();

        if (FAILED(hr)) {
            m_taskbarInterface->Release();
            m_taskbarInterface = NULL;
        }
    }
}
#endif
