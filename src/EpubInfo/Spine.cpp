/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Virgo.
**
**  Virgo is Epub Parser.
**
*************************************************************************/

#include "Spine.h"

#include <QtDebug>

Spine::Spine(QObject *parent) :
	QObject(parent)
	, m_TocInfo("")
{
	m_SpineItemList.clear();
}

Spine::~Spine()
{
	m_SpineItemList.clear();
}

SpineItem* Spine::GetSpineItemByIDRef(QString idref)
{
	foreach(SpineItem* spineItem, m_SpineItemList) {
		if (spineItem->GetIDRef() == idref) {
			return spineItem;
		}
	}

	return NULL;
}

bool Spine::SetSpineItem(QString idref, QString linear)
{
	SpineItem* spineItem = new SpineItem();
	if (spineItem) {
		spineItem->SetIDRef(idref);
		spineItem->SetLinear(linear);
		m_SpineItemList.append(spineItem);
		return true;
	}

	return false;
}

/************************************************************************
**
**  Class SpineItem
**
*************************************************************************/

SpineItem::SpineItem(QObject *parent) :
	QObject(parent)
	, m_IDRef("")
	, m_Linear("")
{

}

SpineItem::~SpineItem()
{

}