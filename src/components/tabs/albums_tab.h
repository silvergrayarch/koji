// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#pragma once

#include <vector>
#include <gtkmm.h>
#include "../utils.h"
#include "../../player/player.h"

class AlbumsTab
{
  public:
    AlbumsTab();
    void update();
    void setPlayer(Player *player_ptr) { player_ = player_ptr; }
    
    TreeColumnSet                collumns;
    Gtk::Box                     box;
    Gtk::ScrolledWindow          window;
    Gtk::TreeView                tree;
    Glib::RefPtr<Gtk::ListStore> tree_refrence;

  private:
    void    onClicked(int n_press, double x, double y);
    Player *player_;
};