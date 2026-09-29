// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray

#include "utils.h"

std::string formatTime(const float seconds)
{
    if (seconds < 0.0f)
        return "--:--";

    int total   = static_cast<int>(seconds);
    int hours   = total / 3600;
    int minutes = (total % 3600) / 60;
    int secs    = total % 60;

    if (hours > 0)
        return std::format("{:02}:{:02}:{:02}", hours, minutes, secs);

    return std::format("{:02}:{:02}", minutes, secs);
}

Gtk::TreeModelColumn<Glib::ustring> &TreeColumnSet::addStringColumn()
{
    string_columns.emplace_back();
    Gtk::TreeModelColumn<Glib::ustring> &new_column = string_columns.back();
    add(new_column);
    return new_column;
}

Glib::RefPtr<Gtk::ListStore> setupStringTreeView(Gtk::TreeView &tree_view, TreeColumnSet &column_set, const std::vector<Glib::ustring> &column_headers)
{
    for (unsigned int header_index = 0; header_index < column_headers.size(); ++header_index)
        column_set.addStringColumn();

    Glib::RefPtr<Gtk::ListStore> list_store_reference = Gtk::ListStore::create(column_set);
    tree_view.set_model(list_store_reference);

    for (unsigned int header_index = 0; header_index < column_headers.size(); ++header_index)
        tree_view.append_column(column_headers[header_index], column_set.string_columns[header_index]);

    for (Gtk::TreeViewColumn *column : tree_view.get_columns())
        column->set_expand(true);

    return list_store_reference;
}