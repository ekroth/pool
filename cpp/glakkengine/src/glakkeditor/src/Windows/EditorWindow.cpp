/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Windows/EditorWindow.hpp"

#include "Widgets/MainGLWidget.hpp"
#include "ui_MainWindow.h"
#include "Widgets/LevelTreeWidgetItem.hpp"

#include <QtGui/QWidget>

using namespace kke;

EditorWindow::EditorWindow()
{
    mainWindow = new Ui_MainWindow();
    mainWindow->setupUi(this);
    
    // Setup TreeView item button events
    QObject::connect(mainWindow->buttonAddLayer, SIGNAL(clicked()), this, SLOT(addLayer()));
    QObject::connect(mainWindow->buttonProperty, SIGNAL(clicked()), this, SLOT(itemProperties()));
    QObject::connect(mainWindow->buttonMoveUp, SIGNAL(clicked()), this, SLOT(moveItemUp()));
    QObject::connect(mainWindow->buttonMoveDown, SIGNAL(clicked()), this, SLOT(moveItemDown()));
    QObject::connect(mainWindow->buttonCopy, SIGNAL(clicked()), this, SLOT(copyItem()));
    QObject::connect(mainWindow->buttonCut, SIGNAL(clicked()), this, SLOT(cutItem()));
    QObject::connect(mainWindow->buttonPaste, SIGNAL(clicked()), this, SLOT(pasteItem()));
    QObject::connect(mainWindow->buttonDel, SIGNAL(clicked()), this, SLOT(deleteItem()));
}

// LevelTreeWidget item buttons

void EditorWindow::addLayer()
{
    mainWindow->levelTreeWidget->addTopLevelItem(new LevelTreeWidgetItem(0, "Null"));
}

void EditorWindow::itemProperties()
{
  
}

void EditorWindow::moveItemUp()
{
    LevelTreeWidgetItem *item = (LevelTreeWidgetItem*)mainWindow->levelTreeWidget->currentItem();
    
    if (item->GetItemType() == "Layer")
    {
		// If item type is layer
	
		if (item != mainWindow->levelTreeWidget->topLevelItem(0) && 
		mainWindow->levelTreeWidget->topLevelItemCount() > 1)
		{
			// If it is not the top item, it can be swapped up
			LevelTreeWidgetItem* target = (LevelTreeWidgetItem*)mainWindow->levelTreeWidget->itemAbove(item);
			int targetIndex = mainWindow->levelTreeWidget->indexOfTopLevelItem(item);	    
			
			// Remove and place back in opposite order (swap)
			mainWindow->levelTreeWidget->removeItemWidget(item, 0);
			mainWindow->levelTreeWidget->removeItemWidget(target, 0);
			
			mainWindow->levelTreeWidget->insertTopLevelItem(targetIndex, target);
			mainWindow->levelTreeWidget->insertTopLevelItem(targetIndex, item);
			
			mainWindow->levelTreeWidget->setCurrentItem(item);
		}
    }
    else
    {
      
    }
}

void EditorWindow::moveItemDown()
{
	LevelTreeWidgetItem *item = (LevelTreeWidgetItem*)mainWindow->levelTreeWidget->currentItem();
    
    if (item->GetItemType() == "Layer")
    {
		// If item type is layer
		
		if (item != mainWindow->levelTreeWidget->topLevelItem(mainWindow->levelTreeWidget->topLevelItemCount() -1 ) && 
		mainWindow->levelTreeWidget->topLevelItemCount() > 1)
		{
			// If it is not the bottom item, it can be swapped down
			LevelTreeWidgetItem *target = (LevelTreeWidgetItem*)mainWindow->levelTreeWidget->itemBelow(item);
			int targetIndex = mainWindow->levelTreeWidget->indexOfTopLevelItem(item);	    
			
			// Remove and place back in opposite order (swap)
			mainWindow->levelTreeWidget->removeItemWidget(item, 0);
			mainWindow->levelTreeWidget->removeItemWidget(target, 0);
			
			mainWindow->levelTreeWidget->insertTopLevelItem(targetIndex, item);
			mainWindow->levelTreeWidget->insertTopLevelItem(targetIndex, target);
			
			mainWindow->levelTreeWidget->setCurrentItem(item);
		}
    }
    else
    {
      
    }
}

void EditorWindow::copyItem()
{
  
}

void EditorWindow::cutItem()
{
  
}

void EditorWindow::pasteItem()
{
  
}

void EditorWindow::deleteItem()
{
  
}

EditorWindow::~EditorWindow()
{
    delete mainWindow;
}
