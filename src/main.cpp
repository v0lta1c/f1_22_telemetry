#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    
    MainWindow mainWindow;
    mainWindow.show();                    // Show the window 

    return app.exec();                // Run the app
}
