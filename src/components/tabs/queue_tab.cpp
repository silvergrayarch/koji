// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray

#include "queue_tab.h"

QueueTab::QueueTab()
{
    window.set_child(tree);
    tree.set_enable_search(false);
    tree.set_rubber_banding(false);

    window.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    window.set_expand();

    box.append(window);

    std::vector<Glib::ustring> queue_column_headers = {"Title", "Album", "Artist", "Duration"};
    tree_refrence                                   = setupStringTreeView(tree, collumns, queue_column_headers);

    auto click_gesture = Gtk::GestureClick::create();
    click_gesture->signal_pressed().connect(sigc::mem_fun(*this, &QueueTab::onClicked));
    tree.add_controller(click_gesture);
}

void QueueTab::update()
{
    if (!(tree_refrence->children().size() == 0))
        tree_refrence->clear();
    for (SongEntry &song : player_->queue)
    {
        auto row                        = *(tree_refrence->append());
        row[collumns.string_columns[0]] = song.title;
        row[collumns.string_columns[1]] = song.album;
        row[collumns.string_columns[2]] = song.artist;
        row[collumns.string_columns[3]] = formatTime(song.duration);
    }

    if (player_->current_song < 0 || player_->current_song >= static_cast<int>(tree_refrence->children().size()))
        return;

    Gtk::TreeModel::Path path;
    path.push_back(player_->current_song);

    tree.get_selection()->select(path);
    tree.scroll_to_row(path);
}
void QueueTab::onClicked(int n_press, double x, double y)
{
    double offset_y = y - tree.get_column(0)->get_button()->get_allocation().get_height();

    Gtk::TreeModel::Path path;

    if (!tree.get_path_at_pos(static_cast<int>(x), static_cast<int>(offset_y), path))
        return;

    int selected_index = path[0];

    if (selected_index < 0 || selected_index >= player_->queue.size())
        return;

    player_->current_song = selected_index;
    player_->updateCurrentSong();
}