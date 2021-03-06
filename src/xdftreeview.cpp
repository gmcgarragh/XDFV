/*******************************************************************************
 *
 *    Copyright (C) 2015-2018 Greg McGarragh <greg.mcgarragh@colostate.edu>
 *
 *    This source code is licensed under the GNU General Public License (GPL),
 *    Version 3.  See the file COPYING for more details.
 *
 ******************************************************************************/

#include <qaction.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qmenu.h>

#include "xdfv.h"
#include "xdftreeview.h"


XDFTreeViewItem::XDFTreeViewItem(XDFTreeView *parent, const char *name_)
    : QTreeWidgetItem((QTreeWidget *) parent), has_data_table(false)
{
    name = strdup(name_);
}



XDFTreeViewItem::XDFTreeViewItem(XDFTreeViewItem *parent, XDFTreeViewItem *after, const char *name_)
    : QTreeWidgetItem((QTreeWidgetItem *) parent, (QTreeWidgetItem *) after), has_data_table(false)
{
    name = strdup(name_);
}



XDFTreeViewItem::~XDFTreeViewItem()
{
    free(name);
}



bool XDFTreeViewItem::hasDataTable()
{
    return has_data_table;
}



void XDFTreeViewItem::setHasDataTable(bool has)
{
    has_data_table = has;
}



XDFTreeView::XDFTreeView(const char *file_name_, XDFV::FileType file_type, QWidget *parent)
    : QTreeWidget(parent), file_type(file_type)
{
    file_name = strdup(file_name_);
/*
    setAutoScroll(false);
*/
    setRootIsDecorated(true);
    setSortingEnabled(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu(const QPoint &)));
}



XDFTreeView::~XDFTreeView()
{
    free(file_name);
}



const char *XDFTreeView::filename()
{
    return file_name;
}



XDFV::FileType XDFTreeView::fileType()
{
    return file_type;
}



void XDFTreeView::load()
{

}



void XDFTreeView::showContextMenu(const QPoint &point)
{
    QMenu menu(this);

    QAction *copy_item_name_action = new QAction("Copy item name", this);
    connect(copy_item_name_action, SIGNAL(triggered()), this, SLOT(copyItemName()));
    menu.addAction(copy_item_name_action);

    menu.addSeparator();

    QAction *view_data_table_action = new QAction("View data table", this);
    view_data_table_action->setEnabled(((XDFTreeViewItem *) currentItem())->hasDataTable());
    connect(view_data_table_action, SIGNAL(triggered()), this, SLOT(showDataTable()));
    menu.addAction(view_data_table_action);

    menu.exec(mapToGlobal(point));
}



void XDFTreeView::copyItemName()
{
    copyItemName((XDFTreeViewItem *) currentItem(), 0);
}



void XDFTreeView::copyItemName(XDFTreeViewItem *item, int column)
{
    QApplication::clipboard()->setText(item->name, QClipboard::Clipboard);
    QApplication::clipboard()->setText(item->name, QClipboard::Selection);
}



void XDFTreeView::find(QString &name)
{
    QTreeWidgetItemIterator it(this);

    if (currentItem()) {
        it = QTreeWidgetItemIterator(currentItem());
        it++;
    }

    while (*it) {
        if ((*it)->text(0).contains(name)) {
            setCurrentItem(*it);
            return;
        }
        ++it;
    }
}



void XDFTreeView::findPrev(QString &name)
{
    QTreeWidgetItemIterator it(this);

    if (currentItem()) {
        it = QTreeWidgetItemIterator(currentItem());
        it--;
    }

    while (*it) {
        if ((*it)->text(0).contains(name)) {
            setCurrentItem(*it);
            return;
        }
        --it;
    }
}



void XDFTreeView::selectAll(QString &name)
{
    bool flag = true;

    QTreeWidgetItemIterator it(this);

    while (*it) {
        if ((*it)->text(0).contains(name)) {
            if (flag) {
                setCurrentItem(*it);
                flag = false;
            }
            else
                (*it)->setSelected(true);
        }

        ++it;
    }
}



void XDFTreeView::expandAll()
{
    QTreeWidgetItemIterator it(this);

    while (*it) {
        expandItem(*it);
        ++it;
    }
}



void XDFTreeView::collapseAll()
{
    QTreeWidgetItemIterator it(this);

    while (*it) {
        collapseItem(*it);
        ++it;
    }
}



void XDFTreeView::showDataTable()
{
    if (currentItem() != NULL)
        showDataTable((XDFTreeViewItem *) currentItem(), 0);
}



void XDFTreeView::showDataTable(XDFTreeViewItem *item, int column)
{

}



void XDFTreeView::setFontSize(int size)
{
    if (size < 1)
        return;

    QFont font(this->font());
    font.setPointSize(size);
    setFont(font);
}



void XDFTreeView::changeFontSize(int delta)
{
    QFont font(this->font());

    if (font.pointSize() + delta < 1)
        return;

    font.setPointSize(font.pointSize() + delta);
    setFont(font);
}



void XDFTreeView::mousePressEvent(QMouseEvent *event)
{
    setAutoScroll(false);
    QTreeWidget::mousePressEvent(event);
    setAutoScroll(true);
}



void XDFTreeView::colorize(QTreeWidgetItem *item, bool color)
{

}



void XDFTreeView::colorizeAll(bool color)
{
    QTreeWidgetItemIterator it(this);

    while (*it) {
        colorize(*it, color);
        ++it;
    }
}
