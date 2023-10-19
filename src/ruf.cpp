
#include "ruf.hpp"
#include "finder.hpp"
#include "fmt/color.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "fmt/xchar.h"
#include "prompt.hpp"
#include <cstdlib>
#include <filesystem>
#include <optional>
#include <regex>

namespace ruf {

Ruf::Ruf(const Parameters &params) : m_params(params) {}

void Ruf::checkParams() {
  if (!std::filesystem::exists(m_params.rootDirectory)) {
    fmt::println(
        "'{}' directory doesn't exist, double check your paths/spelling",
        m_params.rootDirectory);
    std::exit(1);
  } else if (!std::filesystem::is_directory(m_params.rootDirectory)) {
    fmt::println("It doesn't look like '{}' is a directory, double check your "
                 "paths/spelling",
                 m_params.rootDirectory);
    std::exit(1);
  }
}

std::string Ruf::constructPrompt(const std::filesystem::path &oldPath,
                                 const std::regex &oldRegex) {

  std::string oldPromptReplacement =
      fmt::format(fmt::fg(fmt::color::gray) | fmt::emphasis::strikethrough,
                  "{}", m_params.oldPattern);

  std::string newPromptReplacement =
      fmt::format(fmt::fg(fmt::color::green), "{}", m_params.newPattern);

  std::string oldFilename = std::regex_replace(oldPath.filename().string(),
                                               oldRegex, oldPromptReplacement);

  std::string newFilename = std::regex_replace(oldPath.filename().string(),
                                               oldRegex, newPromptReplacement);

  std::string parentDir = oldPath.parent_path().string();
  return fmt::format("{}/{} -> {}/{}", parentDir, oldFilename, parentDir,
                     newFilename);
}

void Ruf::run() {

  checkParams();

  std::regex searchRegex{m_params.oldPattern};
  std::string replacement = m_params.newPattern;
  std::optional<std::regex> excludeRegex = std::nullopt;

  if (m_params.excludePattern) {
    excludeRegex = std::regex{*m_params.excludePattern};
  }

  ruf::RecursiveFileFinder finder{m_params.rootDirectory, searchRegex,
                                  excludeRegex};

  std::optional<std::filesystem::path> path;

  while ((path = finder.next())) {
    std::string filename = path->filename().string();

    std::string replaced =
        std::regex_replace(filename, searchRegex, replacement);

    std::filesystem::path newPath = path->parent_path() / replaced;

    std::string prompt = constructPrompt(*path, searchRegex);

    bool renameFile = m_params.assumeYes || ruf::PromptUser(prompt);
    if (renameFile) {
      try {
        std::filesystem::rename(*path, newPath);
        fmt::println("renamed {} to {}", path->string(), newPath.string());
      } catch (const std::filesystem::filesystem_error &e) {
        fmt::println("failed to rename {}: {}", filename, e.what());
      }
    }
  }
}

} // namespace ruf
