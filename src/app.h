#ifndef app_h
#define app_h

#include <filesystem>

enum class Mode { REPL, EXEC, UNDEFINED };

class App {
  public:
    void run(int argc, const char* argv[]);

  private:
    void repl();
    void run_file(std::filesystem::path);

    Mode check_mode(int);
};

#endif
