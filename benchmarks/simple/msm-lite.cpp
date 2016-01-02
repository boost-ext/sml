
#include <iostream>
#include <typeinfo>
#ifdef WIN32
#include "windows.h"
#else
#include <sys/time.h>
#endif
#include "msm/msm.hpp"

struct play {};
struct end_pause {};
struct stop {};
struct pause {};
struct open_close {};
struct cd_detected {};

auto start_playback = [] {};
auto resume_playback = [] {};
auto close_drawer = [] {};
auto open_drawer = [] {};
auto stop_and_open = [] {};
auto stopped_again = [] {};
auto store_cd_info = [] {};
auto pause_playback = [] {};
auto stop_playback = [] {};

struct player {
  auto configure() const noexcept {
    using namespace msm;

    state Empty, Open, Stopped, Playing, Pause;
    /*    state<class empty> Empty;*/
    // state<class open> Open;
    // state<class stopped> Stopped;
    // state<class playing> Playing;
    /*state<class pause> Pause;*/

    return make_transition_table(
        Stopped == Playing + event<play> / start_playback,
        Pause == Playing + event<end_pause> / resume_playback,
        //  +------------------------------------------------------------------------------+
        Open == Empty + event<open_close> / close_drawer,
        //  +------------------------------------------------------------------------------+
        Empty(initial) == Open + event<open_close> / open_drawer,
        Pause == Open + event<open_close> / stop_and_open,
        Stopped == Open + event<open_close> / open_drawer,
        Playing == Open + event<open_close> / stop_and_open,
        //  +------------------------------------------------------------------------------+
        Playing == Pause + event<pause> / pause_playback,
        //  +------------------------------------------------------------------------------+
        Playing == Stopped + event<stop> / stop_playback,
        Pause == Stopped + event<stop> / stop_playback,
        Empty == Stopped + event<cd_detected> / store_cd_info,
        Stopped == Stopped + event<stop> / stopped_again);
  }

  // auto configure() const noexcept {
  // using namespace msm;
  // return make_transition_table(
  //"Stopped"_s == "Playing"_s + event<play> / start_playback,
  //"Pause"_s == "Playing"_s + event<end_pause> / resume_playback,
  ////
  ///+------------------------------------------------------------------------------+
  //"Open"_s == "Empty"_s + event<open_close> / close_drawer,
  ////
  ///+------------------------------------------------------------------------------+
  //"Empty"_s(init) == "Open"_s + event<open_close> / open_drawer,
  //"Pause"_s == "Open"_s + event<open_close> / stop_and_open,
  //"Stopped"_s == "Open"_s + event<open_close> / open_drawer,
  //"Playing"_s == "Open"_s + event<open_close> / stop_and_open,
  ////
  ///+------------------------------------------------------------------------------+
  //"Playing"_s == "Pause"_s + event<pause> / pause_playback,
  ////
  ///+------------------------------------------------------------------------------+
  //"Playing"_s == "Stopped"_s + event<stop> / stop_playback,
  //"Pause"_s == "Stopped"_s + event<stop> / stop_playback,
  //"Empty"_s == "Stopped"_s + event<cd_detected> / store_cd_info,
  //"Stopped"_s == "Stopped"_s + event<stop> / stopped_again);
  //}
};

#ifndef WIN32
long mtime(struct timeval &tv1, struct timeval &tv2) {
  return (tv2.tv_sec - tv1.tv_sec) * 1000000 + ((tv2.tv_usec - tv1.tv_usec));
}
#endif

int main() {
// for timing
#ifdef WIN32
  LARGE_INTEGER res;
  ::QueryPerformanceFrequency(&res);
  LARGE_INTEGER li, li2;
#else
  struct timeval tv1, tv2;
  gettimeofday(&tv1, NULL);
#endif

  player p;
  msm::sm<player> p2{p};
// for timing
#ifdef WIN32
  ::QueryPerformanceCounter(&li);
#else
  gettimeofday(&tv1, NULL);
#endif
  std::cout << sizeof(p.configure()) << std::endl;
  std::cout << sizeof(p2) << std::endl;
  for (int i = 0; i < 1'000'000; ++i) {
    // for (int i = 0; i < 1; ++i) {
    // Open
    // Empty
    // Stopped
    // Playing
    // pause_d
    // Playing
    // pause_
    // Stopped
    // Stopped
    // Open
    // Empty
    p2.process_event(open_close{});
    p2.process_event(open_close{});
    p2.process_event(cd_detected{});
    p2.process_event(play{});
    p2.process_event(pause{});
    // go back to Playing
    p2.process_event(end_pause{});
    p2.process_event(pause{});
    p2.process_event(stop{});
    // event leading to the same state
    p2.process_event(stop{});
    p2.process_event(open_close{});
    p2.process_event(open_close{});

    // p2.process_event(open_close{});
  }
#ifdef WIN32
  ::QueryPerformanceCounter(&li2);
#else
  gettimeofday(&tv2, NULL);
#endif
#ifdef WIN32
  std::cout << "msm took in s:"
            << (double)(li2.QuadPart - li.QuadPart) / res.QuadPart << "\n"
            << std::endl;
#else
  std::cout << "msm took in us:" << mtime(tv1, tv2) << "\n" << std::endl;
#endif
  return 0;
}
