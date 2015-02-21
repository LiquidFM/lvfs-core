/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2015 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#include "lvfs_core_qt_InteractiveEvents.h"

#include <lvfs-core/models/Qt/Node>

#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QDialogButtonBox>


namespace LVFS {
namespace Core {
namespace Qt {


namespace {

class PLATFORM_MAKE_PRIVATE UserInputDialog : public QDialog
{
public:
    enum Icon
    {
        Information,
        Warning,
        Critical,
        Question
    };

public:
    UserInputDialog(Icon icon, const QString &title, const QString &question, qint32 buttons, QWidget *parent = 0) :
        m_answer(QDialogButtonBox::NoButton),
        m_layout(this),
        m_text(question),
        m_buttonBox(QDialogButtonBox::StandardButtons(buttons))
    {
        setWindowTitle(title);

        m_vLayout.addWidget(&m_text);
        m_vLayout.addWidget(&m_lineEdit);

        m_iconLabel.setPixmap(standardIcon(icon, this));
        m_hLayout.addWidget(&m_iconLabel);
        m_hLayout.addLayout(&m_vLayout);

        m_layout.addLayout(&m_hLayout);
        m_layout.addWidget(&m_buttonBox);

        connect(&m_buttonBox, SIGNAL(accepted()),                this, SLOT(accept()));
        connect(&m_buttonBox, SIGNAL(rejected()),                this, SLOT(reject()));
        connect(&m_buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
    }

    int answer() const { return m_answer; }
    QString value() const { return m_lineEdit.text(); }

    static inline QPixmap standardIcon(Icon icon, QWidget *widget)
    {
        switch (icon)
        {
            case UserInputDialog::Information:
                return Node::standardIcon(QStyle::SP_MessageBoxInformation, QStyle::PM_MessageBoxIconSize, widget);

            case UserInputDialog::Warning:
                return Node::standardIcon(QStyle::SP_MessageBoxWarning, QStyle::PM_MessageBoxIconSize, widget);

            case UserInputDialog::Critical:
                return Node::standardIcon(QStyle::SP_MessageBoxCritical, QStyle::PM_MessageBoxIconSize, widget);

            case UserInputDialog::Question:
                return Node::standardIcon(QStyle::SP_MessageBoxQuestion, QStyle::PM_MessageBoxIconSize, widget);

            default:
                break;
        }

        return QPixmap();
    }

private Q_SLOTS:
    void buttonClicked(QAbstractButton *button)
    {
        m_answer = m_buttonBox.standardButton(button);
    }

private:
    int m_answer;
    QVBoxLayout m_layout;
    QHBoxLayout m_hLayout;
    QVBoxLayout m_vLayout;
    QLabel m_iconLabel;
    QLabel m_text;
    QLineEdit m_lineEdit;
    QDialogButtonBox m_buttonBox;
};

}


void QuestionEvent::showDialog(QWidget *parent)
{
    EFC::Mutex::Locker lock(m_result.m_mutex);
    m_result.m_answer = QMessageBox::question(parent, m_title, m_question, QMessageBox::StandardButtons(m_buttons));
    m_result.m_done = true;
    m_result.m_condition.wakeAll();
}

void UserInputEvent::showDialog(QWidget *parent)
{
    EFC::Mutex::Locker lock(m_result.m_mutex);
    UserInputDialog dialog(UserInputDialog::Question, m_title, m_question, m_buttons, parent);

    dialog.exec();
    m_result.m_answer = dialog.answer();
    m_result.m_value = dialog.value();
    m_result.m_done = true;
    m_result.m_condition.wakeAll();
}

}}}
