#include "prefs.h"
#include "ui_prefs.h"
#include "arcclock.h"
#include <QSettings>
#include <QColorDialog>
#include <QDebug>

Prefs::Prefs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prefs)
{
    ui->setupUi(this);

    QSettings settings("ArcClock", "ArcClock");

    m_datecolor = settings.value("dateColor").toString();
    m_timecolor = settings.value("timeColor").toString();
    m_hourcolor = settings.value("hourColor").toString();
    m_minutecolor = settings.value("minuteColor").toString();
    m_secondcolor = settings.value("secondColor").toString();

    if (settings.value("timeFormat").toString() == "hh:mm ap")
        ui->cb_timeFormat->setChecked(true);

    m_textfont = settings.value("textFont").toString();
    ui->fcb_time->setCurrentFont(QFont(m_textfont));

    QString dateColorStyle = QString("border: 1px solid black;background-color: %1;")
            .arg(m_datecolor);
    ui->lbl_dateColor->setStyleSheet(dateColorStyle);
    QString timeColorStyle = QString("border: 1px solid black;background-color: %1;")
            .arg(m_timecolor);
    ui->lbl_timeColor->setStyleSheet(timeColorStyle);
    QString hourColorStyle = QString("border: 1px solid black;background-color: %1;")
            .arg(m_hourcolor);
    ui->lbl_hourColor->setStyleSheet(hourColorStyle);
    QString minuteColorStyle = QString("border: 1px solid black;background-color: %1;")
            .arg(m_minutecolor);
    ui->lbl_minuteColor->setStyleSheet(minuteColorStyle);
    QString secondColorStyle = QString("border: 1px solid black;background-color: %1;")
            .arg(m_secondcolor);
    ui->lbl_secondColor->setStyleSheet(secondColorStyle);
    ui->cb_showDate->setChecked(settings.value("showDate").toBool());
    ui->cb_shadow->setChecked(settings.value("shadow").toBool());
    ui->sb_startSize->setValue(settings.value("initHeight").toInt());
}

Prefs::~Prefs()
{
    delete ui;
}


void Prefs::on_pb_apply_clicked()
{
    QSettings settings("ArcClock", "ArcClock");
    settings.setValue("showDate", ui->cb_showDate->isChecked());
    settings.setValue("initHeight", ui->sb_startSize->value());
    settings.setValue("initWidth", ui->sb_startSize->value());
    settings.setValue("timeColor", m_timecolor);
    settings.setValue("dateColor", m_datecolor);
    settings.setValue("hourColor", m_hourcolor);
    settings.setValue("minuteColor", m_minutecolor);
    settings.setValue("secondColor", m_secondcolor);
    settings.setValue("shadow",ui->cb_shadow->isChecked());

    if (ui->cb_timeFormat->isChecked()) settings.setValue("timeFormat", "hh:mm ap");
    else settings.setValue("timeFormat", "hh:mm");

    settings.setValue("textFont", ui->fcb_time->currentText());

    settings.sync();
    emit updateSettings();
}

void Prefs::on_tb_timeColor_clicked()
{
    QColor c = QColorDialog::getColor(m_timecolor, this, "Time Text", QColorDialog::ShowAlphaChannel);

    if (c.isValid()) {
        m_timecolor = c.name(QColor::HexArgb);
        QString timeColorStyle = QString("border: 1px solid black;background-color: %1;")
                .arg(m_timecolor);
        ui->lbl_timeColor->setStyleSheet(timeColorStyle);
    }
}

void Prefs::on_tb_dateColor_clicked()
{
    QColor c = QColorDialog::getColor(m_datecolor, this, "Date Text", QColorDialog::ShowAlphaChannel);

    if (c.isValid()) {
        m_datecolor = c.name(QColor::HexArgb);
        QString dateColorStyle = QString("border: 1px solid black;background-color: %1;")
                .arg(m_datecolor);
        ui->lbl_dateColor->setStyleSheet(dateColorStyle);
    }
}

void Prefs::on_tb_hourColor_clicked()
{
    QColor c = QColorDialog::getColor(m_hourcolor, this, "Hour Arc", QColorDialog::ShowAlphaChannel);

    if (c.isValid()) {
        m_hourcolor = c.name(QColor::HexArgb);
        QString hourColorStyle = QString("border: 1px solid black;background-color: %1;")
                .arg(m_hourcolor);
        ui->lbl_hourColor->setStyleSheet(hourColorStyle);
    }
}

void Prefs::on_tb_minuteColor_clicked()
{
    QColor c = QColorDialog::getColor(m_minutecolor, this, "Minute Arc", QColorDialog::ShowAlphaChannel);

    if (c.isValid()) {
        m_minutecolor = c.name(QColor::HexArgb);
        QString minuteColorStyle = QString("border: 1px solid black;background-color: %1;")
                .arg(m_minutecolor);
        ui->lbl_minuteColor->setStyleSheet(minuteColorStyle);
    }
}

void Prefs::on_tb_secondColor_clicked()
{
    QColor c = QColorDialog::getColor(m_secondcolor, this, "Second Arc", QColorDialog::ShowAlphaChannel);

    if (c.isValid()) {
        m_secondcolor = c.name(QColor::HexArgb);
        QString secondColorStyle = QString("border: 1px solid black;background-color: %1;")
                .arg(m_secondcolor);
        ui->lbl_secondColor->setStyleSheet(secondColorStyle);
    }
}

