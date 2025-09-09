#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class InputReceiver
{
  public:
    vector<Event> mEvents;

  private:
    void addEvent(Event event);
    vector<Event>& getEvents();
    void clearEvents();
}
