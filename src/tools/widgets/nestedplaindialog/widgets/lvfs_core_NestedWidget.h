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

#ifndef LVFS_CORE_NESTEDWIDGET_H_
#define LVFS_CORE_NESTEDWIDGET_H_

#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <lvfs-core/tools/widgets/BaseNestedWidget>


class PLATFORM_MAKE_PUBLIC NestedWidget : public BaseNestedWidget
{
public:
    enum DialogCode
    {
    	Rejected,
    	Accepted
    };

public:
	NestedWidget(NestedDialog *parent, const QString &title);
	virtual ~NestedWidget();

	/* BaseNestedWidget */
	virtual QWidget *centralWidget();
	virtual void setReadOnly(bool value);

	int exec();

	QString title() const { return m_groupbox.title(); }

protected:
	void setCentralWidget(QWidget *widget)
	{
		Q_ASSERT_X
		(
			m_hlayout.count() == 1,
			"NestedWidget::setCentralWidget",
			"Central widget already set!"
		);

		m_hlayout.addWidget(widget, 1);
	}

private:
    class Button : public QPushButton
    {
    public:
    	Button(const QString &text);

        virtual QSize sizeHint() const;
        virtual QSize minimumSizeHint() const;
    };

private:
	QGroupBox m_groupbox;
	QHBoxLayout m_hlayout;
	QVBoxLayout m_vlayout;
	Button m_ok;
	Button m_cancel;
};

#endif /* LVFS_CORE_NESTEDWIDGET_H_ */
