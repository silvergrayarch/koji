// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#include "footer.h"
#include "../../library/entries.h"
#include "../utils/utils.h"

Footer::Footer() : Gtk::Box(Gtk::Orientation::VERTICAL, 4)
{
    append(status_row);

    status_row.append(status_label);

    Gtk::Box *spacer = Gtk::make_managed<Gtk::Box>();
    spacer->set_hexpand(true);
    status_row.append(*spacer);

    status_row.append(time_label);
    status_row.append(volume_label);
    status_row.append(shuffle_label);
    status_row.append(repeat_label);

    append(divider);

    hint_label.set_halign(Gtk::Align::START);
    hint_label.set_text("s: shuffle   r: repeat   space: play/pause   x: stop   q: quit");
    append(hint_label);
}

void Footer::update(const Player &player)
{
    std::string status_icon = player.current_song == -1 ? "⏹" : player.paused ? "⏸" : "⯈";
    std::string status_text = player.current_song == -1 ? "nothing playing" : player.queue.queue[player.current_song].title;
    status_label.set_text(status_icon + " " + status_text);

    std::string position_time = player.current_song != -1 ? formatTime(player.position) : "--:--";
    std::string duration_time = player.current_song != -1 ? formatTime(player.queue.queue[player.current_song].duration) : "--:--";
    time_label.set_text(position_time + "/" + duration_time);

    volume_label.set_text("Vol:" + std::to_string(player.volume) + "%");

    shuffle_label.set_text(player.shuffle ? "Shuf:On" : "Shuf:Off");

    repeat_label.set_text(player.repeat_mode == RepeatMode::Off ? "Rep:Off" : player.repeat_mode == RepeatMode::All ? "Rep:All" : "Rep:Trk");
}