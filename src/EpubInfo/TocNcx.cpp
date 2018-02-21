/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Virgo.
**
**  Virgo is Epub Parser.
**
*************************************************************************/

#include "TocNcx.h"

#include <QtDebug>

TocNcx::TocNcx(QObject *parent) :
	QObject(parent)
{

}

TocNcx::~TocNcx()
{

}

TocNcxItem*	TocNcx::GetTocNcxItemByPID(QString id)
{
	foreach(TocNcxItem* tocItem, m_TocNcxItemList) {
		if (tocItem->GetID() == id) {
			return tocItem;
		}
	}

	return NULL;
}

TocNcxItem*	TocNcx::GetTocNcxItemByPlayOrder(QString playOrder)
{
	foreach(TocNcxItem* tocItem, m_TocNcxItemList) {
		if (tocItem->GetPlayOrder() == playOrder) {
			return tocItem;
		}
	}

	return NULL;
}


bool TocNcx::SetTocNcxItem(QString id, QString playOrder, QString title, QString src)
{
	TocNcxItem* tocItem = new TocNcxItem();
	if (tocItem) {
		tocItem->SetID(id);
		tocItem->SetPlayOrder(playOrder);
		tocItem->SetTitle(title);
		tocItem->SetAttrHref(src);
		m_TocNcxItemList.append(tocItem);
		return true;
	}

	return false;
}


/************************************************************************
**
**  Class TocNcxItem
**
*************************************************************************/

TocNcxItem::TocNcxItem(QObject *parent) :
	QObject(parent)
{

}

TocNcxItem::~TocNcxItem()
{

}