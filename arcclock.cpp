#include <QtWidgets>
#include <QDebug>

#include "arcclock.h"
#include "prefs.h"

ArcClock::ArcClock(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint
              | Qt::WindowStaysOnBottomHint)
{


    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_X11NetWmWindowTypeDock);

    this->readSettings(true);

    if ((posX > 0) || (posY > 0))
        this->move(posX, posY);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);

    QAction *quitAction = new QAction(tr("E&xit"), this);
    quitAction->setShortcut(tr("Ctrl+Q"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    QAction *configAction = new QAction(tr("&Preferences"), this);
    configAction->setShortcut(tr("Ctrl+P"));
    connect(configAction, SIGNAL(triggered()), this, SLOT(onConfig()));

    addAction(configAction);
    addAction(quitAction);

    setContextMenuPolicy(Qt::ActionsContextMenu);
    setToolTip(tr("Drag the clock with the left mouse button.\n"
                  "Use the right mouse button to open a context menu."));
    setWindowTitle(tr("ArcClock"));
}

void ArcClock::closeEvent(QCloseEvent* event)
{
    this->writePosition();
    event->accept();
}

void ArcClock::writePosition()
{
    QSettings settings("ArcClock", "ArcClock");
    settings.setValue("posX", (this->frameGeometry().x() < 0) ? 1 : this->frameGeometry().x());
    settings.setValue("posY", (this->frameGeometry().y() < 0) ? 1 : this->frameGeometry().y());
    qApp->exit();
}

ArcClock::~ArcClock()
{
}

void ArcClock::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void ArcClock::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void ArcClock::paintEvent(QPaintEvent *)
{
    int side = qMin(width(), height());
    //QTime time = QTime( 9, 52, 59);
    QTime time = QTime::currentTime();

    QPainter painter(this);
    QPoint pp(side/2, side/2);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(dialColor));

    QFont font(textFont, side/6);
    QFontMetrics fm(font);
    font.setBold(true);
    painter.setFont(font);
    QString timeText = time.toString(timeFormat);
    painter.setPen(QColor("#30ffffff"));

    if(shadow==true){
        QRect rectShadow(2, (side / 2) - (fm.height() / 2) + 2, side, side / 2 + 1);
        painter.drawText(rectShadow, Qt::AlignHCenter, timeText);
    }
    QRect rect(0, (side / 2) - (fm.height() / 2), side, side / 2);
    painter.setPen(QColor(timeColor));
    painter.drawText(rect, Qt::AlignHCenter , timeText);


    if (showDate) {
        QRect rect2(0, side / 2 + fm.height() / 2 - 4 , side, side / 2);
        QFont font2(textFont, side/16);
        painter.setFont(font2);
        painter.setPen(QColor(dateColor));
        painter.drawText(rect2, Qt::AlignHCenter, QDate::currentDate().toString("d. MMMM"));
        QRect rect3(0, 0, side, (side / 2) - (fm.height() / 2) + 8);
        painter.drawText(rect3, Qt::AlignHCenter | Qt::AlignBottom, QDate::currentDate().toString("dddd"));
    }

    int twelve = (time.hour() > 12) ? time.hour() - 12 : time.hour();

    if(shadow==true){
        QPainter painter2Shadow(this);
        QPainterPath hourPathShadow;
        painter2Shadow.setRenderHint(QPainter::Antialiasing);
        QRect hourRectShadow(static_cast<int>(side / 7.0) + 1, static_cast<int>(side / 7.0) + 1,
                             static_cast<int>(side / 1.4) + 1, static_cast<int>(side / 1.4) + 1);
        hourPathShadow.arcMoveTo(hourRectShadow, 90.0);
        hourPathShadow.arcTo(hourRectShadow, 90.0, -30.0 * twelve - time.minute() / 2);
        QPen hourPenShadow;
        hourPenShadow.setWidth(side/32);
        hourPenShadow.setColor(QColor("#30ffffff"));
        hourPenShadow.setCapStyle(Qt::RoundCap);
        painter2Shadow.setPen(hourPenShadow);
        painter2Shadow.drawPath(hourPathShadow);
    }

    QPainter painter2(this);
    QPainterPath hourPath;
    painter2.setRenderHint(QPainter::Antialiasing);
    QRect hourRect(static_cast<int>(side / 7.0), static_cast<int>(side / 7.0),
                   static_cast<int>(side / 1.4), static_cast<int>(side / 1.4));
    hourPath.arcMoveTo(hourRect, 90.0);
    hourPath.arcTo(hourRect, 90.0, -30.0 * twelve - time.minute() / 2);
    QPen hourPen;
    hourPen.setWidth(side/32);
    hourPen.setColor(QColor(hourColor));
    hourPen.setCapStyle(Qt::RoundCap);
    painter2.setPen(hourPen);
    painter2.drawPath(hourPath);

    if(shadow == true)    {
        QPainter painter3Shadow(this);
        QPainterPath minutePathShadow;
        painter3Shadow.setRenderHint(QPainter::Antialiasing);
        QRect minuteRectShadow(static_cast<int>(side / 12.0) + 1, static_cast<int>(side / 12.0) + 1,
                               static_cast<int>(side / 1.2) + 1, static_cast<int>(side / 1.2) + 1);
        minutePathShadow.arcMoveTo(minuteRectShadow,90.0);
        minutePathShadow.arcTo(minuteRectShadow, 90.0, -6.0 * time.minute());
        QPen minutePenShadow;
        minutePenShadow.setWidth(side/35);
        minutePenShadow.setColor(QColor("#30ffffff"));
        minutePenShadow.setCapStyle(Qt::RoundCap);
        painter3Shadow.setPen(minutePenShadow);
        painter3Shadow.drawPath(minutePathShadow);
    }

    QPainter painter3(this);
    QPainterPath minutePath;
    painter3.setRenderHint(QPainter::Antialiasing);
    QRect minuteRect(static_cast<int>(side / 12.0), static_cast<int>(side / 12.0),
                     static_cast<int>(side / 1.2), static_cast<int>(side / 1.2));
    minutePath.arcMoveTo(minuteRect,90.0);
    minutePath.arcTo(minuteRect, 90.0, -6.0 * time.minute());
    QPen minutePen;
    minutePen.setWidth(side/35);
    minutePen.setColor(QColor(minuteColor));
    minutePen.setCapStyle(Qt::RoundCap);
    painter3.setPen(minutePen);
    painter3.drawPath(minutePath);

    QPainter painter4(this);
    QPainterPath secondPath;
    painter4.setRenderHint(QPainter::Antialiasing);
    QRect secondRect(static_cast<int>(side / 42.0), static_cast<int>(side / 42.0),
                     static_cast<int>(side / 1.05), static_cast<int>(side / 1.05));
    secondPath.arcMoveTo(secondRect,90.0);
    secondPath.arcTo(secondRect, 90.0, -6.0 * time.second());
    QPen secondPen;
    secondPen.setWidth(side/40);
    secondPen.setColor(QColor(secondColor));
    secondPen.setCapStyle(Qt::RoundCap);
    painter4.setPen(secondPen);
    painter4.drawPath(secondPath);

    if(shadow == true)    {
        QPainter painter4Shadow(this);
        QPainterPath secondPathShadow;
        painter4Shadow.setRenderHint(QPainter::Antialiasing);
        QRect secondRectShadow(static_cast<int>(side / 42.0) + 1, static_cast<int>(side / 42.0) + 1,
                               static_cast<int>(side / 1.05) + 1, static_cast<int>(side / 1.05) + 1);
        secondPathShadow.arcMoveTo(secondRectShadow,90.0);
        secondPathShadow.arcTo(secondRectShadow, 90.0, -6.0 * time.second());
        QPen secondPenShadow;
        secondPenShadow.setWidth(side/40);
        secondPenShadow.setColor(QColor("#30ffffff"));
        secondPenShadow.setCapStyle(Qt::RoundCap);
        painter4Shadow.setPen(secondPenShadow);
        painter4Shadow.drawPath(secondPathShadow);
    }
}

void ArcClock::resizeEvent(QResizeEvent * /* event */)
{
}

QSize ArcClock::sizeHint() const
{
    return QSize(initWidth, initHeight);
}

void ArcClock::initVars()
{
    QSettings settings("ArcClock", "ArcClock");

    settings.setValue("initWidth", 200);
    settings.setValue("initHeight", 200);
    settings.setValue("showDate", true);
    settings.setValue("hourColor", "#46000000");
    settings.setValue("minuteColor", "#64000000");
    settings.setValue("secondColor", "#ff0ff000");
    settings.setValue("timeColor", "#78000000");
    settings.setValue("dateColor", "#6e000000");
    settings.setValue("secondColor", "#6e000000");
    settings.setValue("shadow",false);
    settings.setValue("timeFormat", "hh:mm");
    settings.setValue("textFont", "URW Gothic L");
    settings.setValue("posX", 0);
    settings.setValue("posY", 0);
    settings.setValue("Existant", true);
}

void ArcClock::readSettings(bool startup)
{
    QSettings settings("ArcClock", "ArcClock");

    if (!settings.contains("Existant"))
        initVars();

    settings.sync();

    if (startup) {
        initWidth = settings.value("initWidth").toInt();
        initHeight = settings.value("initHeight").toInt();
        posX = 0;
        posY = 0;

        if (settings.value("posX").toInt() > -1)
            posX = settings.value("posX").toInt();

        if (settings.value("posY").toInt() > -1)
            posY = settings.value("posY").toInt();
    }
    shadow = settings.value("shadow").toBool();
    showDate    = settings.value("showDate").toBool();
    hourColor   = settings.value("hourColor").toString();
    minuteColor = settings.value("minuteColor").toString();
    timeColor   = settings.value("timeColor").toString();
    dateColor   = settings.value("dateColor").toString();
    timeFormat  = settings.value("timeFormat").toString();
    textFont    = settings.value("textFont").toString();
    secondColor = settings.value("secondColor").toString();
}

void ArcClock::onConfig(void)
{
    Prefs *p = new Prefs(this);
    connect(p, SIGNAL(updateSettings()), this, SLOT(prefsChanged()));
    p->show();
}

void ArcClock::prefsChanged()
{
    readSettings(false);
}
