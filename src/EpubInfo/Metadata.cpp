/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Virgo.
**
**  Virgo is Epub Parser.
**
*************************************************************************/

#include "Metadata.h"

#include <QtDebug>

Metadata::Metadata(QObject *parent) :
	QObject(parent)
	, m_Identifier("")
	, m_Title("")
	, m_Language("")
	, m_Contributor("")
	, m_Coverage("")
	, m_Creator("")
	, m_Date("")
	, m_Description("")
	, m_Format("")
	, m_Publisher("")
	, m_Relation("")
	, m_Rights("")
	, m_Source("")
	, m_Subject("")
	, m_Type("")
	, m_Rendition(new Rendition())
{
	m_MetaItemList.clear();
}

Metadata::~Metadata()
{
	if (m_Rendition) {
		delete m_Rendition;
		m_Rendition = 0;
	}

	m_MetaItemList.clear();

}

MetaItem* Metadata::GetMetaItemByName(QString name)
{
	foreach(MetaItem* metaItem, m_MetaItemList) {
		if (metaItem->GetName() == name) {
			return metaItem;
		}
	}

	return NULL;
}

bool Metadata::SetMetaItem(QString name, QString content, QString title)
{
	MetaItem* metaItem = new MetaItem();
	if (metaItem) {
		metaItem->SetName(name);
		metaItem->SetContent(content);
		metaItem->SetTitle(title);
		m_MetaItemList.append(metaItem);
		return true;
	}

	return false;
}

bool Metadata::SetRendition(QXmlStreamReader &opfReader)
{
	QString value = opfReader.attributes().value("", "property").toString();
	if (value == "rendition:layout")
		m_Rendition->SetRenditionLayout(opfReader.readElementText());
	else if (value == "rendition:orientation")
		m_Rendition->SetRenditionOrientation(opfReader.readElementText());
	else if (value == "rendition:spread")
		m_Rendition->SetRenditionSpread(opfReader.readElementText());

	return true;

}

/************************************************************************
**
**  Class MetaItem
**
*************************************************************************/

MetaItem::MetaItem(QObject *parent) :
	QObject(parent)
	, m_Name("")
	, m_Content("")
	, m_Title("")
{

}

MetaItem::~MetaItem()
{

}

/************************************************************************
**
**  Class Rendition
**
*************************************************************************/

Rendition::Rendition(QObject *parent) :
	QObject(parent)
	, m_Layout("")
	, m_Orientation("")
	, m_Spread("")
{

}

Rendition::~Rendition()
{

}

