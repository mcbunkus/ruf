#include "prompt.hpp"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include <iostream>
#include <string>

namespace ruf {

static const std::unordered_map<std::string, bool> m_responseMap{
    {"", true},
    {"y", true},
    {"n", false},
};

bool PromptUser(const std::string &prompt) {

  fmt::print("{} [Yn] ", prompt);

  std::string answer;
  std::getline(std::cin, answer);

  // strip all the whitespace from the answer
  answer.erase(std::remove_if(answer.begin(), answer.end(), ::isspace),
               answer.end());

  if (answer.size() > 1 || m_responseMap.find(answer) == m_responseMap.end()) {
    fmt::println("'{}' is not a valid answer", answer);
    return PromptUser(prompt);
  }

  return m_responseMap.at(answer);
}

} // namespace ruf
