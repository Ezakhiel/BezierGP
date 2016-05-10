#include <QApplication>
#include "GUI/MainWindow.h"

#include "Core/RealSquareMatrices.cpp"

using namespace std;
using namespace cagd;

int main(int argc, char** argv)
{
    // creating an application object and setting its style
    QApplication app(argc, argv);
    app.setStyle("Cleanlooks");

    // creating a main window object
    MainWindow mwnd;
    mwnd.showMaximized();

    // running the application
    return app.exec();
}
