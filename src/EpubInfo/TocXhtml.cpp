/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Virgo.
**
**  Virgo is Epub Parser.
**
*************************************************************************/

#include "TocXhtml.h"

#include <QtDebug>

TocXhtml::TocXhtml(QObject *parent) :
	QObject(parent)
{
    
}

TocXhtml::~TocXhtml()
{
    
}

NavItem* TocXhtml::GetNavItem(QString type)
{
	foreach(NavItem* navItem, m_NavItemList) {
		if (navItem->GetAttrType() == type) {
			return navItem;
		}
	}

	return NULL;
}

bool TocXhtml::SetNavItem(QString type, QString id, QString title)
{
	NavItem* navItem = new NavItem();
	if (navItem) {
		navItem->SetAttrType(type);
		navItem->SetAttrID(id);
		navItem->SetTitle(title);
		m_NavItemList.append(navItem);
		return true;
	}

	return false;
}

/************************************************************************
**
**  Class NavItem
**
*************************************************************************/

NavItem::NavItem(QObject *parent) :
	QObject(parent)
{

}

NavItem::~NavItem()
{

}

bool NavItem::SetTocXHtmlItem(QString title, QString src)
{
	TocXHtmlItem* tocItem = new TocXHtmlItem();
	if (tocItem) {
		tocItem->SetTitle(title);
		tocItem->SetAttrHref(src);
		m_TocHtmlItemList.append(tocItem);
		return true;
	}

	return false;
}


/************************************************************************
**
**  Class TocXHtmlItem
**
*************************************************************************/

TocXHtmlItem::TocXHtmlItem(QObject *parent) :
	QObject(parent)
{

}

TocXHtmlItem::~TocXHtmlItem()
{

}