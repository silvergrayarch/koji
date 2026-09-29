// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#pragma once

#include "entries.h"
#include <string>
#include <vector>

std::vector<AlbumEntry> get_albums();
std::vector<PlaylistEntry> get_playlists();
bool rename_playlist(const PlaylistEntry &playlist, const std::string &name);
void save_playlist(const PlaylistEntry &playlist);
bool duplicate_playlist(const PlaylistEntry &playlist);
