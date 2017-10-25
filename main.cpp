#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <optional>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <utility>
#include <ncurses.h>

using std::cout;
using std::endl;
using std::string;
using std::optional;
using std::vector;
using std::stringstream;
using std::unordered_map;
using std::to_string;
using std::pair;
using std::make_pair;

vector<string> split(const string& s) {
  string buf;
  stringstream ss{s};
  vector<string> result;
  while (ss >> buf) result.push_back(buf);
  return result;
}

class UI {
  WINDOW* _user_win;
  vector<string> _messages;
  vector<string> _prev_input;
  string _curr_input;
  
public:
  UI(): _user_win(nullptr), messages() {
     setlocale(LC_ALL,"");
     initscr();             // initialize ncurses
     noecho();              // don't actually write the characters received from input
     keypad(stdscr, TRUE);  // special characters like F1
     raw();                 // print things immediately
  }
  
  ~UI() { endwin(); }
  
  void message(const string& msg) {
    messages.push_back(msg);
  }

  void display() {
    
    
  }
};

class Otto {
  vector<pair<string, long>> _stack;
  UI _ui;

  void _push_number(long n) {
    _stack.push_back(make_pair("long", n));
  }

  long _pop_number(void) {
    long result = _stack.back().second;
    _stack.pop_back();
    return result;
  }
  
  unordered_map<string, std::function<void()>> builtins = {
    {"POP", [this](){
        if (_stack.size()) {
          _ui.message("popped: " + to_string(_pop_number()) + "\n");
        } else {
          _ui.message("stack is empty!\n");
        }}},
    {"STACK", [this](){
        if (_stack.size()) {
          for (const auto& el : _stack) {
            _ui.message("  " + to_string(el.second) + "\n");
          }
        } else {
          _ui.message("stack is empty!\n");
        }}},
    {"+", [this](){
        if (_stack.size() < 2) {
          _ui.message("not enough numbers!\n");
        } else {
          auto b = _pop_number();
          auto a = _pop_number();
          _push_number(a + b);
        }}},
    {"*", [this](){
        if (_stack.size() < 2) {
          _ui.message("not enough numbers!\n");
        } else {
          auto b = _pop_number();
          auto a = _pop_number();
          _push_number(a * b);
        }}},
    {"/", [this](){
        if (_stack.size() < 2) {
          _ui.message("not enough numbers!\n");
        } else {
          auto b = _pop_number();
          auto a = _pop_number();
          _push_number(a / b);
        }}},
    {"-", [this](){
        if (_stack.size() < 2) {
          _ui.message("not enough numbers!\n");
        } else {
          auto b = _pop_number();
          auto a = _pop_number();
          _push_number(a - b);
        }}}
  };
  
public:
  Otto(): _stack(), _ui() {}
  
  optional<long> number(const string& word) {
    try {
      long value = std::stol(word.c_str());
      return optional(value);
      //TODO cache this to use later?
    } catch (...) { /* invalid_argument, out_of_range */
      return optional<long>();
    }
  }

  void read_line(const string& line) {
    for (auto word : split(line)) {
      read_word(word);
    }
  }
  
  void read_word(const string& word) {
    if (auto res = number(word); res) {
      _push_number(*res);
    } else {
      if (builtins.find(word) != builtins.end()) {
        builtins[word]();
      }
    }
  }

  void handle_input(int c) {
    
  }

  void display() {
    
  }

  void loop() {
    while (true) {
      c = getch();
      if (c == 113/* q */) break;
      handle_input(c);
      _ui.display();
    }
  }
};

int main(int argc, char** argv) {
  Otto otto;
  if (argc) {
    for (string line; getline(std::cin, line);) {
      otto.read_line(line);
    }
  } else {
    printw("hello world\n");
    otto.loop();
    getch();
  }
}
