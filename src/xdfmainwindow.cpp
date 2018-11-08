/*******************************************************************************
 *
 *    Copyright (C) 2015-2018 Greg McGarragh <greg.mcgarragh@colostate.edu>
 *
 *    This source code is licensed under the GNU General Public License (GPL),
 *    Version 3.  See the file COPYING for more details.
 *
 ******************************************************************************/

#include <hdf.h>
#include <hdf5.h>
#include <netcdf.h>

#include <qboxlayout.h>
#include <qdialog.h>
#include <qfiledialog.h>
#include <qframe.h>
#include <qlabel.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpushbutton.h>

#include "xdfv.h"
#include "version.h"
#include "hdftreeview.h"
#include "hdf5treeview.h"
#include "nctreeview.h"
#include "xdfmainwindow.h"
#include "xdftreeview.h"


XDFMainWindow::XDFMainWindow(QWidget *parent)
     : QMainWindow(parent)
{
    QMenuBar *menu_bar;

    QMenu *file_menu;
    QAction *open_file_action;
    QAction *quit_action;

    QMenu *edit_menu;
    QAction *copy_item_name_action;

    QMenu *find_menu;
    QAction *find_action;
    QAction *find_prev_action;
    QAction *find_all_action;

    QMenu *view_menu;
    QAction *expand_all_action;
    QAction *expand_all_tabs_action;
    QAction *collapse_all_action;
    QAction *collapse_all_tabs_action;
    QAction *view_current_data_action;
/*
    QAction *split_horizontally_action;
    QAction *split_vertically_action;
*/
    QAction *close_current_tab_action;

    QMenu *preferences_menu;

    QAction *increase_font_size_action;
    QAction *decrease_font_size_action;
    QAction *default_font_size_action;
    QAction *view_in_color_action;

    QMenu *help_menu;
    QAction *about_action;

    QWidget *central_widget;
    QVBoxLayout *vertical_layout;
    QHBoxLayout *find_horiz_layout;
    QPushButton *find_push_button;
    QPushButton *find_prev_push_button;
    QPushButton *find_all_push_button;
    QPushButton *find_close_push_button;


    setAttribute(Qt::WA_DeleteOnClose, true);


    menu_bar = new QMenuBar(this);


    /* File menu */

    file_menu = new QMenu(menu_bar);
    file_menu->setTitle("File");

    open_file_action = file_menu->addAction("Open file");
    open_file_action->setShortcut(QKeySequence("Ctrl+o"));
    QObject::connect(open_file_action, SIGNAL(triggered()), this, SLOT(openFile()));

    quit_action = file_menu->addAction("Quit");
    quit_action->setShortcut(QKeySequence("Ctrl+q"));
    QObject::connect(quit_action, SIGNAL(triggered()), this, SLOT(close()));

    /* Edit menu */

    edit_menu = new QMenu(menu_bar);
    edit_menu->setTitle("Edit");

    copy_item_name_action = edit_menu->addAction("Copy item name");
    copy_item_name_action->setShortcut(QKeySequence("Ctrl+c"));

    /* Find menu */
    find_menu = new QMenu(menu_bar);
    find_menu->setTitle("Find");

    find_action = find_menu->addAction("Find");
    find_action->setShortcut(QKeySequence("Ctrl+f"));

    find_prev_action = find_menu->addAction("Find previous");
    find_prev_action->setShortcut(QKeySequence("F3"));

    find_all_action = find_menu->addAction("Find all");
    find_all_action->setShortcut(QKeySequence("Ctrl+F3"));

    /* View menu */
    view_menu = new QMenu(menu_bar);
    view_menu->setTitle("View");

    expand_all_action = view_menu->addAction("Expand all");
    expand_all_action->setShortcut(QKeySequence("Ctrl+e"));

    expand_all_tabs_action = view_menu->addAction("Expand all tabs");
    expand_all_tabs_action->setShortcut(QKeySequence("Alt+e"));

    collapse_all_action = view_menu->addAction("Collapse all");
    collapse_all_action->setShortcut(QKeySequence("Ctrl+d"));

    collapse_all_tabs_action = view_menu->addAction("Collapse all tabs");
    collapse_all_tabs_action->setShortcut(QKeySequence("Alt+d"));

    view_menu->addSeparator();
/*
    split_horizontally_action = view_menu->addAction("Split Horizontally");
    split_horizontally_action->setShortcut(QKeySequence("Ctrl+h"));

    split_vertically_action = view_menu->addAction("Split Vertically");
    split_vertically_action->setShortcut(QKeySequence("Ctrl+v"));

    view_menu->addSeparator();
*/
    view_current_data_action = view_menu->addAction("View data table");
    view_current_data_action->setShortcut(QKeySequence(Qt::Key_Space));

    view_menu->addSeparator();

    close_current_tab_action = view_menu->addAction("Close current tab");
    close_current_tab_action->setShortcut(QKeySequence("Ctrl+t"));

    /* Preferences menu */

    preferences_menu = new QMenu(menu_bar);
    preferences_menu->setTitle("Preferences");

    increase_font_size_action = preferences_menu->addAction("Increase font size");
    increase_font_size_action->setShortcut(QKeySequence("Ctrl+="));

    decrease_font_size_action = preferences_menu->addAction("Decrease font size");
    decrease_font_size_action->setShortcut(QKeySequence("Ctrl+-"));

    default_font_size_action = preferences_menu->addAction("Defaut font size");
    default_font_size_action->setShortcut(QKeySequence("Ctrl+\\"));

    preferences_menu->addSeparator();

    view_in_color_action = preferences_menu->addAction("View in color");
    view_in_color_action->setCheckable(true);
    view_in_color_action->setShortcut(QKeySequence("Ctrl+v"));

    /* Help menu */
    help_menu = new QMenu(menu_bar);
    help_menu->setTitle("Help");

    about_action = help_menu->addAction("About " + QString(PROGRAM_NAME));
    QObject::connect(about_action, SIGNAL(triggered()), this, SLOT(showAbout()));


    menu_bar->addMenu(file_menu);
    menu_bar->addMenu(edit_menu);
    menu_bar->addMenu(find_menu);
    menu_bar->addMenu(view_menu);
    menu_bar->addMenu(preferences_menu);
    menu_bar->addMenu(help_menu);
    setMenuBar(menu_bar);


    central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    vertical_layout = new QVBoxLayout(central_widget);
    vertical_layout->setSpacing(0);
    vertical_layout->setContentsMargins(0,0,0,0);

    tab_tree_view = new XDFTabTreeView(this);
    vertical_layout->addWidget(tab_tree_view);

    find_frame = new QFrame(central_widget);
    find_frame->hide();
    vertical_layout->addWidget(find_frame);

    find_horiz_layout = new QHBoxLayout(find_frame);
    find_horiz_layout->setSpacing(0);
    find_horiz_layout->setContentsMargins(0,0,0,0);

    find_line_edit = new QLineEdit(find_frame);
    find_horiz_layout->addWidget(find_line_edit);

    find_push_button = new QPushButton("Find", find_frame);
    find_horiz_layout->addWidget(find_push_button);
    find_prev_push_button = new QPushButton("Find Prev", find_frame);
    find_horiz_layout->addWidget(find_prev_push_button);
    find_all_push_button = new QPushButton("Find All", find_frame);
    find_horiz_layout->addWidget(find_all_push_button);
    find_close_push_button = new QPushButton("Close", find_frame);

    find_horiz_layout->addWidget(find_close_push_button);


    QObject::connect(copy_item_name_action,     SIGNAL(triggered()),   tab_tree_view, SLOT(copyItemName()));

    QObject::connect(find_action,               SIGNAL(triggered()),   this,          SLOT(find()));
    QObject::connect(find_push_button,          SIGNAL(clicked()),     this,          SLOT(find()));
    QObject::connect(find_prev_action,          SIGNAL(triggered()),   this,          SLOT(findPrev()));
    QObject::connect(find_prev_push_button,     SIGNAL(clicked()),     this,          SLOT(findPrev()));
    QObject::connect(find_all_action,           SIGNAL(triggered()),   this,          SLOT(findAll()));
    QObject::connect(find_all_push_button,      SIGNAL(clicked()),     this,          SLOT(findAll()));
    QObject::connect(find_close_push_button,    SIGNAL(clicked()),     find_frame,    SLOT(hide()));

    QObject::connect(expand_all_action,         SIGNAL(triggered()),   tab_tree_view, SLOT(expandAll()));
    QObject::connect(expand_all_tabs_action,    SIGNAL(triggered()),   tab_tree_view, SLOT(expandAllTabs()));
    QObject::connect(collapse_all_action,       SIGNAL(triggered()),   tab_tree_view, SLOT(collapseAll()));
    QObject::connect(collapse_all_tabs_action,  SIGNAL(triggered()),   tab_tree_view, SLOT(collapseAllTabs()));
    QObject::connect(view_current_data_action,  SIGNAL(triggered()),   tab_tree_view, SLOT(showDataTable()));
    QObject::connect(close_current_tab_action,  SIGNAL(triggered()),   tab_tree_view, SLOT(closeCurrentTab()));

    QObject::connect(increase_font_size_action, SIGNAL(triggered()),   tab_tree_view, SLOT(increaseFontSize()));
    QObject::connect(decrease_font_size_action, SIGNAL(triggered()),   tab_tree_view, SLOT(decreaseFontSize()));
    QObject::connect(default_font_size_action,  SIGNAL(triggered()),   tab_tree_view, SLOT(useDefaultFontSize()));
    QObject::connect(view_in_color_action,      SIGNAL(toggled(bool)), tab_tree_view, SLOT(setColorized(bool)));
    QObject::connect(tab_tree_view,             SIGNAL(colorizedChanged(bool)), view_in_color_action, SLOT(setChecked(bool)));
}



XDFMainWindow::~XDFMainWindow()
{

}



XDFTabTreeView *XDFMainWindow::tabTreeView()
{
    return tab_tree_view;
}



char *XDFMainWindow::cut_fn(const char *in, char *out)
{
    char *pos;

    pos = (char *) in;
    if (! (pos = strrchr((char *) in, '/')))
        pos = (char *) in;
    else
        pos++;

    strcpy(out, pos);

    if (strlen(out) > max_tab_size) {
        out[max_tab_size] = '\0';
        strcpy(out + max_tab_size - 3, "...");
    }

    return out;
}



XDFMainWindow::FileType XDFMainWindow::file_type_from_extension(QString file_name)
{
    FILE *fp;

    QFileInfo fi(file_name);

    if (fi.suffix() == "nc")
        return XDFMainWindow::NetCDF;

    fp = fopen(file_name.toLatin1().data(), "r");
    if (fp == NULL)
        throw FileNotFound;
    fclose(fp);

    if (H5Fis_hdf5(file_name.toLatin1().data()) > 0)
        return XDFMainWindow::HDF5;

    if (fi.suffix() == "h5")
        return XDFMainWindow::HDF5;

    if (fi.suffix() == "hdf")
        return XDFMainWindow::HDF4;

    return XDFMainWindow::Unknown;
}



void XDFMainWindow::openFile()
{
    QMessageBox messageBox;
    QStringList file_names;

    XDFMainWindow::FileType file_type = XDFMainWindow::Unknown;

    file_names = QFileDialog::getOpenFileNames(this, "", "", "(*.hdf *.h5 *.nc)");

    for (int i = 0; i < file_names.size(); ++i) {
        try {
            if ((file_type = file_type_from_extension(file_names.at(i))) == XDFMainWindow::Unknown) {
                messageBox.critical(this, "XDFV Error", "Unknown file extension.");
                return;
            }
        }
        catch (ErrorCode e) {
            if (e == FileNotFound) {
                messageBox.critical(this, "XDFV Error", "File does not exist.");
                return;
            }
        }
        try {
            openFile(file_type, file_names.at(i).toLatin1().data(), 0);
        }
        catch (ErrorCode e) {
            if (e == FileNotFound)
                messageBox.critical(this, "XDFV Error", "File does not exist.");
            else if (e == UnableToOpenFile)
                messageBox.critical(this, "XDFV Error", "Unable to open file, invalid format or file corrupt.");
        }
    }
}



void XDFMainWindow::openFile(const char *file_name, int flag)
{
    XDFMainWindow::FileType file_type;

    try {
        if ((file_type = file_type_from_extension(file_name)) == XDFMainWindow::Unknown)
            throw UnknownFileExtension;
    }
    catch (int e) {
        throw e;
    }
    try {
        openFile(file_type, file_name, flag);
    }
    catch (int e) {
        throw e;
    }
}



void XDFMainWindow::openFile(XDFMainWindow::FileType file_type, const char *file_name, int flag)
{
    char *temp;

    int index;

    XDFTreeView *xdf_tree_view = NULL;

    try {
        if (file_type == XDFMainWindow::HDF4)
            xdf_tree_view = new HDFTreeView(file_name, flag, tabTreeView());
        else if (file_type == XDFMainWindow::HDF5)
            xdf_tree_view = new HDF5TreeView(file_name, tabTreeView());
        else if (file_type == XDFMainWindow::NetCDF)
            xdf_tree_view = new NCTreeView  (file_name, tabTreeView());
        else {
            fprintf(stderr, "ERROR: Unknown file type\n");
            exit(1);
        }
    }
    catch (int e) {
        if (e == XDFProcessor::FileNotFound)
            throw FileNotFound;
        else if (e == XDFProcessor::UnableToOpenFile)
            throw UnableToOpenFile;
    }

    xdf_tree_view->setFontSize(tabTreeView()->getFontSize());
    xdf_tree_view->setColorized(tabTreeView()->isColorized());

    temp = (char *) malloc(LN * sizeof(char));
    index = tabTreeView()->addTab(xdf_tree_view, cut_fn(file_name, temp));
    tabTreeView()->setTabToolTip(index, file_name);
    free(temp);
}



void XDFMainWindow::find()
{
    if (! find_frame->isVisible()) {
        find_frame->show();
        return;
    }

    QString name = find_line_edit->text();

    if (name.trimmed().isEmpty())
        return;

    tabTreeView()->find(name);
}



void XDFMainWindow::findPrev()
{
    if (! find_frame->isVisible()) {
        find_frame->show();
        return;
    }

    QString name = find_line_edit->text();

    if (name.trimmed().isEmpty())
        return;

    tabTreeView()->findPrev(name);
}



void XDFMainWindow::findAll()
{
    if (! find_frame->isVisible()) {
        find_frame->show();
        return;
    }

    QString name = find_line_edit->text();

    if (name.trimmed().isEmpty())
        return;

    tabTreeView()->selectAll(name);
}



void XDFMainWindow::showAbout()
{
    QDialog about(this);
    QVBoxLayout layout(&about);
    layout.setSpacing(15);

    QLabel label0("<font size = 10>" + QString(PROGRAM_NAME) + "</font>", &about);
    label0.setAlignment(Qt::AlignHCenter);
    layout.addWidget(&label0);

    QLabel label1("X (Network Common | Hierarchical 4 | Hierarchical 5)<br>"
                  "Data Format Viewer", &about);
    label1.setAlignment(Qt::AlignHCenter);
    layout.addWidget(&label1);

    QLabel label3("Build SHA-1: " + QString(build_sha_1), &about);
    label3.setAlignment(Qt::AlignHCenter);
    layout.addWidget(&label3);

    QLabel label2("Build date: "  + QString(build_date), &about);
    label2.setAlignment(Qt::AlignHCenter);
    layout.addWidget(&label2);

    QLabel label4("Copyright (C) 2015-2018 Greg McGarragh", &about);
    label4.setAlignment(Qt::AlignHCenter);
    layout.addWidget(&label4);

    int n = 0;
    char temp[LN];
    char temp2[80];

    n += snprintf(temp+n, LN-n, "Linked with:<br>");

    strcpy(temp2, nc_inq_libvers());
    *(strstr(temp2, "of") - 1) = '\0';
    n += snprintf(temp+n, LN-n, "NetCDF: %s, ", temp2);

    uint32 major_v;
    uint32 minor_v;
    uint32 release;
    Hgetlibversion(&major_v, &minor_v, &release, temp2);
    n += snprintf(temp+n, LN-n, "HDF: %d.%d.%d, ", major_v, minor_v, release);

    unsigned majnum;
    unsigned minnum;
    unsigned relnum;
    H5get_libversion(&majnum, &minnum, &relnum);
    n += snprintf(temp+n, LN-n, "HDF5: %d.%d.%d, ", majnum, minnum, relnum);

    n += snprintf(temp+n, LN-n, "Qt: %s", qVersion());

    QLabel label5(temp, &about);
    label5.setAlignment(Qt::AlignHCenter);
    layout.addWidget(&label5);

    about.exec();
}