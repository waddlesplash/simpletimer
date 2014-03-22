/*
 * Copyright (c) 2009-2014 waddlesplash
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "TimerWind.h"
#include "ui_TimerWind.h"

#include <QMessageBox>
#include <QDesktopWidget>
#include <QWhatsThis>

TimerWind::TimerWind(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimerWind)
{
    ui->setupUi(this);

    countingUp = false;
    showWarn = true;
    didDoOvrTenMbox = false;
    didDoOvrFiftMbox = false;
    didDoOvrFiveMbox = false;
    didDoFiveMbox = false;
    didDo30sMbox = false;
    origMins = 1;
    minLeft = 1;
    secLeft = 0;

    if(style()->styleHint(QStyle::SH_DialogButtonBox_ButtonsHaveIcons)) {
        ui->startBtn->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
        ui->pauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        ui->resetBtn->setIcon(style()->standardIcon(QStyle::SP_DialogResetButton));
    }
}

TimerWind::~TimerWind()
{
    delete ui;
}


void TimerWind::modalMsgBox(QString tagline, QString text, bool critical)
{
    QDesktopWidget *desktop = QApplication::desktop();
    QMessageBox *msg = new QMessageBox(desktop);
    msg->setWindowTitle(tr("SimpleTimer"));
    msg->setInformativeText(text);
    msg->setText("<big><b>" + tagline + "</b></big>");
    msg->setWindowFlags(msg->windowFlags() | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

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
    msg->show();
}

void TimerWind::updateTimeLabel()
{
    if(secLeft < 10) {
        ui->timeLbl->setText(QString("%1 : 0%2").arg(minLeft).arg(secLeft));
    } else {
        ui->timeLbl->setText(QString("%1 : %2").arg(minLeft).arg(secLeft));
    }
}

bool TimerWind::event(QEvent *e)
{
    if(e->type() == QEvent::EnterWhatsThisMode) {
        QWhatsThis::leaveWhatsThisMode();
        e->accept();
        QMessageBox *msg = new QMessageBox(this);
        msg->setWindowTitle(tr("About SimpleTimer"));
        msg->setText(tr("<b>SimpleTimer</b>"));
        msg->setInformativeText(tr("<i>Version 1.3</i><br>&copy; 2009-2014 waddlesplash<br><a href=\"https://github.com/waddlesplash/simpletimer\">View on GitHub</a>"));
        msg->setIconPixmap(windowIcon().pixmap(32, 32));
        msg->exec();
        return true;
    } else {
        return QDialog::event(e);
    }
}

void TimerWind::on_startBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    setWindowTitle(tr("Timing"));
    origMins = ui->minSelect->value();
    minLeft = origMins;
    updateTimeLabel();
    showWarn = !ui->showWarnChk->isChecked();

    triggerTimer.setInterval(1000);
    triggerTimer.setSingleShot(false);
    connect(&triggerTimer, SIGNAL(timeout()), this, SLOT(time()));
    triggerTimer.start();
    accurateTimer.start();

#ifdef Q_OS_WIN
    taskbarButton = new QWinTaskbarButton(this);
    taskbarButton->setWindow(windowHandle());
    taskbarProgress = taskbarButton->progress();
    taskbarProgress->show();
    taskbarProgress->resume();
    taskbarProgress->setValue(100);
#endif
}

void TimerWind::time()
{
    int minPassed = 0;
    int secPassed = accurateTimer.restart()/1000;

    /* Compute minPassed */
    while(secPassed >= 60)  {
        minPassed += 1;
        secPassed -= 60;
    }

    if(!countingUp) {
        minLeft -= minPassed;
        secLeft -= secPassed;
        if(minLeft <= 0 && secLeft <= 0) {
            modalMsgBox(tr("Time's up!"), tr("Your time is up!"), true);
            countingUp = true;
            if(minLeft < 0) { minLeft *= -1; }
            if(secLeft < 0) { secLeft *= -1; }

            setStyleSheet("background-color: rgb(255, 0, 0);");

#ifdef Q_OS_WIN
            taskbarProgress->stop();
            taskbarProgress->setValue(100);
#endif
        }
        /* Get rid of any negatives on the seconds */
        while(secLeft < 0) {
            minLeft -= 1;
            secLeft += 60;
        }

        if(showWarn) {
            if(minLeft <= 4 && origMins >= 5 && !didDoFiveMbox) {
                didDoFiveMbox = true;
                modalMsgBox(tr("5 minutes left!"), tr("You have 5 minutes left."));
            } else if(minLeft <= 0 && secLeft <= 30 && origMins > 0 && !didDo30sMbox) {
                didDo30sMbox = true;
                modalMsgBox(tr("30 seconds left!"),tr("You have 30 seconds left."));
            }
        }

    } else if(countingUp) {
        minLeft += minPassed;
        secLeft += secPassed;
        if(secLeft >= 60) {
            minLeft += 1;
            secLeft -= 60;
        }
        if(minLeft >= 5 && !didDoOvrFiveMbox) {
            didDoOvrFiveMbox = true;
            modalMsgBox(tr("5 minutes overtime!"), tr("You are 5 minutes overtime!"), true);
        } else if(minLeft >= 10 && !didDoOvrTenMbox) {
            didDoOvrTenMbox = true;
            modalMsgBox(tr("10 minutes overtime!"), tr("You are 10 minutes overtime!"), true);
        } else if(minLeft >= 15 && !didDoOvrFiftMbox) {
            didDoOvrFiftMbox = true;
            modalMsgBox(tr("15 minutes overtime!"), tr("You are 15 minutes overtime!"), true);
        }
    }

#ifdef Q_OS_WIN
    if(!countingUp) {
        int percent = ((minLeft+(secLeft/60.0))/origMins)*100;
        taskbarProgress->setValue(percent);
    }
#endif
    updateTimeLabel();
}

void TimerWind::on_pauseBtn_clicked()
{
    if(triggerTimer.isActive()) {
        triggerTimer.stop();
        ui->pauseBtn->setText(tr("Resume"));

#ifdef Q_OS_WIN
        taskbarProgress->resume();
        taskbarProgress->pause();
#endif
    } else {
        triggerTimer.start();
        accurateTimer.restart();
        ui->pauseBtn->setText(tr("Pause"));

#ifdef Q_OS_WIN
        if(countingUp) {
            taskbarProgress->stop();
        } else {
            taskbarProgress->resume();
        }
#endif
    }
}

void TimerWind::on_resetBtn_clicked()
{
    triggerTimer.stop();
    minLeft = origMins;
    secLeft = 0;

    setStyleSheet("");
    ui->pauseBtn->setText(tr("Pause"));
    updateTimeLabel();

    countingUp = false;
    didDoOvrTenMbox = false;
    didDoOvrFiftMbox = false;
    didDoOvrFiveMbox = false;
    didDoFiveMbox = false;
    didDo30sMbox = false;

    triggerTimer.start();
    accurateTimer.restart();

#ifdef Q_OS_WIN
    taskbarProgress->resume();
    taskbarProgress->setValue(100);
#endif
}
