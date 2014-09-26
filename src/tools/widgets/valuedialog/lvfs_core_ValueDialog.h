/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * lvfs-core is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lvfs-core is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lvfs-core. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LVFS_CORE_VALUEDIALOG_H_
#define LVFS_CORE_VALUEDIALOG_H_

#include <QtCore/QVariant>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QMessageBox>
#include <QtGui/QSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QDateTimeEdit>


/* Select editor meta-function */
template <typename Value> struct Editor {};

template <> struct Editor<QString>        { typedef QLineEdit     type; typedef QString        value_type; };
template <> struct Editor<char>           { typedef QSpinBox      type; typedef char           value_type; };
template <> struct Editor<unsigned char>  { typedef QSpinBox      type; typedef unsigned char  value_type; };
template <> struct Editor<short>          { typedef QSpinBox      type; typedef short          value_type; };
template <> struct Editor<unsigned short> { typedef QSpinBox      type; typedef unsigned short value_type; };
template <> struct Editor<int>            { typedef QSpinBox      type; typedef int            value_type; };
template <> struct Editor<unsigned int>   { typedef QSpinBox      type; typedef unsigned int   value_type; };
template <> struct Editor<long>           { typedef QSpinBox      type; typedef long           value_type; };
template <> struct Editor<unsigned long>  { typedef QSpinBox      type; typedef unsigned long  value_type; };
template <> struct Editor<QDate>          { typedef QDateEdit     type; typedef QDate          value_type; };
template <> struct Editor<QTime>          { typedef QTimeEdit     type; typedef QTime          value_type; };
template <> struct Editor<QDateTime>      { typedef QDateTimeEdit type; typedef QDateTime      value_type; };


/* Editor value meta-function */
template <typename T> struct EditorValue {};

template <> struct EditorValue<QString>
{
    typedef Editor<QString>                     EditorBaseType;
    typedef const typename EditorBaseType::type ConstEditorType;
    typedef typename EditorBaseType::type       EditorType;
    typedef typename EditorBaseType::value_type Value;

    static Value value(const QWidget *editor)
    {
        return static_cast<ConstEditorType*>(editor)->text().simplified();
    }
    static void setValue(QWidget *editor, const Value &value)
    {
        static_cast<EditorType*>(editor)->setText(value);
    }
    static void setValue(QWidget *editor, const QVariant &value)
    {
        static_cast<EditorType*>(editor)->setText(value.value<Value>());
    }
};

template <> struct EditorValue<char>
{
    typedef Editor<char>                        EditorBaseType;
    typedef const typename EditorBaseType::type ConstEditorType;
    typedef typename EditorBaseType::type       EditorType;
    typedef typename EditorBaseType::value_type Value;

    static Value value(const QWidget *editor)
    {
        return static_cast<ConstEditorType*>(editor)->value();
    }
    static void setValue(QWidget *editor, Value value)
    {
        static_cast<EditorType*>(editor)->setValue(value);
    }
    static void setValue(QWidget *editor, const QVariant &value)
    {
        static_cast<EditorType*>(editor)->setValue(value.value<Value>());
    }
};

template <> struct EditorValue<int>
{
    typedef Editor<int>                         EditorBaseType;
    typedef const typename EditorBaseType::type ConstEditorType;
    typedef typename EditorBaseType::type       EditorType;
    typedef typename EditorBaseType::value_type Value;

    static Value value(const QWidget *editor)
    {
        return static_cast<ConstEditorType*>(editor)->value();
    }
    static void setValue(QWidget *editor, Value value)
    {
        static_cast<EditorType*>(editor)->setValue(value);
    }
    static void setValue(QWidget *editor, const QVariant &value)
    {
        static_cast<EditorType*>(editor)->setValue(value.value<Value>());
    }
};

template <> struct EditorValue<QDate>
{
    typedef Editor<QDate>                       EditorBaseType;
    typedef const typename EditorBaseType::type ConstEditorType;
    typedef typename EditorBaseType::type       EditorType;
    typedef typename EditorBaseType::value_type Value;

    static Value value(const QWidget *editor)
    {
        return static_cast<ConstEditorType*>(editor)->date();
    }
    static void setValue(QWidget *editor, const Value &value)
    {
        static_cast<EditorType*>(editor)->setDate(value);
    }
    static void setValue(QWidget *editor, const QVariant &value)
    {
        static_cast<EditorType*>(editor)->setDate(value.value<Value>());
    }
};

template <> struct EditorValue<QTime>
{
    typedef Editor<QTime>                       EditorBaseType;
    typedef const typename EditorBaseType::type ConstEditorType;
    typedef typename EditorBaseType::type       EditorType;
    typedef typename EditorBaseType::value_type Value;

    static Value value(const QWidget *editor)
    {
        return static_cast<ConstEditorType*>(editor)->time();
    }
    static void setValue(QWidget *editor, const Value &value)
    {
        static_cast<EditorType*>(editor)->setTime(value);
    }
    static void setValue(QWidget *editor, const QVariant &value)
    {
        static_cast<EditorType*>(editor)->setTime(value.value<Value>());
    }
};

template <> struct EditorValue<QDateTime>
{
    typedef Editor<QDateTime>                   EditorBaseType;
    typedef const typename EditorBaseType::type ConstEditorType;
    typedef typename EditorBaseType::type       EditorType;
    typedef typename EditorBaseType::value_type Value;

    static Value value(const QWidget *editor)
    {
        return static_cast<ConstEditorType*>(editor)->dateTime();
    }
    static void setValue(QWidget *editor, const Value &value)
    {
        static_cast<EditorType*>(editor)->setDateTime(value);
    }
    static void setValue(QWidget *editor, const QVariant &value)
    {
        static_cast<EditorType*>(editor)->setDateTime(value.value<Value>());
    }
};


template <typename T>
class ValueDialog : public QDialog
{
public:
    ValueDialog(const QString &title, const QString &label, const T &value = T(), QWidget *parent = 0);

    virtual void accept();
    T value() const;

private:
    typedef typename Editor<T>::type Editor;

private:
    QLabel m_label;
    Editor m_edit;
    QGridLayout m_gridLayout;
    QDialogButtonBox m_buttonBox;
};


template <typename T>
ValueDialog<T>::ValueDialog(const QString &title, const QString &label, const T &value, QWidget *parent) :
	QDialog(parent),
	m_label(this),
	m_edit(this),
	m_gridLayout(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	setWindowTitle(title);

	QFont font;
	font.setBold(true);
	font.setWeight(75);
	m_label.setFont(font);
	m_label.setText(label);

	m_gridLayout.setSpacing(6);
	m_gridLayout.setContentsMargins(6, 6, 6, 6);
	m_gridLayout.addWidget(&m_label, 0, 0, 1, 1);
	m_gridLayout.addWidget(&m_edit, 0, 1, 1, 1);
	m_gridLayout.addWidget(&m_buttonBox, 1, 0, 1, 2);

	connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	EditorValue<T>::setValue(&m_edit, value);
	m_edit.selectAll();
}

template <typename T>
T ValueDialog<T>::value() const
{
	return EditorValue<T>::value(&m_edit);
}

template <typename T>
void ValueDialog<T>::accept()
{
	if (value() == T())
		QMessageBox::information(this, windowTitle(), tr("You must enter the value!"));
	else
		QDialog::accept();
}

#endif /* LVFS_CORE_VALUEDIALOG_H_ */
