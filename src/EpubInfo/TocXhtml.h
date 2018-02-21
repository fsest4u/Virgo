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
#ifndef TOCXHTML_H
#define TOCXHTML_H

#include <QtCore/QObject>

class NavItem;
class TocXHtmlItem;

class TocXhtml : public QObject
{
    Q_OBJECT

public:
    explicit TocXhtml(QObject *parent = 0);
    ~TocXhtml();

	QString				GetDocTitle() { return m_DocTitle; }
	void				SetDocTitle(QString data) { m_DocTitle = data; }
	QString				GetSectionTitle() { return m_SectionTitle; }
	void				SetSectionTitle(QString data) { m_SectionTitle = data; }
	QList<NavItem*>		GetNavItemList() { return m_NavItemList; }
	NavItem*			GetNavItem(QString type);
	bool				SetNavItem(QString type, QString id, QString title = "");


private:

	QString				m_DocTitle;
	QString				m_SectionTitle;
	QList<NavItem*>		m_NavItemList;

};

class NavItem : public QObject
{
	Q_OBJECT

public:
	explicit NavItem(QObject *parent = 0);
	~NavItem();

	QString 				GetAttrType() { return m_Type; }
	void					SetAttrType(QString data) { m_Type = data; }
	QString					GetAttrID() { return m_ID; }
	void					SetAttrID(QString data) { m_ID = data; }
	QString					GetTitle() { return m_Title; }
	void					SetTitle(QString data) { m_Title = data; }
	QList<TocXHtmlItem*>	GetTocXHtmlItemList() { return m_TocHtmlItemList; }
	bool					SetTocXHtmlItem(QString title, QString src);

private:

	QString					m_Type;
	QString					m_ID;
	QString					m_Title;
	QList<TocXHtmlItem*>	m_TocHtmlItemList;

};


class TocXHtmlItem : public QObject
{
	Q_OBJECT

public:
	explicit TocXHtmlItem(QObject *parent = 0);
	~TocXHtmlItem();

	QString				GetTitle() { return m_Title; }
	void				SetTitle(QString data) { m_Title = data; }
	QString				GetAttrHref() { return m_Href; }
	void				SetAttrHref(QString data) { m_Href = data; }

private:

	QString				m_Title;
	QString				m_Href;

};

#endif // TOCXHTML_H
