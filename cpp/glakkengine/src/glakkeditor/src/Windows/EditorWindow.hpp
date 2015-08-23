/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef EDITORWINDOW_HPP
#define EDITORWINDOW_HPP

#include <QtGui/QMainWindow>

class Ui_MainWindow;

namespace kke
{
    class MainGLWidget;

    class EditorWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        EditorWindow();
        virtual ~EditorWindow();

    public slots:
		void addLayer();
		void itemProperties();
		void moveItemUp();
		void moveItemDown();
		void copyItem();
		void cutItem();
		void pasteItem();
		void deleteItem();
	

    private:
        Ui_MainWindow *mainWindow;
    };
}

#endif // EDITORWINDOW_HPP
