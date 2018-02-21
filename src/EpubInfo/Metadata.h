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
#ifndef METADATA_H
#define METADATA_H

#include <QObject>
#include <QtCore/QXmlStreamReader>

class MetaItem;
class Rendition;

class Metadata : public QObject
{
    Q_OBJECT

public:
    explicit Metadata(QObject *parent = 0);
    ~Metadata();

	QString				GetMetadataIdentifier() { return m_Identifier; };
	void				SetMetadataIdentifier(QString data) { m_Identifier = data; };
	QString				GetMetadataTitle() { return m_Title; };
	void				SetMetadataTitle(QString data) { m_Title = data; };
	QString				GetMetadataLanguage() { return m_Language; };
	void				SetMetadataLanguage(QString data) { m_Language = data; };
	QString				GetMetadataContributor() { return m_Contributor; };
	void				SetMetadataContributor(QString data) { m_Contributor = data; };
	QString				GetMetadataCoverage() { return m_Coverage; };
	void				SetMetadataCoverage(QString data) { m_Coverage = data; };
	QString				GetMetadataCreator() { return m_Creator; };
	void				SetMetadataCreator(QString data) { m_Creator = data; };
	QString				GetMetadataDate() { return m_Date; };
	void				SetMetadataDate(QString data) { m_Date = data; };
	QString				GetMetadataDescription() { return m_Description; };
	void				SetMetadataDescription(QString data) { m_Description = data; };
	QString				GetMetadataFormat() { return m_Format; };
	void				SetMetadataFormat(QString data) { m_Format = data; };
	QString				GetMetadataPublisher() { return m_Publisher; };
	void				SetMetadataPublisher(QString data) { m_Publisher = data; };
	QString				GetMetadataRelation() { return m_Relation; };
	void				SetMetadataRelation(QString data) { m_Relation = data; };
	QString				GetMetadataRights() { return m_Rights; };
	void				SetMetadataRights(QString data) { m_Rights = data; };
	QString				GetMetadataSource() { return m_Source; };
	void				SetMetadataSource(QString data) { m_Source = data; };
	QString				GetMetadataSubject() { return m_Subject; };
	void				SetMetadataSubject(QString data) { m_Subject = data; };
	QString				GetMetadataType() { return m_Type; };
	void				SetMetadataType(QString data) { m_Type = data; };

	QList<MetaItem*>	GetMetaItemList() { return m_MetaItemList; }
	MetaItem*			GetMetaItemByName(QString name);
	bool				SetMetaItem(QString name, QString content, QString title);
	Rendition*			GetRendition() { return m_Rendition; }
	bool				SetRendition(QXmlStreamReader &opfReader);


private:
    
	// mandantory
	QString				m_Identifier;
	QString				m_Title;
	QString				m_Language;
	// optional
	QString				m_Contributor;
	QString				m_Coverage;
	QString				m_Creator;
	QString				m_Date;
	QString				m_Description;
	QString				m_Format;
	QString				m_Publisher;
	QString				m_Relation;
	QString				m_Rights;
	QString				m_Source;
	QString				m_Subject;
	QString				m_Type;

	QList<MetaItem*>	m_MetaItemList;
	Rendition*			m_Rendition;

};

class MetaItem : public QObject
{
	Q_OBJECT

public:
	explicit MetaItem(QObject *parent = 0);
	~MetaItem();

	QString				GetName() { return m_Name; }
	void				SetName(QString data) { m_Name = data; }
	QString				GetContent() { return m_Content; }
	void				SetContent(QString data) { m_Content = data; }
	QString				GetTitle() { return m_Title; }
	void				SetTitle(QString data) { m_Title = data; }

private:

	QString				m_Name;
	QString				m_Content;
	QString				m_Title;

};

class Rendition : public QObject
{
	Q_OBJECT

public:
	explicit Rendition(QObject *parent = 0);
	~Rendition();

	QString				GetRenditionLayout() { return m_Layout; }
	void				SetRenditionLayout(QString data) { m_Layout = data; }
	QString				GetRenditionOrientation() { return m_Orientation; }
	void				SetRenditionOrientation(QString data) { m_Orientation = data; }
	QString				GetRenditionSpread() { return m_Spread; }
	void				SetRenditionSpread(QString data) { m_Spread = data; }

private:

	QString				m_Layout;
	QString				m_Orientation;
	QString				m_Spread;

};


#endif // METADATA_H
