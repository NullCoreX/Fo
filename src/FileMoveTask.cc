#include "FileMoveTask.h"
#include <QRandomGenerator>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QMetaObject>
#include <QFile>
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
    for (int i = 0; i < total; ++i) {
        const QString &fileName = fileList.at(i);
        QFileInfo fileInfo(sourcePath + "/" + fileName);

        if (!fileInfo.exists()) {
            appendLog(QString("⚠️ File not found: %1").arg(fileName));
            skippedCount++;
            continue;
        }

        QString category = CategoryManager::instance().getCategory(fileInfo.suffix());
        QDir destDir(destBaseDir + "/" + category);

        if (!destDir.exists()) {
            if (!destDir.mkpath(".")) {
                appendLog(QString("❌ Failed to create directory: %1").arg(category));
                skippedCount++;
                continue;
            }
        }

        QString extension = fileInfo.suffix();
        QString newFileName = generateUniqueFilename(destDir.absolutePath(), currentDate, extension);
        QString destPath = destDir.filePath(newFileName);

        if (QFile::rename(fileInfo.absoluteFilePath(), destPath)) {
            appendLog(QString("✅ Moved: %1 → %2/%3")
                          .arg(fileName)
                          .arg(category)
                          .arg(newFileName));
            movedCount++;
        } else {
            appendLog(QString("❌ Failed to move: %1").arg(fileName));
            skippedCount++;
        }

        int progress = static_cast<int>((i + 1) * 100.0 / total);
        emitProgress(progress, movedCount, skippedCount, total);
    }

    emitFinished(movedCount, skippedCount, total);
}

QString FileMoveTask::generateUniqueFilename(const QString &directory, 
                                              const QString &date, 
                                              const QString &extension) {
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

        newFileName = QString("%1 - %2.%3").arg(date).arg(randomCode).arg(extension);
        QFile checkFile(directory + "/" + newFileName);
        if (!checkFile.exists()) {
            unique = true;
        }
        attempts++;
    }

    if (!unique) {
        QString timestamp = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
        newFileName = QString("%1 - %2_%3.%4")
                          .arg(date)
                          .arg(timestamp)
                          .arg(QRandomGenerator::global()->bounded(10000))
                          .arg(extension);
    }

    return newFileName;
}

void FileMoveTask::appendLog(const QString &msg) {
    if (!logWidget) return;
    QMetaObject::invokeMethod(logWidget.data(), [logWidget = logWidget, msg]() {
        if (logWidget) logWidget->append(msg);
    }, Qt::QueuedConnection);
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
