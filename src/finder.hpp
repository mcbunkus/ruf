#pragma once

#include <filesystem>
#include <optional>
#include <regex>
#include <string_view>

namespace ruf {

/**
 * RecursiveFileFinder is a "generator" that provides paths to regular files
 * (i.e. not directories, socket files, etc) that matches both a search pattern
 * and does not match an exclude pattern.
 *
 * Provide a directory from which to search from, a pattern to match filenames
 * on (it will only match on filenames, not any of the parent directories), and
 * optionally an exclude regex pattern, which matches on the filenames and
 * parent directories.
 *
 * Once you've constructed it, simply call next() when you want the next
 * matching file path. It will return nullopt once you have reached the end.
 */
class RecursiveFileFinder {
  std::filesystem::recursive_directory_iterator m_diriter;
  std::filesystem::recursive_directory_iterator m_end;

  std::regex &m_regex;
  std::optional<std::regex> m_exclude;

public:
  RecursiveFileFinder(const std::string &root, std::regex &matcher,
                      std::optional<std::regex> exclude);

  std::optional<std::filesystem::path> next();
};
} // namespace ruf
