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
#ifndef EPUB_UTIL_H
#define EPUB_UTIL_H

#include <QtCore/QObject>

class EpubUtil : public QObject
{
    Q_OBJECT

public:

    explicit EpubUtil();
    ~EpubUtil();

	bool EpubImport(const QString &zippath, const QString &destpath);
	bool EpubExport(const QString &fullfolderpath, const QString &fullfilepath);

private:
    
	bool UnZip(const QString &zippath, const QString &destpath);
	bool Zip(const QString &fullfolderpath, const QString &fullfilepath);


};

#endif // EPUB_UTIL_H
