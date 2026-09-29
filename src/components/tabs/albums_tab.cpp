// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray

#include "albums_tab.h"

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

    auto click_gesture = Gtk::GestureClick::create();
    click_gesture->signal_pressed().connect(sigc::mem_fun(*this, &AlbumsTab::onClicked));
    tree.add_controller(click_gesture);
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

void AlbumsTab::onClicked(int n_press, double x, double y)
{
    double offset_y = y - tree.get_column(0)->get_button()->get_allocation().get_height();

    Gtk::TreeModel::Path path;

    if (!tree.get_path_at_pos(static_cast<int>(x), static_cast<int>(offset_y), path))
        return;

    int selected_index = path[0];

    if (selected_index < 0 || selected_index >= player_->albums.size())
        return;

    player_->clearQueue();
    player_->addSongsToQueue(player_->albums[selected_index].songs);
}