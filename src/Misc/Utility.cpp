/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Virgo.
**
**  Virgo is Epub Parser.
**
*************************************************************************/

#include "Utility.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#if defined(Q_OS_WIN32)
std::wstring Utility::QStringToStdWString(const QString &str)
{
	return std::wstring((const wchar_t *)str.utf16());
}

QString Utility::stdWStringToQString(const std::wstring &str)
{
	return QString::fromUtf16((const ushort *)str.c_str());
}
#endif

// Returns true if the file can be read;
// shows an error dialog if it can't
// with a message elaborating what's wrong
bool Utility::IsFileReadable(const QString &fullfilepath)
{
	// Qt has <QFileInfo>.exists() and <QFileInfo>.isReadable()
	// functions, but then we would have to create our own error
	// message for each of those situations (and more). Trying to
	// actually open the file enables us to retrieve the exact
	// reason preventing us from reading the file in an error string.
	QFile file(fullfilepath);

	// Check if we can open the file
	if (!file.open(QFile::ReadOnly)) {
		//Utility::DisplayStdErrorDialog(
		//	QObject::tr("Cannot read file %1:\n%2.")
		//	.arg(fullfilepath)
		//	.arg(file.errorString())
		//);
		return false;
	}

	file.close();
	return true;
}

// Reads the text file specified with the full file path;
// text needs to be in UTF-8 or UTF-16; if the file cannot
// be read, an error dialog is shown and an empty string returned
QString Utility::ReadUnicodeTextFile(const QString &fullfilepath)
{
	// TODO: throw an exception instead of
	// returning an empty string
	QFile file(fullfilepath);

	// Check if we can open the file
	if (!file.open(QFile::ReadOnly)) {
		std::string msg = fullfilepath.toStdString() + ": " + file.errorString().toStdString();
		//throw(CannotOpenFile(msg));
		return QString("");
	}

	QTextStream in(&file);
	// Input should be UTF-8
	in.setCodec("UTF-8");
	// This will automatically switch reading from
	// UTF-8 to UTF-16 if a BOM is detected
	in.setAutoDetectUnicode(true);
	return ConvertLineEndings(in.readAll());
}


// Writes the provided text variable to the specified
// file; if the file exists, it is truncated
void Utility::WriteUnicodeTextFile(const QString &text, const QString &fullfilepath, QIODevice::OpenModeFlag flag)
{
	QFile file(fullfilepath);

	if (!file.open(QIODevice::WriteOnly |
		flag |
		QIODevice::Text
	)
		) {
		std::string msg = file.fileName().toStdString() + ": " + file.errorString().toStdString();
		//throw(CannotOpenFile(msg));
		return;
	}

	QTextStream out(&file);
	// We ALWAYS output in UTF-8
	out.setCodec("UTF-8");
	out << text;
}

// Converts Mac and Windows style line endings to Unix style
// line endings that are expected throughout the Qt framework
QString Utility::ConvertLineEndings(const QString &text)
{
	QString newtext(text);
	return newtext.replace("\x0D\x0A", "\x0A").replace("\x0D", "\x0A");
}
