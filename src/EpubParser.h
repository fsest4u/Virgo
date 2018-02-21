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
#ifndef EPUBPARSER_H
#define EPUBPARSER_H

#include <QWidget>

class OPFData;

class EpubParser : public QWidget
{
    Q_OBJECT

public:


    explicit EpubParser(QWidget *parent = 0);
    ~EpubParser();

	//////////////////////////////
	// File and Zip Functions
	//////////////////////////////
	QString		GetZipFilePath() { return m_ZipFilePath; };
	void		SetZipFilePath(QString filepath) { m_ZipFilePath = filepath; };
	QString		GetFolderPath() { return m_FolderPath; };
	void		SetFolderPath(QString folderpath) { m_FolderPath = folderpath; };

	bool		EpubRead();
	bool		EpubWrite();

	//////////////////////////////
	// Validation
	//////////////////////////////
	bool		CheckMimeType();

	//////////////////////////////
	// XML Parsing
	//////////////////////////////
	QString		GetOPFFilePath();
	bool		EpubParsing();

	//////////////////////////////
	// TOC Parsing
	//////////////////////////////
	bool		TocNcxParsing();
	bool		TocXhtmlParsing();

	//////////////////////////////
	// Metadata
	//////////////////////////////
	QString GetMetadataTitle();

	//////////////////////////////
	// Manifest
	//////////////////////////////
	QStringList GetManifestItemIDList();

	//////////////////////////////
	// Spine
	//////////////////////////////
	QStringList GetSpineItemIDRefList();

	//////////////////////////////
	// TOC - XHTML
	//////////////////////////////

	//////////////////////////////
	// TOC - NCX
	//////////////////////////////


private:

	QString			m_ZipFilePath;
	QString			m_FolderPath;
	QString			m_OPFFilePath;
	OPFData*		m_OPFData;

	bool			m_bOPFParse;
    

};

#endif // EPUBPARSER_H
