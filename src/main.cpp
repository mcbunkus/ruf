#include "cxxopts.hpp"
#include "fmt/core.h"
#include "fmt/xchar.h"
#include "ruf.hpp"
#include <exception>
#include <iostream>
#include <optional>

int main(int argc, char *argv[]) {

  const std::string description =
      "\nruf (recursively updates filenames) is an interactive utility for\n"
      "renaming files. It requires 2 positional arguments: a regex pattern\n"
      "to match filenames on, and a string pattern to replace the matched\n"
      "text with. Run ruf -h or ruf --help for more info about options\n";

  cxxopts::Options options{"ruf", description};

  // clang-format off
  options.add_options()
    ("old", "pattern to match filenames on", cxxopts::value<std::string>())
    ("new", "pattern to replace in matched filenames", cxxopts::value<std::string>())
    ("e,exclude", "pattern to exclude filenames from the search", cxxopts::value<std::string>())
    ("r,root", "root directory to start search from", cxxopts::value<std::string>()->default_value("."))
    ("y,yes", "assume yes for all prompts", cxxopts::value<bool>()->default_value("false"))
    ("h,help", "display this help message", cxxopts::value<bool>()->default_value("false"));
  // clang-format on

  options.positional_help("<old pattern> <new pattern>");
  options.parse_positional({"old", "new"});
  auto args = options.parse(argc, argv);

  if (args.count("help")) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  ruf::Ruf::Parameters params;

  if (!args.count("old")) {
    fmt::println("I require a pattern to match filenames on");
    std::cerr << options.help() << std::endl;
    return 1;
  }

  params.rootDirectory = args["root"].as<std::string>();
  params.oldPattern = args["old"].as<std::string>();

  if (!args.count("new")) {
    fmt::println("I require a pattern to replace in matched filenames");
    std::cerr << options.help() << std::endl;
    return 1;
  }

  params.newPattern = args["new"].as<std::string>();

  params.assumeYes = args["yes"].as<bool>();

  if (!args.count("exclude")) {
    params.excludePattern = std::nullopt;
  } else {
    params.excludePattern = args["exclude"].as<std::string>();
  }

  ruf::Ruf ruf{params};

  try {
    ruf.run();
  } catch (const std::exception &e) {
    fmt::println("oops, I had an unrecoverable error: {}", e.what());
  }

  return 0;
}
