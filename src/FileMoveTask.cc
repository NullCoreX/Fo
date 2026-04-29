#include "FileMoveTask.h"
#include <QRandomGenerator>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QMetaObject>
#include <QFile>
#include <QTextStream>
#include "CategoryManager.h"

FileMoveTask::FileMoveTask(const QString &sourcePath,
                           const QString &destBaseDir,
                           const QStringList &fileList,
                           QPointer<QTextBrowser> logWidget,
                           QPointer<QObject> signalReceiver,
                           const QString &currentDate)
    : sourcePath(sourcePath),
      destBaseDir(destBaseDir),
      fileList(fileList),
      logWidget(logWidget),
      signalReceiver(signalReceiver),
      currentDate(currentDate),
      movedCount(0),
      skippedCount(0),
      total(fileList.size())
{
    setAutoDelete(true);
}

void FileMoveTask::run() {
    QStringList logEntries;

    for (int i = 0; i < total; ++i) {
        const QString &fileName = fileList.at(i);
        QFileInfo fileInfo(sourcePath + "/" + fileName);

        if (!fileInfo.exists()) {
            QString msg = QString("⚠️ File not found: %1").arg(fileName);
            appendLog(msg);
            logEntries << msg;
            skippedCount++;
            continue;
        }

        QString category = CategoryManager::instance().getCategory(fileInfo.suffix());
        QString yearPath = getYearFolder(destBaseDir, category, fileInfo.absoluteFilePath());

        QString extension = fileInfo.suffix();
        QString baseName = fileInfo.completeBaseName();

        QString newFileName = generateUniqueFilename(yearPath, currentDate, extension, baseName);
        QString destPath = yearPath + "/" + newFileName;

        if (QFile::rename(fileInfo.absoluteFilePath(), destPath)) {
            QString msg = QString("✅ Moved: %1 → %2/%3/%4")
                              .arg(fileName)
                              .arg(category)
                              .arg(yearPath.section('/', -1))
                              .arg(newFileName);
            appendLog(msg);
            logEntries << msg;
            movedCount++;
        } else {
            QString msg = QString("❌ Failed to move: %1").arg(fileName);
            appendLog(msg);
            logEntries << msg;
            skippedCount++;
        }

        int progress = static_cast<int>((i + 1) * 100.0 / total);
        emitProgress(progress, movedCount, skippedCount, total);
    }

    writeLogFile(logEntries);

    emitFinished(movedCount, skippedCount, total);
}

QString FileMoveTask::generateUniqueFilename(const QString &directory,
                                             const QString &date,
                                             const QString &extension,
                                             const QString &originalName) {
    const QString chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int randomCodeLength = 12;

    QRandomGenerator *generator = QRandomGenerator::global();
    QString newFileName;
    bool unique = false;
    int attempts = 0;
    const int maxAttempts = 100;

    while (!unique && attempts < maxAttempts) {
        QString randomCode;
        for (int i = 0; i < randomCodeLength; ++i) {
            randomCode.append(chars.at(generator->bounded(chars.length())));
        }

        newFileName = QString("%1 - %2 - %3.%4")
                          .arg(date)
                          .arg(originalName)
                          .arg(randomCode)
                          .arg(extension);

        QFile checkFile(directory + "/" + newFileName);
        if (!checkFile.exists()) {
            unique = true;
        }
        attempts++;
    }

    if (!unique) {
        QString timestamp = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
        newFileName = QString("%1 - %2 - %3_%4.%5")
                          .arg(date)
                          .arg(originalName)
                          .arg(timestamp)
                          .arg(QRandomGenerator::global()->bounded(10000))
                          .arg(extension);
    }

    return newFileName;
}

QString FileMoveTask::getYearFolder(const QString &baseDir, const QString &category,
                                    const QString &filePath) {
    QFileInfo fileInfo(filePath);
    QDateTime lastModified = fileInfo.lastModified();
    QString year = lastModified.toString("yyyy");

    if (year.isEmpty() || year.toInt() < 2000 || year.toInt() > 2100) {
        year = QDate::currentDate().toString("yyyy");
    }

    QString yearPath = baseDir + "/" + category + "/" + year;
    QDir yearDir(yearPath);

    if (!yearDir.exists()) {
        yearDir.mkpath(".");
    }

    return yearPath;
}

QString FileMoveTask::generateLogFileName() {
    QDateTime now = QDateTime::currentDateTime();
    return QString("organize_log_%1.txt")
        .arg(now.toString("yyyy-MM-dd_HH-mm-ss"));
}

void FileMoveTask::appendLog(const QString &msg) {
    if (!logWidget) return;
    QMetaObject::invokeMethod(logWidget.data(), [logWidget = logWidget, msg]() {
        if (logWidget) logWidget->append(msg);
    }, Qt::QueuedConnection);
}

void FileMoveTask::writeLogFile(const QStringList &logEntries) {
    QString logFileName = generateLogFileName();
    QString logFilePath = destBaseDir + "/" + logFileName;

    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&logFile);

        stream << "═══════════════════════════════════════════\n";
        stream << "📋 File Organizer Pro - Organization Log\n";
        stream << "═══════════════════════════════════════════\n";
        stream << "📅 Date: " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n";
        stream << "📂 Source: " << sourcePath << "\n";
        stream << "🎯 Destination: " << destBaseDir << "\n";
        stream << "📊 Total files processed: " << total << "\n";
        stream << "✅ Successfully moved: " << movedCount << "\n";
        stream << "⚠️ Skipped/Failed: " << skippedCount << "\n";
        stream << "═══════════════════════════════════════════\n\n";

        stream << "📝 Detailed Operations:\n";
        stream << "───────────────────────────────────────────\n";
        for (const QString &entry : logEntries) {
            stream << entry << "\n";
        }

        stream << "\n═══════════════════════════════════════════\n";
        stream << "🏁 Operation completed at: "
               << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n";

        logFile.close();

        appendLog(QString("📝 Log file created: %1").arg(logFileName));
    } else {
        appendLog("❌ Failed to create log file!");
    }
}

void FileMoveTask::emitProgress(int percent, int moved, int skipped, int total) {
    if (!signalReceiver) return;
    QMetaObject::invokeMethod(signalReceiver.data(), "onProgressUpdate",
                              Qt::QueuedConnection,
                              Q_ARG(int, percent),
                              Q_ARG(int, moved),
                              Q_ARG(int, skipped),
                              Q_ARG(int, total));
}

void FileMoveTask::emitFinished(int moved, int skipped, int total) {
    if (!signalReceiver) return;
    QMetaObject::invokeMethod(signalReceiver.data(), "onTaskFinished",
                              Qt::QueuedConnection,
                              Q_ARG(int, moved),
                              Q_ARG(int, skipped),
                              Q_ARG(int, total));
}
