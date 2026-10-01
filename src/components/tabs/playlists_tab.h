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
  void cleanup();
  void setPlayer(Player *player_ptr) { player_ = player_ptr; }

  Gtk::Box box;
  Gtk::TreeView tree;
  TreeColumnSet collumns;
  Gtk::PopoverMenu popup;
  Gtk::ScrolledWindow window;
  Glib::RefPtr<Gtk::ListStore> tree_refrence;

private:
  void getSelectedPlaylist(double x, double y);

  void onLeftClick(int n_press, double x, double y);
  void onRightClick(int n_press, double x, double y);

  void appendButtonClick();

  int selected_playlist_;
  Player *player_;
};
