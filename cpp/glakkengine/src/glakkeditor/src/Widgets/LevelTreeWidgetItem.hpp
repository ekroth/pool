/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LEVELTREEWIDGETITEM_HPP
#define KKE_LEVELTREEWIDGETITEM_HPP

#include <QtGui/QTreeWidget>
#include <string>

namespace kke
{
class LevelTreeWidgetItem : public QTreeWidgetItem
{
public:
	LevelTreeWidgetItem(void* levelItem, const std::string& itemType, int type = Type);

	void* GetLevelItem() const;
	const std::string& GetItemType() const;
private:
	void *levelItem;
	std::string itemType;
};
}

#endif // KKE_LEVELTREEWIDGETITEM_HPP
