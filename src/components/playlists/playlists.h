// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#pragma once

#include <vector>
#include <gtkmm.h>
#include "../../library/entries.h"
#include "../../library/library.h"
#include "../queue/queue.h"
#include "../utils/utils.h"

class Player;

class Playlists
{
  public:
    Playlists();
    void update();
    void setPlayer(Player *player_ptr) { player_ = player_ptr; }

    std::vector<PlaylistEntry>   playlists = koji::library::getPlaylists();
    TreeColumnSet                collumns;
    Gtk::Box                     box;
    Gtk::ScrolledWindow          window;
    Gtk::TreeView                tree;
    Glib::RefPtr<Gtk::ListStore> tree_refrence;

  private:
    Player *player_;
    void    onClicked(int n_press, double x, double y);
};