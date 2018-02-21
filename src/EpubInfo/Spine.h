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
#ifndef SPINE_H
#define SPINE_H

#include <QtCore/QObject>

class SpineItem;

class Spine : public QObject
{
	Q_OBJECT

public:
	explicit Spine(QObject *parent = 0);
	~Spine();

	QString				GetTocInfo() { return m_TocInfo; }
	void				SetTocInfo(QString data) { m_TocInfo = data; }
	QList<SpineItem*>	GetSpineItemList() { return m_SpineItemList; }
	SpineItem*			GetSpineItemByIDRef(QString idref);
	bool				SetSpineItem(QString idref, QString linear = "yes");

private:

	QString				m_TocInfo;
	QList<SpineItem*>	m_SpineItemList;

};


class SpineItem : public QObject
{
	Q_OBJECT

public:
	explicit SpineItem(QObject *parent = 0);
	~SpineItem();

	QString				GetIDRef() { return m_IDRef; }
	void				SetIDRef(QString data) { m_IDRef = data; }
	QString				GetLinear() { return m_Linear; }
	void				SetLinear(QString data) { m_Linear = data; }
private:

	QString				m_IDRef;
	QString				m_Linear;

};

#endif // SPINE_H
