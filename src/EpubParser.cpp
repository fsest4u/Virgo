/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Virgo.
**
**  Virgo is Epub Parser.
**
*************************************************************************/

#include "EpubParser.h"

#include <QtCore/QtDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QXmlStreamReader>


#include "EpubInfo/OPFData.h"
#include "EpubInfo/Metadata.h"
#include "EpubInfo/Manifest.h"
#include "EpubInfo/Spine.h"
#include "EpubInfo/TocNcx.h"
#include "EpubInfo/TocXhtml.h"
#include "Misc/EpubUtil.h"
#include "Misc/Utility.h"


const QString MIMETYPE_FILE_NAME = "mimetype";
const QString EPUB_MIME_DATA = "application/epub+zip";

const QString CONTAINER_XML_FILE_NAME = "META-INF/container.xml";
const QString OEBPS_MIMETYPE = "application/oebps-package+xml";


EpubParser::EpubParser(QWidget *parent) :
    QWidget(parent)
	, m_ZipFilePath("")
	, m_FolderPath("")
	, m_OPFFilePath("")
	, m_OPFData(NULL)
	, m_bOPFParse(false)
{
    
}

EpubParser::~EpubParser()
{
    
}

//////////////////////////////
// File and Zip Functions
//////////////////////////////

bool EpubParser::EpubRead()
{
	bool ret = false;

	// Unzip
	EpubUtil epubUtil;
	ret = epubUtil.EpubImport(m_ZipFilePath, m_FolderPath);

	return ret;
}

bool EpubParser::EpubWrite()
{
	bool ret = false;

	// Zip
	EpubUtil epubUtil;
	ret = epubUtil.EpubExport(m_FolderPath, m_ZipFilePath);

	return ret;
}

//////////////////////////////
// Validation
//////////////////////////////
bool EpubParser::CheckMimeType()
{
	QString fullfilename = m_FolderPath + "/" + MIMETYPE_FILE_NAME;
	QFile file(fullfilename);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		while (!in.atEnd()) {
			QString line = in.readLine();
			if (line.contains(EPUB_MIME_DATA)) {
				file.close();
				return true;
			}
		}
		file.close();
		return false;
	}
	return false;
}

//////////////////////////////
// XML Parsing
//////////////////////////////

QString EpubParser::GetOPFFilePath()
{
	QString filepath = "";
	QString fullfilename = m_FolderPath + "/" + CONTAINER_XML_FILE_NAME;
	QString containerText = Utility::ReadUnicodeTextFile(fullfilename);
	if (containerText.isEmpty()) { return filepath; }

	// read container.xml for opf file path
	QXmlStreamReader container;
	container.addData(containerText);
	while (!container.atEnd()) {
		container.readNext();

		if (container.isStartElement() && container.name() == "rootfile") {
			if (container.attributes().hasAttribute("media-type") &&
				container.attributes().value("", "media-type") == OEBPS_MIMETYPE ) {
				filepath = m_FolderPath + "/" + container.attributes().value("", "full-path").toString();
				// As per OCF spec, the first rootfile element
				// with the OEBPS mimetype is considered the "main" one.
				break;
			}
		}
	}

	return filepath;
}

bool EpubParser::EpubParsing()
{
	m_bOPFParse = false;
	if (!CheckMimeType()) { return m_bOPFParse; }

	// get opf file path
	m_OPFFilePath = GetOPFFilePath();

	// read opf file
	m_OPFData = new OPFData();
	m_bOPFParse = m_OPFData->OPFParsing(m_FolderPath, m_OPFFilePath);
	return m_bOPFParse;
}

//////////////////////////////
// TOC Parsing
//////////////////////////////
bool EpubParser::TocNcxParsing()
{
	bool ret = false;
	if (!m_bOPFParse || !m_OPFData) { return ret; }
	QString fullfilename = m_FolderPath + "/" + m_OPFData->GetMainfest()->GetTocNcxPath();
	QFile file(fullfilename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return ret;

	QXmlStreamReader ncxReader(&file);
	while (!ncxReader.atEnd()) {
		ncxReader.readNext();
		if (ncxReader.isStartElement()) {
			QString name = ncxReader.name().toString();
			if (name == "head") {
				ncxReader.readNext();
				if (ncxReader.isStartElement() && ncxReader.name().toString() == "meta") {
					while (!(ncxReader.isEndElement()
						&& (ncxReader.name().toString() == "meta"))) {

						int depth = ncxReader.attributes().value("dtb:depth").toString().toInt();
						int totalPageCount = ncxReader.attributes().value("dtb:totalPageCount").toString().toInt();
						int maxPageNumber = ncxReader.attributes().value("dtb:maxPageNumber").toString().toInt();

						m_OPFData->GetTocNcx()->SetDepth(depth);
						m_OPFData->GetTocNcx()->SetTotalPageCount(totalPageCount);
						m_OPFData->GetTocNcx()->SetMaxPageNumber(maxPageNumber);
					}
				}
			}
			else if (name == "docTitle") {
				while (!(ncxReader.isEndElement()
					&& (ncxReader.name().toString() == "docTitle"))) {
					ncxReader.readNext();
					if (ncxReader.isStartElement() && ncxReader.name().toString() == "text") {
						//epub_data.setToc_title(ncxReader.readElementText());
						m_OPFData->GetTocNcx()->SetDocTitle(ncxReader.readElementText());
						break;
					}
				}
			}
			else if (name == "navMap") {
				while (!(ncxReader.isEndElement()
					&& (ncxReader.name().toString() == "navMap"))) {
					ncxReader.readNext();
					if (ncxReader.isStartElement() && ncxReader.name().toString() == "navPoint") {
						while (!(ncxReader.isEndElement()
							&& (ncxReader.name().toString() == "navPoint"))) {

							QString id = ncxReader.attributes().value("id").toString();
							QString playOrder = ncxReader.attributes().value("playOrder").toString();

							ncxReader.readNext();

							QString label = "";
							QString src = "";
							if (ncxReader.isStartElement() && ncxReader.name().toString() == "text") {
								label = ncxReader.readElementText();
							}
							else if (ncxReader.isStartElement() && ncxReader.name().toString() == "content") {
								src = ncxReader.attributes().value("src").toString();
							}
							ret |= m_OPFData->GetTocNcx()->SetTocNcxItem(id, playOrder, label, src);
						}
						//epub_data.insertTocList(*toc_name, *toc_src);
					}
				}
			}
		}
	}
	file.close();
	return ret;

}

bool EpubParser::TocXhtmlParsing()
{
	bool ret = false;
	if (!m_bOPFParse || !m_OPFData) { return ret; }
	QString fullfilename = m_FolderPath + "/" + m_OPFData->GetMainfest()->GetTocXhtmlPath();
	QFile file(fullfilename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return ret;

	QXmlStreamReader navReader(&file);
	while (!navReader.atEnd()) {
		navReader.readNext();
		if (navReader.isStartElement()) {
			QString name = navReader.name().toString();
			if (name == "title") {
				//epub_data.setToc_title(navReader.readElementText());
				m_OPFData->GetTocXhtml()->SetDocTitle(navReader.readElementText());
			}
			else if (name == "nav" && navReader.attributes().value("epub:type").toString() == "toc") {

				QString type = navReader.attributes().value("epub:type").toString();
				QString id = navReader.attributes().value("id").toString();
				QString title = "";
				m_OPFData->GetTocXhtml()->SetNavItem(type, id, title);

				while (!(navReader.isEndElement()
					&& (navReader.name().toString() == "nav"))) {
					navReader.readNext();
					if (navReader.isStartElement() && navReader.name().toString() == "a") {

						QString src = "";
						QString label = "";
						src = navReader.attributes().value("href").toString();
						label = navReader.readElementText();
						//epub_data.insertTocList(*toc_name, *toc_src);
						ret |= m_OPFData->GetTocXhtml()->GetNavItem(type)->SetTocXHtmlItem(label, src);
					}
				}
			}
			//else if (name == "nav" && navReader.attributes().value("epub:type").toString() == "page-list") {

			//	QString type = navReader.attributes().value("epub:type").toString();
			//	QString id = navReader.attributes().value("id").toString();
			//	QString title = "";
			//	m_OPFData->GetTocXhtml()->SetNavItem(type, id, title);

			//	while (!(navReader.isEndElement()
			//		&& (navReader.name().toString() == "nav"))) {
			//		navReader.readNext();
			//		if (navReader.isStartElement() && navReader.name().toString() == "a") {

			//			QString src = "";
			//			QString label = "";
			//			src = navReader.attributes().value("href").toString();
			//			label = navReader.readElementText();
			//			//epub_data.insertPageList(*page_name, *toc_src);
			//			ret |= m_OPFData->GetTocXhtml()->GetNavItem(type)->SetTocXHtmlItem(label, src);
			//		}
			//	}
			//}
		}
	}
	file.close();
	return ret;

}

//////////////////////////////
// Metadata
//////////////////////////////
QString EpubParser::GetMetadataTitle()
{
	if (!m_bOPFParse || !m_OPFData) { return QString(""); }
	else return m_OPFData->GetMetadata()->GetMetadataTitle();
}

//////////////////////////////
// Manifest
//////////////////////////////

//////////////////////////////
// Spine
//////////////////////////////

//////////////////////////////
// TOC - XHTML
//////////////////////////////

//////////////////////////////
// TOC - NCX
//////////////////////////////
