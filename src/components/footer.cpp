// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#include "footer.h"
#include "../library/entries.h"
#include "utils.h"

Footer::Footer() : Gtk::Box(Gtk::Orientation::VERTICAL, 4)
{
  append(status_row_);

  status_row_.append(status_label_);

  Gtk::Box *spacer = Gtk::make_managed<Gtk::Box>();
  spacer->set_hexpand(true);
  status_row_.append(*spacer);

  status_row_.append(time_label_);
  status_row_.append(volume_label_);
  status_row_.append(shuffle_label_);
  status_row_.append(repeat_label_);

  append(divider_);

  hint_label_.set_halign(Gtk::Align::START);
  hint_label_.set_text("s: shuffle   r: repeat   space: play/pause   x: stop   esc: quit");
  append(hint_label_);
}

void Footer::update(const Player &player)
{
  std::string status_icon = player.current_song == -1 ? "⏹" : player.paused ? "⏸" : "⯈";
  std::string status_text = player.current_song == -1 ? "nothing playing" : player.queue[player.current_song].title;
  status_label_.set_text(status_icon + " " + status_text);

  std::string position_time = player.current_song != -1 ? formatTime(player.position) : "--:--";
  std::string duration_time = player.current_song != -1 ? formatTime(player.queue[player.current_song].duration) : "--:--";
  time_label_.set_text(position_time + "/" + duration_time);

  volume_label_.set_text("Vol:" + std::to_string(player.volume) + "%");

  shuffle_label_.set_text(player.shuffle ? "Shuf:On" : "Shuf:Off");

  repeat_label_.set_text(player.repeat_mode == RepeatMode::Off ? "Rep:Off" : player.repeat_mode == RepeatMode::All ? "Rep:All" : "Rep:Trk");
}
