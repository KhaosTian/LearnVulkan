#include "GlobalContext.h"
#include "WindowSystem.h"
import std;

int main() {

    Nova::kGlobalContext.Startup();

    while (!Nova::kGlobalContext.GetWindowSystem()->ShouldClose()) {
        Nova::kGlobalContext.GetWindowSystem()->PollEvent();
    }

    Nova::kGlobalContext.Shutdown();
    return 0;
}
