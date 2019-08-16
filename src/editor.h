//
// Created by jputlock on 6/1/19.
//

#ifndef NOTEX_EDITOR_H
#define NOTEX_EDITOR_H

#include <gtkmm.h>
#include <iostream>
#include <fstream>

#include "notexview.h"

class Editor : public Gtk::ApplicationWindow {

public:
    Editor();
    virtual ~Editor();

protected:
    //Signal handlers:
    void on_button_clicked();

    // Member items
    std::vector<NotexView*> editing_windows;
    NotexView* current_editing_window;
    Gtk::Fixed m_fixed;

    // menubar items
    Gtk::MenuBar m_menuBar;
    Gtk::MenuItem m_menuFile;
    Gtk::Menu m_subMenuFiles;
    Gtk::MenuItem m_menuNew, m_menuOpen, m_menuSave;

    // in case we need a separator in the submenu
    // Gtk::SeparatorMenuItem hline;
private:
    void init_menubar();
    void on_menu_file_new();
    void on_menu_file_open();
    void on_menu_file_save();
    bool on_keypress(GdkEventKey *event);
    void set_keybindings(void);
    void (Editor::*keybind_table[26])(void);
};



#endif //NOTEX_EDITOR_H
