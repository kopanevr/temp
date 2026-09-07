/**
 * @file
 * @brief
 */

//

#include <cstdlib>

//

#include "Application.hpp"

//

int main(int argc, char *argv[]) {
  auto *const app = Application::getInstance();
  if (!app->init(argc, argv))
    return EXIT_FAILURE;

  return app->exec();
}
