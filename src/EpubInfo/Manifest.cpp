/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Virgo.
**
**  Virgo is Epub Parser.
**
*************************************************************************/

#include "Manifest.h"

#include <QtDebug>

Manifest::Manifest(QObject *parent) :
	QObject(parent)
	, m_TocNcxPath("")
	, m_TocXhtmlPath("")
{
	m_ManifestItemList.clear();
}

Manifest::~Manifest()
{
	m_ManifestItemList.clear();
}

ManifestItem* Manifest::GetItemByID(QString id)
{
	foreach(ManifestItem* manifestItem, m_ManifestItemList) {
		if (manifestItem->GetID() == id) {
			return manifestItem;
		}
	}

	return NULL;
}

ManifestItem* Manifest::GetItemByHref(QString href)
{
	foreach(ManifestItem* manifestItem, m_ManifestItemList) {
		if (manifestItem->GetHref() == href) {
			return manifestItem;
		}
	}

	return NULL;
}

QList<ManifestItem*> Manifest::GetItemListByMediaType(QString mediaType)
{
	QList<ManifestItem*> manifestItemList;
	manifestItemList.clear();

	foreach(ManifestItem* manifestItem, m_ManifestItemList) {
		if (manifestItem->GetMediaType() == mediaType) {
			manifestItemList.append(manifestItem);
		}
	}
	return manifestItemList;
}

QList<ManifestItem*> Manifest::GetItemListByProperties(QString properties)
{
	QList<ManifestItem*> manifestItemList;
	manifestItemList.clear();

	foreach(ManifestItem* manifestItem, m_ManifestItemList) {
		if (manifestItem->GetProperties() == properties) {
			manifestItemList.append(manifestItem);
		}
	}
	return manifestItemList;
}


bool Manifest::SetManifestItem(QString id, QString href, QString mediaType, QString properties)
{
	ManifestItem* manifestItem = new ManifestItem();
	if (manifestItem) {
		manifestItem->SetID(id);
		manifestItem->SetHref(href);
		manifestItem->SetMediaType(mediaType);
		manifestItem->SetProperties(properties);
		m_ManifestItemList.append(manifestItem);
		return true;
	}

	return false;
}

/************************************************************************
**
**  Class ManifestItem
**
*************************************************************************/

ManifestItem::ManifestItem(QObject *parent) :
	QObject(parent)
	, m_ItemID("")
	, m_Href("")
	, m_MediaType("")
	, m_Properties("")
{

}

ManifestItem::~ManifestItem()
{

}