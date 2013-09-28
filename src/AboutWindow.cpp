/* Copyright (C) 2013  Marco Heisig

This file is part of Rabenstein.

Rabenstein is free software: you can redistribute it and/or modify it under the
terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
details.

You should have received a copy of the GNU Affero General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include "AboutWindow.hpp"
#include "config.hpp"

namespace Rabenstein {
AboutWindow::AboutWindow(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("About Rabenstein"));
    QPushButton* button = new QPushButton(tr("OK"), this);
    QVBoxLayout* vlayout = new QVBoxLayout(this);
    QLabel* text = new QLabel(this);
    button->setDefault(true);
    text->setWordWrap(true);
    text->setOpenExternalLinks(true);
    text->setText(tr("Rabenstein") +
                  tr("<br/><br/> Version: %1")
                  .arg(Rabenstein::version.c_str()) +
                  tr("<br/><br/>Rabenstein is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.<br/>"));
    connect(button, SIGNAL(clicked()),
            this, SLOT(reject()));
    vlayout->addWidget(text);
    vlayout->addWidget(button);
}

AboutWindow::~AboutWindow() {}

}
