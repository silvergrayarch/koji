// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#pragma once

#include "../player/player.h"
#include <gtkmm.h>

class Footer : public Gtk::Box
{
public:
  Footer();

  void update(const Player &player);

private:
  Gtk::Box status_row_{Gtk::Orientation::HORIZONTAL, 8};
  Gtk::Separator divider_;

  Gtk::Label status_label_;
  Gtk::Label time_label_;
  Gtk::Label volume_label_;
  Gtk::Label shuffle_label_;
  Gtk::Label repeat_label_;
  Gtk::Label hint_label_;
};
