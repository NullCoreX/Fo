#ifndef FILEORGANIZER_H
#define FILEORGANIZER_H

#include <QMainWindow>
#include <QThreadPool>
#include <QPointer>

QT_BEGIN_NAMESPACE
namespace Ui { class FO; }
QT_END_NAMESPACE

class FileOrganizer : public QMainWindow {
    Q_OBJECT

public:
    explicit FileOrganizer(QWidget *parent = nullptr);
    ~FileOrganizer();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onAllCheckboxToggled(bool checked);
    void onIndividualCheckboxToggled();
    void onSourceClicked();
    void onDestinationClicked();
    void onShowDirectory();
    void onCategory();
    void onProgressUpdate(int percent, int moved, int skipped, int total);
    void onTaskFinished(int moved, int skipped, int total);
    void onClear();
    void onExit();

private:
    void setupConnections();
    void setupUI();
    void updateCheckboxStates();

private:
    Ui::FO *ui;
    QThreadPool threadPool;
};

#endif // FILEORGANIZER_H
