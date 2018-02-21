/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Virgo.
**
**  Virgo is Epub Parser.
**
*************************************************************************/

#pragma once
#ifndef MANIFEST_H
#define MANIFEST_H

#include <QtCore/QObject>

class ManifestItem;


class Manifest : public QObject
{
    Q_OBJECT

public:
    explicit Manifest(QObject *parent = 0);
    ~Manifest();

	QList<ManifestItem*>	GetManifestItemList();
	ManifestItem*			GetItemByID(QString id);
	ManifestItem*			GetItemByHref(QString href);
	QList<ManifestItem*>	GetItemListByMediaType(QString mediaType);
	QList<ManifestItem*>	GetItemListByProperties(QString properties);
	bool					SetManifestItem(QString id, QString href, QString mediaType, QString properties = "");

	QString					GetTocNcxPath() { return m_TocNcxPath; }
	void					SetTocNcxPath(QString data) { m_TocXhtmlPath = data; }
	QString					GetTocXhtmlPath() { return m_TocXhtmlPath; }
	void					SetTocXhtmlPath(QString data) { m_TocXhtmlPath = data; }

private:

	QList<ManifestItem*>	m_ManifestItemList;
	QString					m_TocNcxPath;
	QString					m_TocXhtmlPath;

    
};


class ManifestItem : public QObject
{
	Q_OBJECT

public:
	explicit ManifestItem(QObject *parent = 0);
	~ManifestItem();

	QString				GetID() { return m_ItemID; }
	void				SetID(QString data) { m_ItemID = data; }
	QString				GetHref() { return m_Href; }
	void				SetHref(QString data) { m_Href = data; }
	QString				GetMediaType() { return m_MediaType; }
	void				SetMediaType(QString data) { m_MediaType = data; }
	QString				GetProperties() { return m_Properties; }
	void				SetProperties(QString data) { m_Properties = data; }

private:

	QString m_ItemID;
	QString m_Href;
	QString m_MediaType;
	QString m_Properties;	// EPUB3

};


#endif // MANIFEST_H
