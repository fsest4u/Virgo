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
#ifndef UTILITY_H
#define UTILITY_H

#include <QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QIODevice>


class Utility
{
	Q_DECLARE_TR_FUNCTIONS(Utility)

public:

#if defined(Q_OS_WIN32)
	static std::wstring QStringToStdWString(const QString &str);
	static QString stdWStringToQString(const std::wstring &str);
#endif

	static bool IsFileReadable(const QString &fullfilepath);

	static QString Utility::ReadUnicodeTextFile(const QString &fullfilepath);
	static void Utility::WriteUnicodeTextFile(const QString &text, const QString &fullfilepath, QIODevice::OpenModeFlag flag);
	static QString Utility::ConvertLineEndings(const QString &text);



};

#endif // UTILITY_H
