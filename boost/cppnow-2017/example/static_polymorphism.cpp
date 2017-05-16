#include <cassert>
#include <fstream>
#include <iostream>

template <class T>
struct Reader {
  int read() { return static_cast<T*>(this)->read_impl(); }
  int read_impl() { return 0; }
};

struct FileReader : Reader<FileReader> {
  explicit FileReader(const std::string& str) : file(str) { assert(file.good()); }

  int read_impl() {
    auto value = 0;
    file >> value;
    return value;
  }

  std::fstream file;
};

template <class T>
struct Viewer {
  void show(int value) { static_cast<T*>(this)->show_impl(value); }
  void show_impl(int) {}
};

struct ConsoleViewer : Viewer<ConsoleViewer> {
  void show_impl(int value) { std::cout << value << std::endl; }
};

template <class TReader, class TViewer>
struct App {
  TReader& reader;
  TViewer& viewer;

  App(TReader& reader, TViewer& viewer) : reader(reader), viewer(viewer) {}

  void run() { viewer.show(reader.read()); }
};

int main() {
  FileReader reader{"input.txt"};
  ConsoleViewer viewer{};
  App<FileReader, ConsoleViewer>{reader, viewer}.run();
  // App{reader, viewer}.run();
}
