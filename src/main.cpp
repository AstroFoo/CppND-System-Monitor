#include "ncurses_display.h"
#include "system.h"
#include "platform.h"

using namespace Platform;

int main() {
  // TODO: Implement platform identification, either using command line arguments or an install configuration
  // For now, just hardcode the platform here.  Only linux is supported.
  Platform::Platform platform = Platform::Platform::kLinux;
  System system(platform);
  NCursesDisplay::Display(system);
}