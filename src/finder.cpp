#include "finder.hpp"
#include <filesystem>
#include <iterator>
#include <optional>
#include <regex>

namespace ruf {

RecursiveFileFinder::RecursiveFileFinder(const std::string &root,
                                         std::regex &regex,
                                         std::optional<std::regex> exclude)
    : m_diriter(root), m_regex(regex), m_exclude(exclude) {

  // default constructed recursive_directory_iterator constructs an end iterator
  m_end = std::filesystem::recursive_directory_iterator{};
}

std::optional<std::filesystem::path> RecursiveFileFinder::next() {

  if (m_diriter == m_end) {
    return std::nullopt;
  }

  std::filesystem::path path = m_diriter->path();

  // in case this doesn't match the regex we're looking for, or this path
  // is not a regular file, or it matches an exclude pattern if the user
  // provided one, continue until we find a file that matches the pattern
  // we're looking for
  while (!std::regex_search(path.filename().string(), m_regex) ||
         !std::filesystem::is_regular_file(path) ||
         (m_exclude && std::regex_search(path.string(), *m_exclude))) {

    m_diriter++;
    if (m_diriter == m_end) {
      return std::nullopt;
    }

    path = m_diriter->path();
  }

  m_diriter++;
  return path;
}

} // namespace ruf
