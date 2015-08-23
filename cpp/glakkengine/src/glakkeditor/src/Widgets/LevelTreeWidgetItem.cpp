/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Widgets/LevelTreeWidgetItem.hpp"

using kke::LevelTreeWidgetItem;
using std::string;

LevelTreeWidgetItem::LevelTreeWidgetItem(void* levelItem, const string& itemType, int type) :
        QTreeWidgetItem(type)
{
    this->levelItem = levelItem;
    this->itemType = itemType;
}

void* LevelTreeWidgetItem::GetLevelItem() const
{
    return levelItem;
}

const string& LevelTreeWidgetItem::GetItemType() const
{
    return itemType;
}
