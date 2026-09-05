/**
 * @file
 * @brief
 */

//

#include <cstdlib>

//

#include "Application.hpp"

int main(int argc, char* argv[])
{
    Application* const app = Application::getInstance();
    if (!app->init(argc, argv)) return EXIT_FAILURE;

    return app->exec();
}
