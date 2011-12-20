#include <play.hh>

int play(cc::game& game, const std::string& action)
{
  const char * s = action.c_str();
  const char ** action_s = &s;
  try {
    parse(game, action_s);
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << std::endl;
    return 0;
  }
  return 1;
}
