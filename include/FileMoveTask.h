#ifndef FILEMOVETASK_H
#define FILEMOVETASK_H

#include <QRunnable>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QTextBrowser>

class FileMoveTask : public QRunnable {
public:
    FileMoveTask(const QString &sourcePath,
                 const QString &destBaseDir,
                 const QStringList &fileList,
                 QPointer<QTextBrowser> logWidget,
                 QPointer<QObject> signalReceiver,
                 const QString &currentDate);

    void run() override;

private:
    QString generateUniqueFilename(const QString &directory,
                                   const QString &date,
                                   const QString &extension,
                                   const QString &originalName);
    QString getYearFolder(const QString &baseDir, const QString &category,
                          const QString &filePath);
    QString generateLogFileName();

    void appendLog(const QString &msg);
    void writeLogFile(const QStringList &logEntries);
    void emitProgress(int percent, int moved, int skipped, int total);
    void emitFinished(int moved, int skipped, int total);

    QString sourcePath;
    QString destBaseDir;
    QStringList fileList;
    QPointer<QTextBrowser> logWidget;
    QPointer<QObject> signalReceiver;
    QString currentDate;
    int movedCount;
    int skippedCount;
    int total;
};

#endif // FILEMOVETASK_H
