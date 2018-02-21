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
#ifndef TOCNCXL_H
#define TOCNCXL_H

#include <QtCore/QObject>

class TocNcxItem;

class TocNcx : public QObject
{
	Q_OBJECT

public:
	explicit TocNcx(QObject *parent = 0);
	~TocNcx();

	QString				GetDocTitle() { return m_DocTitle; }
	void				SetDocTitle(QString data) { m_DocTitle = data; }
	int					GetDepth() { return m_nDepth; }
	void				SetDepth(int data) { m_nDepth = data; }
	int					GetTotalPageCount() { return m_nTotalPageCount; }
	void				SetTotalPageCount(int data) { m_nTotalPageCount = data; }
	int					GetMaxPageNumber() { return m_nMaxPageNumber; }
	void				SetMaxPageNumber(int data) { m_nMaxPageNumber = data; }
	QList<TocNcxItem*>	GetTocNcxItemList() { return m_TocNcxItemList; }
	TocNcxItem*			GetTocNcxItemByPID(QString id);
	TocNcxItem*			GetTocNcxItemByPlayOrder(QString playOrder);
	bool				SetTocNcxItem(QString id, QString playOrder, QString title = "", QString src = "");

private:

	QString				m_DocTitle;
	int					m_nDepth;
	int					m_nTotalPageCount;
	int					m_nMaxPageNumber;
	QList<TocNcxItem*>	m_TocNcxItemList;

};


class TocNcxItem : public QObject
{
	Q_OBJECT

public:
	explicit TocNcxItem(QObject *parent = 0);
	~TocNcxItem();

	QString				GetID() { return m_ID; }
	void				SetID(QString data) { m_ID = data; }
	QString				GetPlayOrder() { return m_PlayOrder; }
	void				SetPlayOrder(QString data) { m_PlayOrder = data; }
	QString				GetTitle() { return m_Title; }
	void				SetTitle(QString data) { m_Title = data; }
	QString				GetAttrHref() { return m_Href; }
	void				SetAttrHref(QString data) { m_Href = data; }

private:

	QString				m_ID;
	QString				m_PlayOrder;
	QString				m_Title;
	QString				m_Href;

};

#endif // TOCNCXL_H
