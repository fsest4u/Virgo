/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Virgo.
**
**  Virgo is Epub Parser.
**
*************************************************************************/

#include "EpubUtil.h"

#include "zip.h"
#include "unzip.h"
#ifdef _WIN32
#include "iowin32.h"
#endif

#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>

#include "Misc/Utility.h"


#define BUFF_SIZE 8192

static const char * EPUB_MIME_DATA = "application/epub+zip";


EpubUtil::EpubUtil()
{
    
}

EpubUtil::~EpubUtil()
{
    
}

bool EpubUtil::EpubImport(const QString &zippath, const QString &destpath)
{
	QDir srcDir(destpath);
	if (!srcDir.exists()) {
		QFileInfo fileInfo(destpath);
		QString filePath = fileInfo.absolutePath();
		QDir dstDir(filePath);
		QString filename = fileInfo.completeBaseName();
		dstDir.mkdir(filename);
	}
	return UnZip(zippath, destpath);
}

bool EpubUtil::EpubExport(const QString &fullfolderpath, const QString &fullfilepath)
{
	return Zip(fullfolderpath, fullfilepath);
}

bool EpubUtil::UnZip(const QString &zippath, const QString &destpath)
{
	int res = 0;
	QDir dir(destpath);
	//if (!cp437) {
	//	cp437 = new QCodePage437Codec();
	//}
#ifdef Q_OS_WIN32
	zlib_filefunc64_def ffunc;
	fill_win32_filefunc64W(&ffunc);
	unzFile zfile = unzOpen2_64(Utility::QStringToStdWString(QDir::toNativeSeparators(zippath)).c_str(), &ffunc);
#else
	unzFile zfile = unzOpen64(QDir::toNativeSeparators(zippath).toUtf8().constData());
#endif

	if ((zfile == NULL) || (!Utility::IsFileReadable(zippath)) || (!dir.exists())) {
		return false;
	}

	res = unzGoToFirstFile(zfile);

	if (res == UNZ_OK) {
		do {
			// Get the name of the file in the archive.
			char file_name[MAX_PATH] = { 0 };
			unz_file_info64 file_info;
			unzGetCurrentFileInfo64(zfile, &file_info, file_name, MAX_PATH, NULL, 0, NULL, 0);
			QString qfile_name;
			QString cp437_file_name;
			qfile_name = QString::fromUtf8(file_name);
			if (!(file_info.flag & (1 << 11))) {
				// General purpose bit 11 says the filename is utf-8 encoded. If not set then
				// IBM 437 encoding might be used.
				//cp437_file_name = cp437->toUnicode(file_name);
				cp437_file_name = QString::fromUtf8(file_name);
			}

			// If there is no file name then we can't do anything with it.
			if (!qfile_name.isEmpty()) {
				// We use the dir object to create the path in the temporary directory.
				// Unfortunately, we need a dir ojbect to do this as it's not a static function.
				// Full file path in the temporary directory.
				QString file_path = destpath + "/" + qfile_name;
				QFileInfo qfile_info(file_path);

				// Is this entry a directory?
				if (file_info.uncompressed_size == 0 && qfile_name.endsWith('/')) {
					dir.mkpath(qfile_name);
					continue;
				}
				else {
					dir.mkpath(qfile_info.path());
				}

				// Open the file entry in the archive for reading.
				if (unzOpenCurrentFile(zfile) != UNZ_OK) {
					unzClose(zfile);
					return false;
				}

				// Open the file on disk to write the entry in the archive to.
				QFile entry(file_path);

				if (!entry.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
					unzCloseCurrentFile(zfile);
					unzClose(zfile);
					return false;
				}

				// Buffered reading and writing.
				char buff[BUFF_SIZE] = { 0 };
				int read = 0;

				while ((read = unzReadCurrentFile(zfile, buff, BUFF_SIZE)) > 0) {
					entry.write(buff, read);
				}

				entry.close();

				// Read errors are marked by a negative read amount.
				if (read < 0) {
					unzCloseCurrentFile(zfile);
					unzClose(zfile);
					return false;
				}

				// The file was read but the CRC did not match.
				// We don't check the read file size vs the uncompressed file size
				// because if they're different there should be a CRC error.
				if (unzCloseCurrentFile(zfile) == UNZ_CRCERROR) {
					unzClose(zfile);
					return false;
				}

				if (!cp437_file_name.isEmpty() && cp437_file_name != qfile_name) {
					QString cp437_file_path = destpath + "/" + cp437_file_name;
					QFile::copy(file_path, cp437_file_path);
				}
			}
		} while ((res = unzGoToNextFile(zfile)) == UNZ_OK);
	}

	if (res != UNZ_END_OF_LIST_OF_FILE) {
		unzClose(zfile);
		return false;
	}

	unzClose(zfile);
	return true;
}

bool EpubUtil::Zip(const QString &fullfolderpath, const QString &fullfilepath)
{
	QString tempFile = fullfolderpath + "-tmp.epub";
	QDateTime timeNow = QDateTime::currentDateTime();
	zip_fileinfo fileInfo;
#if 0//def Q_OS_WIN32
	zlib_filefunc64_def ffunc;
	fill_win32_filefunc64W(&ffunc);
	zipFile zfile = zipOpen2_64(Utility::QStringToStdWString(QDir::toNativeSeparators(tempFile)).c_str(), APPEND_STATUS_CREATE, NULL, &ffunc);
#else
	zipFile zfile = zipOpen64(QDir::toNativeSeparators(tempFile).toUtf8().constData(), APPEND_STATUS_CREATE);
#endif

	if (zfile == NULL) {
		//throw (CannotOpenFile(tempFile.toStdString()));
		return false;
	}

	memset(&fileInfo, 0, sizeof(fileInfo));
	fileInfo.tmz_date.tm_sec = timeNow.time().second();
	fileInfo.tmz_date.tm_min = timeNow.time().minute();
	fileInfo.tmz_date.tm_hour = timeNow.time().hour();
	fileInfo.tmz_date.tm_mday = timeNow.date().day();
	fileInfo.tmz_date.tm_mon = timeNow.date().month() - 1;
	fileInfo.tmz_date.tm_year = timeNow.date().year();

	// Write the mimetype. This must be uncompressed and the first entry in the archive.
	if (zipOpenNewFileInZip64(zfile, "mimetype", &fileInfo, NULL, 0, NULL, 0, NULL, Z_NO_COMPRESSION, 0, 0) != Z_OK) {
		zipClose(zfile, NULL);
		QFile::remove(tempFile);
		//throw(CannotStoreFile("mimetype"));
		return false;
	}

	if (zipWriteInFileInZip(zfile, EPUB_MIME_DATA, (unsigned int)strlen(EPUB_MIME_DATA)) != Z_OK) {
		zipCloseFileInZip(zfile);
		zipClose(zfile, NULL);
		QFile::remove(tempFile);
		//throw(CannotStoreFile("mimetype"));
		return false;
	}

	zipCloseFileInZip(zfile);
	// Write all the files in our directory path to the archive.
	QDirIterator it(fullfolderpath, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable | QDir::Hidden, QDirIterator::Subdirectories);

	while (it.hasNext()) {
		it.next();
		QString relpath = it.filePath().remove(fullfolderpath);

		while (relpath.startsWith("/")) {
			relpath = relpath.remove(0, 1);
		}

		// Add the file entry to the archive.
		// We should check the uncompressed file size. If it's over >= 0xffffffff the last parameter (zip64) should be 1.
		if (zipOpenNewFileInZip4_64(zfile, relpath.toUtf8().constData(), &fileInfo, NULL, 0, NULL, 0, NULL, Z_DEFLATED, 8, 0, 15, 8, Z_DEFAULT_STRATEGY, NULL, 0, 0x0b00, 1 << 11, 0) != Z_OK) {
			zipClose(zfile, NULL);
			QFile::remove(tempFile);
			//throw(CannotStoreFile(relpath.toStdString()));
			return false;
		}

		// Open the file on disk. We will read this and write what we read into
		// the archive.
		QFile dfile(it.filePath());

		if (!dfile.open(QIODevice::ReadOnly)) {
			zipCloseFileInZip(zfile);
			zipClose(zfile, NULL);
			QFile::remove(tempFile);
			//throw(CannotOpenFile(it.fileName().toStdString()));
			return false;
		}

		// Write the data from the file on disk into the archive.
		char buff[BUFF_SIZE] = { 0 };
		qint64 read = 0;

		while ((read = dfile.read(buff, BUFF_SIZE)) > 0) {
			if (zipWriteInFileInZip(zfile, buff, read) != Z_OK) {
				dfile.close();
				zipCloseFileInZip(zfile);
				zipClose(zfile, NULL);
				QFile::remove(tempFile);
				//throw(CannotStoreFile(relpath.toStdString()));
				return false;
			}
		}

		dfile.close();

		// There was an error reading the file on disk.
		if (read < 0) {
			zipCloseFileInZip(zfile);
			zipClose(zfile, NULL);
			QFile::remove(tempFile);
			//throw(CannotStoreFile(relpath.toStdString()));
			return false;
		}

		if (zipCloseFileInZip(zfile) != Z_OK) {
			zipClose(zfile, NULL);
			QFile::remove(tempFile);
			//throw(CannotStoreFile(relpath.toStdString()));
			return false;
		}
	}

	zipClose(zfile, NULL);
	// Overwrite the contents of the real file with the contents from the temp
	// file we saved the data do. We do this instead of simply copying the file
	// because a file copy will lose extended attributes such as labels on OS X.
	QFile temp_epub(tempFile);

	if (!temp_epub.open(QFile::ReadOnly)) {
		//throw(CannotOpenFile(tempFile.toStdString()));
		return false;
	}

	QFile real_epub(fullfilepath);

	if (!real_epub.open(QFile::WriteOnly | QFile::Truncate)) {
		temp_epub.close();
		//throw(CannotWriteFile(fullfilepath.toStdString()));
		return false;
	}

	// Copy the contents from the temp file to the real file.
	char buff[BUFF_SIZE] = { 0 };
	qint64 read = 0;
	qint64 written = 0;

	while ((read = temp_epub.read(buff, BUFF_SIZE)) > 0) {
		written = real_epub.write(buff, read);

		if (written != read) {
			temp_epub.close();
			real_epub.close();
			QFile::remove(tempFile);
			//throw(CannotCopyFile(fullfilepath.toStdString()));
			return false;
		}
	}

	if (read == -1) {
		temp_epub.close();
		real_epub.close();
		QFile::remove(tempFile);
		//throw(CannotCopyFile(fullfilepath.toStdString()));
		return false;
	}

	temp_epub.close();
	real_epub.close();
	QFile::remove(tempFile);

	return true;

}
