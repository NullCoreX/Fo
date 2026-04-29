#include "FileOrganizer.h"
#include "ui_fo.h"
#include "FileMoveTask.h"
#include "CategoryManager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QDate>
#include <QPointer>
#include <QEvent>
#include <QMouseEvent>

FileOrganizer::FileOrganizer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FO)
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
    threadPool.setMaxThreadCount(4);
}

FileOrganizer::~FileOrganizer() {
    threadPool.waitForDone();
    delete ui;
}

void FileOrganizer::setupUI() {
    this->setWindowTitle("File Organizer Pro");
    this->setFixedSize(592, 562);
    
    ui->Source->setPlaceholderText("🔍 Click or tap to select source folder...");
    ui->Destination->setPlaceholderText("🎯 Click or tap to select destination folder...");
    
    ui->Source->installEventFilter(this);
    ui->Destination->installEventFilter(this);
}

void FileOrganizer::setupConnections() {
    connect(ui->All, &QCheckBox::toggled, this, &FileOrganizer::onAllCheckboxToggled);
    connect(ui->Audio, &QCheckBox::toggled, this, &FileOrganizer::onIndividualCheckboxToggled);
    connect(ui->Images, &QCheckBox::toggled, this, &FileOrganizer::onIndividualCheckboxToggled);
    connect(ui->Videos, &QCheckBox::toggled, this, &FileOrganizer::onIndividualCheckboxToggled);
    connect(ui->Documents, &QCheckBox::toggled, this, &FileOrganizer::onIndividualCheckboxToggled);
    
    connect(ui->Show_Directory, &QPushButton::clicked, this, &FileOrganizer::onShowDirectory);
    connect(ui->Category, &QPushButton::clicked, this, &FileOrganizer::onCategory);
    connect(ui->Clear, &QPushButton::clicked, this, &FileOrganizer::onClear);
    connect(ui->Exit, &QPushButton::clicked, this, &FileOrganizer::onExit);
}

void FileOrganizer::onAllCheckboxToggled(bool checked) {
    ui->Audio->setChecked(checked);
    ui->Images->setChecked(checked);
    ui->Videos->setChecked(checked);
    ui->Documents->setChecked(checked);
}

void FileOrganizer::onIndividualCheckboxToggled() {
    updateCheckboxStates();
}

void FileOrganizer::updateCheckboxStates() {
    bool allChecked = ui->Audio->isChecked() &&
                      ui->Images->isChecked() &&
                      ui->Videos->isChecked() &&
                      ui->Documents->isChecked();
    ui->All->blockSignals(true);
    ui->All->setChecked(allChecked);
    ui->All->blockSignals(false);
}

void FileOrganizer::onSourceClicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Source Directory");
    if (!dir.isEmpty()) {
        ui->Source->setText(dir);
    }
}

void FileOrganizer::onDestinationClicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Destination Directory");
    if (!dir.isEmpty()) {
        ui->Destination->setText(dir);
    }
}

void FileOrganizer::onShowDirectory() {
    QString source = ui->Source->text();
    if (source.isEmpty()) {
        ui->Log->append("⚠️ Please select a source directory first!");
        return;
    }

    QDir directory(source);
    if (!directory.exists()) {
        ui->Log->append("❌ Directory does not exist: " + source);
        return;
    }

    ui->Log->append("📂 Contents of: " + source);
    ui->Log->append("----------------------------------------");

    QFileInfoList entries = directory.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    if (entries.isEmpty()) {
        ui->Log->append("📭 The directory is empty.");
    } else {
        int folderCount = 0, fileCount = 0;
        for (const QFileInfo &entry : entries) {
            if (entry.isDir()) {
                ui->Log->append("📁 " + entry.fileName());
                folderCount++;
            } else {
                ui->Log->append("📄 " + entry.fileName());
                fileCount++;
            }
        }
        ui->Log->append("----------------------------------------");
        ui->Log->append(QString("📊 Total: %1 folders, %2 files").arg(folderCount).arg(fileCount));
    }
}

void FileOrganizer::onCategory() {
    QString source = ui->Source->text();
    QString destination = ui->Destination->text();

    if (source.isEmpty() || destination.isEmpty()) {
        ui->Log->append("⚠️ Please select both source and destination directories!");
        return;
    }

    QStringList filters = CategoryManager::instance().getFiltersForCategories(
        ui->All->isChecked(),
        ui->Audio->isChecked(),
        ui->Images->isChecked(),
        ui->Videos->isChecked(),
        ui->Documents->isChecked()
    );

    if (filters.isEmpty()) {
        ui->Log->append("⚠️ Please select at least one format category!");
        return;
    }

    QDir sourceDir(source);
    QFileInfoList files = sourceDir.entryInfoList(filters, QDir::Files);

    if (files.isEmpty()) {
        ui->Log->append("ℹ️ No matching files found in source directory.");
        return;
    }

    QStringList fileNames;
    for (const QFileInfo &fi : files) {
        fileNames << fi.fileName();
    }

    ui->Category->setEnabled(false);
    ui->Log->append("🚀 Starting multi-threaded file organization...");

    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");

    FileMoveTask *task = new FileMoveTask(source, destination, fileNames,
                                          QPointer<QTextBrowser>(ui->Log),
                                          this, currentDate);
    threadPool.start(task);
}

void FileOrganizer::onProgressUpdate(int percent, int moved, int skipped, int total) {
    ui->Log->append(QString("📈 Progress: %1% (%2/%3 files moved, %4 skipped)")
                       .arg(percent).arg(moved).arg(total).arg(skipped));
}

void FileOrganizer::onTaskFinished(int moved, int skipped, int total) {
    ui->Log->append("----------------------------------------");
    ui->Log->append(QString("✅ Successfully moved: %1 files").arg(moved));
    if (skipped > 0) {
        ui->Log->append(QString("⚠️ Failed/Skipped: %1 files").arg(skipped));
    }
    ui->Log->append("🎉 Operation completed!");

    QMessageBox::information(this, "Operation Complete",
                             QString("Successfully moved %1 files to categorized folders.\n"
                                     "Files renamed with date and unique code.\n"
                                     "Skipped/Failed: %2")
                                 .arg(moved).arg(skipped));

    ui->Category->setEnabled(true);
}

void FileOrganizer::onClear() {
    ui->Log->clear();
    ui->Log->append("📝 Log cleared.");
}

void FileOrganizer::onExit() {
    threadPool.waitForDone();
    QApplication::quit();
}

bool FileOrganizer::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (obj == ui->Source) {
            onSourceClicked();
            return true;
        } else if (obj == ui->Destination) {
            onDestinationClicked();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
