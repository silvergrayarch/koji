// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#pragma once

#include <filesystem>
#include <string>
#include <vector>

// filesystem::path path;
// string artist;
// string album;
// string title;
struct SongEntry
{
  std::filesystem::path path;
  std::string artist;
  std::string album;
  std::string title;
  float duration;
  bool operator==(const SongEntry &) const = default;
};

// filesystem::path path;
// string title;
// string artist;
// vector<SongEntry> songs;
struct AlbumEntry
{
  std::filesystem::path path;
  std::string title;
  std::string artist;
  std::vector<SongEntry> songs;
  bool operator==(const AlbumEntry &) const = default;
};

// filesystem::path path;
// string title;
// vector<SongEntry> songs;
struct PlaylistEntry
{
  std::filesystem::path path;
  std::string title;
  std::vector<SongEntry> songs;
  bool operator==(const PlaylistEntry &) const = default;
};
