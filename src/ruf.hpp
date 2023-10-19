#pragma once

#include <filesystem>
#include <optional>
#include <regex>
#include <string>

namespace ruf {

// Ruf is the top-level main class that consolidates parameters and executes
class Ruf {
public:
  struct Parameters {
    std::string rootDirectory;
    std::string oldPattern;
    std::string newPattern;
    std::optional<std::string> excludePattern;
    bool assumeYes;
  };

private:
  const Parameters &m_params;

private:
  void checkParams();
  std::string constructPrompt(const std::filesystem::path &path,
                              const std::regex &oldRegex);

public:
  Ruf(const Parameters &params);
  void run();
};

} // namespace ruf
