// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#pragma once

#include "../../player/player.h"
#include "../utils.h"
#include <gtkmm.h>
#include <vector>

class PlaylistsTab
{
public:
  PlaylistsTab();
  void update();
  void setPlayer(Player *player_ptr) { player_ = player_ptr; }

  TreeColumnSet collumns;
  Gtk::Box box;
  Gtk::ScrolledWindow window;
  Gtk::TreeView tree;
  Glib::RefPtr<Gtk::ListStore> tree_refrence;

private:
  void onClicked(int n_press, double x, double y);
  Player *player_;
};
