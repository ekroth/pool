/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Windows/EditorWindow.hpp"


#include <QtGui/QApplication>

int main(int argc, char **argv)
{
	kke::Logger::SetLogLevel(LogLevel::Debug);
    QApplication App(argc, argv);    
    kke::EditorWindow window;
    window.show();

    return App.exec();
}
