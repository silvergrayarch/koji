// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#pragma once

#include <string>
#include <vector>
#include "entries.h"

std::vector<AlbumEntry>    getAlbums();
std::vector<SongEntry>     getAlbumSongs(const AlbumEntry &album);
std::vector<PlaylistEntry> getPlaylists();
std::vector<SongEntry>     getPlaylistSongs(const PlaylistEntry &playlist);
bool                       renamePlaylist(const PlaylistEntry &entry, const std::string &name);
void                       savePlaylist(const PlaylistEntry &entry, const std::vector<SongEntry> &playlist);
bool                       duplicatePlaylist(const PlaylistEntry &entry);
