/*******************************************************************************
 *
 *    Copyright (C) 2015-2018 Greg McGarragh <greg.mcgarragh@colostate.edu>
 *
 *    This source code is licensed under the GNU General Public License (GPL),
 *    Version 3.  See the file COPYING for more details.
 *
 ******************************************************************************/

#ifndef XDFMAINWINDOW_H
#define XDFMAINWINDOW_H

#include <qfilesystemwatcher.h>
#include <qlineedit.h>
#include <qmainwindow.h>
#include <qsplitter.h>
#include <qtabwidget.h>

#include "xdfv.h"
#include "xdftabtreeview.h"
#include "xdftreeview.h"


class XDFMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ErrorCode {
        FileNotFound = 1,
        UnknownFileExtension,
        UnableToOpenFile
    };

private:
    static const size_t max_tab_size = 32;

    QFrame *find_frame;
    QLineEdit *find_line_edit;
    QFileSystemWatcher *file_watcher;

    XDFTabTreeView *tab_tree_view;

    char *cut_fn(const char *in, char *out);

    XDFV::FileType file_type_from_extension(QString file_name);

public:
    XDFMainWindow(QWidget *parent = 0);
    ~XDFMainWindow();

    XDFTabTreeView *tabTreeView();

public slots:
    void openFile();
    void openFile(const char *file_name, int flag);
    void openFile(XDFV::FileType file_type, const char *file_name, int flag);
    void reloadFile(XDFTreeView *view);
    void reloadFile(const QString &file_name);
    void reloadCurrentFile();

    void find();
    void findPrev();
    void findAll();

    void showAbout();
};

#endif /* XDFMAINWINDOW_H */
