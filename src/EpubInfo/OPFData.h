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
#ifndef OPFDATA_H
#define OPFDATA_H

#include <QtCore/QObject>
#include <QtCore/QXmlStreamReader>

class Metadata;
class Manifest;
class Spine;
class TocXhtml;
class TocNcx;

class OPFData : public QObject
{
    Q_OBJECT

public:
    explicit OPFData(QObject *parent = 0);
    ~OPFData();

	bool					OPFParsing(QString folderpath, QString filepath);

	QString					GetEpubVersion() { return m_EpubVersion; }
	void					SetEpubVersion(QString version) { m_EpubVersion = version; }
	QString					GetEpubUniqueID() { return m_EpubUniqueID; }
	void					SetEpubUniqueID(QString id) { m_EpubUniqueID = id; }

	Metadata*				GetMetadata() { return m_Metadata; }
	bool					SetMetadata(QXmlStreamReader &opfReader);
	Manifest*				GetMainfest() { return m_Manifest; }
	bool					SetManifest(QXmlStreamReader &opfReader);
	Spine*					GetSpine() { return m_Spine; }
	bool					SetSpine(QXmlStreamReader &opfReader);
	TocXhtml*				GetTocXhtml() { return m_TocXhtml; }
	TocNcx*					GetTocNcx() { return m_TocNcx; }


	QString					GetArchivePath() { return m_ArchivePath; }
	int						GetArchiveFileCount() { return m_ArchiveFileCount; }


private:
    
	QString					m_FolderPath;
	QString					m_OPFFilePath;
	QString					m_EpubVersion;
	QString					m_EpubUniqueID;
	Metadata*				m_Metadata;
	Manifest*				m_Manifest;
	Spine*					m_Spine;
	TocXhtml*				m_TocXhtml;
	TocNcx*					m_TocNcx;
	QString					m_ArchivePath;
	int						m_ArchiveFileCount;
};

#endif // OPFDATA_H
