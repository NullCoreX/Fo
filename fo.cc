#include <QApplication>
#include "FileOrganizer.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    FileOrganizer window;
    window.show();
    
    return app.exec();
}
