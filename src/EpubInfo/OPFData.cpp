/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Virgo.
**
**  Virgo is Epub Parser.
**
*************************************************************************/

#include "OPFData.h"

#include <QtCore/QtDebug>
#include <QtCore/QFile>

#include <EpubInfo/Metadata.h>
#include <EpubInfo/Manifest.h>
#include <EpubInfo/Spine.h>
#include <EpubInfo/TocNcx.h>
#include <EpubInfo/TocXhtml.h>

OPFData::OPFData(QObject *parent) :
	QObject(parent)
	, m_FolderPath("")
	, m_OPFFilePath("")
	, m_EpubVersion("")
	, m_EpubUniqueID("")
	, m_Metadata(new Metadata())
	, m_Manifest(new Manifest())
	, m_Spine(new Spine())
	, m_TocNcx(new TocNcx())
	, m_TocXhtml(new TocXhtml())
{
    
}

OPFData::~OPFData()
{
    
	if (m_Metadata) {
		delete m_Metadata;
		m_Metadata = 0;
	}

	if (m_Manifest) {
		delete m_Manifest;
		m_Manifest = 0;
	}

	if (m_Spine) {
		delete m_Spine;
		m_Spine = 0;
	}

	if (m_TocNcx) {
		delete m_TocNcx;
		m_TocNcx = 0;
	}

	if (m_TocXhtml) {
		delete m_TocXhtml;
		m_TocXhtml = 0;
	}

}

bool OPFData::OPFParsing(QString folderpath, QString filepath)
{
	bool ret = false;
	if (folderpath.isEmpty() || filepath.isEmpty()) return ret;

	m_FolderPath = folderpath;
	m_OPFFilePath = filepath;
	QString fullfilepath = m_FolderPath + "/" + m_OPFFilePath;
	QFile file(fullfilepath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return ret;

	QXmlStreamReader opfReader(&file);
	while (!opfReader.atEnd()) {
		opfReader.readNext();
		if (opfReader.isStartElement()) {
			QString name = opfReader.name().toString();
			if (name == "package") {
				SetEpubVersion(opfReader.attributes().value("version").toString());
				SetEpubUniqueID(opfReader.attributes().value("unique-identifier").toString());
			}
			else if (name == "metadata") {
				ret |= SetMetadata(opfReader);
			}
			else if (name == "manifest") {
				ret |= SetManifest(opfReader);
			}
			else if (name == "spine") {
				ret |= SetSpine(opfReader);
			}
		}
	}
	file.close();
	return true;
}

bool OPFData::SetMetadata(QXmlStreamReader &opfReader)
{
	while (!opfReader.atEnd()
		&& !(opfReader.isEndElement()
		&& (opfReader.name().toString() == "metadata"))) {

		opfReader.readNext();
		if (opfReader.isStartElement()) {
			QString name = opfReader.name().toString();
			if (name.contains("identifier") && opfReader.attributes().value("id").toString() == GetEpubUniqueID())
				m_Metadata->SetMetadataIdentifier(opfReader.readElementText());
			else if (name.contains("title"))
				m_Metadata->SetMetadataTitle(opfReader.readElementText());
			else if (name.contains("language"))
				m_Metadata->SetMetadataLanguage(opfReader.readElementText());
			else if (name.contains("contributor"))
				m_Metadata->SetMetadataContributor(opfReader.readElementText());
			else if (name.contains("coverage"))
				m_Metadata->SetMetadataCoverage(opfReader.readElementText());
			else if (name.contains("creator"))
				m_Metadata->SetMetadataCreator(opfReader.readElementText());
			else if (name.contains("date"))
				m_Metadata->SetMetadataDate(opfReader.readElementText());
			else if (name.contains("description"))
				m_Metadata->SetMetadataDescription(opfReader.readElementText());
			else if (name.contains("format"))
				m_Metadata->SetMetadataFormat(opfReader.readElementText());
			else if (name.contains("publisher"))
				m_Metadata->SetMetadataPublisher(opfReader.readElementText());
			else if (name.contains("relation"))
				m_Metadata->SetMetadataRelation(opfReader.readElementText());
			else if (name.contains("rights"))
				m_Metadata->SetMetadataRights(opfReader.readElementText());
			else if (name.contains("source"))
				m_Metadata->SetMetadataSource(opfReader.readElementText());
			else if (name.contains("subject"))
				m_Metadata->SetMetadataSubject(opfReader.readElementText());
			else if (name.contains("type"))
				m_Metadata->SetMetadataType(opfReader.readElementText());
			else if (name.contains("meta")) {
				
				if (!opfReader.attributes().value("name").toString().isEmpty()) {
					QString attrName = opfReader.attributes().value("name").toString();
					QString attrContent = opfReader.attributes().value("content").toString();
					QString title = opfReader.readElementText();
					m_Metadata->SetMetaItem(attrName, attrContent, title);
				}
				else if (!opfReader.attributes().value("property").toString().isEmpty()
						&& GetEpubVersion().startsWith("3")) {
					m_Metadata->SetRendition(opfReader);
				}
			}
		}
		else if (opfReader.isEndElement()) {
			return true;
		}
	}
	return false;
}

bool OPFData::SetManifest(QXmlStreamReader &opfReader)
{
	while (!opfReader.atEnd()
		&& !(opfReader.isEndElement()
			&& (opfReader.name().toString() == "manifest"))) {

		opfReader.readNext();
		QString name = opfReader.name().toString();
		if (opfReader.isStartElement() && name == "item") {
			QXmlStreamAttributes attributes = opfReader.attributes();
			QString id = attributes.value("id").toString();
			QString href = attributes.value("href").toString();
			QString mediaType = attributes.value("media-type").toString();
			QString properties = attributes.value("properties").toString();

			m_Manifest->SetManifestItem(id, href, mediaType, properties);

			// set toc file path
			if (mediaType == "application/x-dtbncx+xml") {
				m_Manifest->SetTocNcxPath(href);
			}
			else if (!properties.isEmpty() && properties == "nav") {
				m_Manifest->SetTocXhtmlPath(href);
			}

		}
		else if (opfReader.isEndElement()) {
			return true;
		}

	}
	return false;

}

bool OPFData::SetSpine(QXmlStreamReader &opfReader)
{
	while (!opfReader.atEnd()
		&& !(opfReader.isEndElement()
			&& (opfReader.name().toString() == "spine"))) {

		opfReader.readNext();
		QString name = opfReader.name().toString();
		if (opfReader.isStartElement() && name == "itemref") {
			QXmlStreamAttributes attributes = opfReader.attributes();
			QString idref = attributes.value("idref").toString();
			QString linear = attributes.value("linear").toString();

			if (linear.isEmpty()) { m_Spine->SetSpineItem(idref); }
			else { m_Spine->SetSpineItem(idref, linear); }
		}
		else if (opfReader.isEndElement()) {
			return true;
		}
	}
	return false;
}

