#include "cleaner.h"
#include "ui_cleaner.h"

Cleaner::Cleaner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cleaner),
    m_settings("ananas", "Cleaner")
{
    ui->setupUi(this);
    m_pPaths = new QStringList;

    //inicialization Button and Filter
    QDir dir;
    QFileInfoList fileList = dir.drives();
    ui->m_lblCurDir->setText(dir.absoluteFilePath(fileList.at(0).filePath()));
    ui->m_lblCurDir->setToolTip("Current folder");

    ui->m_lineEditMask->setText("*.txt, *.dat");
    ui->m_lineEditMask->setToolTip("Mask:[*.txt, *.dat]");

    ui->m_btnDel->setEnabled(false);
    ui->m_btnDel->setIcon(QIcon(":/img/Fire.png"));
    ui->m_btnFind->setIcon(QIcon(":/img/folder.png"));
    ui->m_btnOpen->setIcon(QIcon(":/img/folder_start.png"));
    ui->m_btnQuit->setIcon(QIcon(":/img/exit.png"));

    QBrush brush;
    brush.setTextureImage(QImage(":/img/base.png"));
    QPalette pal;
    pal.setBrush(QPalette::Base, brush);
    ui->m_plTxtEditPath->setPalette(pal);
    ui->m_lineEditMask->setPalette(pal);

    connect(ui->m_btnFind, SIGNAL(clicked()), SLOT(slotFind()));
    connect(ui->m_btnDel, SIGNAL(clicked()), SLOT(slotDelete()));
    connect(ui->m_btnOpen,SIGNAL(clicked()), SLOT(slotOpenDir()));

    readSettings();
}

Cleaner::~Cleaner()
{
    writeSettings();
    delete ui;
}

void Cleaner::start(const QDir &dir)
{
    QApplication::processEvents();

    QStringList listFiles = dir.entryList(ui->m_lineEditMask->text().split(", "), QDir::Files | QDir::Dirs);
    foreach(QString fileName, listFiles)
    {
        m_pPaths->push_back(dir.absoluteFilePath(fileName));
        ui->m_plTxtEditPath->appendPlainText(dir.absoluteFilePath(fileName));
    }

    QStringList listDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString dirName, listDirs)
        start(QDir(dir.absoluteFilePath(dirName)));
}

void Cleaner::toolsOnOff(bool bOk)
{
    //OFF or ON all tools
    ui->m_btnDel->setEnabled(bOk);
    ui->m_btnFind->setEnabled(bOk);
    ui->m_lineEditMask->setEnabled(bOk);
    ui->m_btnOpen->setEnabled(bOk);
    ui->m_btnQuit->setEnabled(bOk);
}

void Cleaner::clearPaths()
{
    if(!m_pPaths->empty())
        m_pPaths->clear();
    ui->m_btnDel->setEnabled(false);

    ui->m_plTxtEditPath->clear();
}

void Cleaner::slotFind()
{
    toolsOnOff(false);
    this->setCursor(Qt::WaitCursor);
    ui->m_lblResult->setText("searching");
    clearPaths();
    QDir dir(ui->m_lblCurDir->text());
    start(dir);

    toolsOnOff(true);
    ui->m_lblResult->setText("ready to delete");
    if(m_pPaths->empty())
    {
        ui->m_lblResult->setText("file not found");
        ui->m_btnDel->setEnabled(false);
    }
    this->setCursor(Qt::ArrowCursor);
}

void Cleaner::slotDelete()
{
    toolsOnOff(false);
    this->setCursor(Qt::WaitCursor);
    ui->m_lblResult->setText("deleting...");
    foreach(QString path, *m_pPaths)
    {
        if(!QFile::remove(path))
        {
            QDir curDir(path);
            if(!curDir.removeRecursively())
                QMessageBox::information(this, "Error!"," File: " + path + " is not possible to remove!");
        }
    }
    toolsOnOff(true);
    clearPaths();
    ui->m_lblResult->setText("deletion comlited");
    this->setCursor(Qt::ArrowCursor);
}

void Cleaner::slotOpenDir()
{
    QString startPath = QFileDialog::getExistingDirectory
                             (nullptr,
                              "Choose the start folder",
                              ui->m_lblCurDir->text(),
                              QFileDialog::ShowDirsOnly);

    if(!startPath.isEmpty())
        ui->m_lblCurDir->setText(startPath);
}

void Cleaner::writeSettings()
{
    m_settings.beginGroup("/Settings");

    m_settings.setValue("/mask", ui->m_lineEditMask->text());
    m_settings.setValue("/width", width());
    m_settings.setValue("height", height());
    m_settings.setValue("/startDir", ui->m_lblCurDir->text());

    m_settings.endGroup();
}

void Cleaner::readSettings()
{
    m_settings.beginGroup("/Settings");

    QString strMask   = m_settings.value("/mask", "*.txt, *.dat").toString();
    int nWidth        = m_settings.value("/width", width()).toInt();
    int nHeight       = m_settings.value("/height", height()).toInt();
    QString strCurDir = m_settings.value("/startDir", ui->m_lblCurDir->text()).toString();

    resize(nWidth, nHeight);

    ui->m_lineEditMask->setText(strMask);
    ui->m_lblCurDir->setText(strCurDir);

    m_settings.endGroup();
}
