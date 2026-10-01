// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray

#include "albums_tab.h"
#include <iostream>

AlbumsTab::AlbumsTab()
{
    window.set_child(tree);
    tree.set_enable_search(false);
    tree.set_rubber_banding(false);

    window.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    window.set_expand();

    box.append(window);

    std::vector<Glib::ustring> album_column_headers = {"Artist", "Album"};
    tree_refrence                                   = setupStringTreeView(tree, collumns, album_column_headers);

    auto mouse_click = Gtk::GestureClick::create();
    mouse_click->set_button(GDK_BUTTON_PRIMARY);
    mouse_click->signal_pressed().connect(sigc::mem_fun(*this, &AlbumsTab::onLeftClick));
    tree.add_controller(mouse_click);

    mouse_click = Gtk::GestureClick::create();
    mouse_click->set_button(GDK_BUTTON_SECONDARY);
    mouse_click->signal_pressed().connect(sigc::mem_fun(*this, &AlbumsTab::onRightClick));
    tree.add_controller(mouse_click);

    Gtk::Button append_button;
    append_button.set_label("button here");
    append_button.set_hexpand();
    append_button.signal_clicked().connect(sigc::mem_fun(*this, &AlbumsTab::appendButtonClick));

    Gtk::Box button_box;
    button_box.set_name("button_box");
    button_box.append(append_button);

    popup.set_parent(box);
    popup.set_child(button_box);
    popup.set_has_arrow(false);
}

void AlbumsTab::update()
{
    if (!(tree_refrence->children().size() == 0))
        tree_refrence->clear();
    for (AlbumEntry &album : player_->albums)
    {
        auto row                        = *(tree_refrence->append());
        row[collumns.string_columns[0]] = album.artist;
        row[collumns.string_columns[1]] = album.title;
    }
}

void AlbumsTab::cleanup()
{
    popup.unparent();
}

// Returns -1 on bad value
int AlbumsTab::getSelectedSong(double x, double y)
{
    double offset_y = y - tree.get_column(0)->get_button()->get_allocation().get_height();

    Gtk::TreeModel::Path path;

    if (!tree.get_path_at_pos(static_cast<int>(x), static_cast<int>(offset_y), path))
        return -1;

    int selected_index = path[0];

    if (selected_index < 0 || selected_index >= player_->albums.size())
        return -1;

    return selected_index;
}

void AlbumsTab::onLeftClick(int n_press, double x, double y)
{
    int selected_index = getSelectedSong(x, y);
    if (selected_index == -1)
        return;

    player_->clearQueue();
    player_->addSongsToQueue(player_->albums[selected_index].songs);
}


void AlbumsTab::onRightClick(int n_press, double x, double y)
{
    int selected_index = getSelectedSong(x, y);
    if (selected_index == -1)
        return;

    const Gdk::Rectangle rect(x, y, 1, 1);
    popup.set_pointing_to(rect);
    popup.popup();
}

void AlbumsTab::appendButtonClick()
{
    std::cout  << "\n\nsomeshithere" << std::endl;
}