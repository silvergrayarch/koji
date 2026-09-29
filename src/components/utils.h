// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#pragma once

#include <format>
#include <gtkmm.h>
#include <vector>

std::string formatTime(const float seconds);

class TreeColumnSet : public Gtk::TreeModel::ColumnRecord
{
public:
  std::deque<Gtk::TreeModelColumn<Glib::ustring>> string_columns;
  Gtk::TreeModelColumn<Glib::ustring> &addStringColumn();
};

Glib::RefPtr<Gtk::ListStore> setupStringTreeView(Gtk::TreeView &tree_view, TreeColumnSet &column_set, const std::vector<Glib::ustring> &column_headers);
