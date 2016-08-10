#ifndef CLEANER_H
#define CLEANER_H

#include <QWidget>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>

namespace Ui {
class Cleaner;
}

class Cleaner : public QWidget
{
    Q_OBJECT

public:
    explicit Cleaner(QWidget *parent = 0);
    ~Cleaner();

private:
    Ui::Cleaner *ui;
    QSettings m_settings;
    QStringList * m_pPaths;

    void start(const QDir & dir);
    void toolsOnOff(bool bOk);
    void clearPaths();

private slots:
    void slotFind();
    void slotDelete();
    void slotOpenDir();
    void writeSettings();
    void readSettings();
};

#endif // CLEANER_H
