#ifndef _PLAY_HH
# define _PLAY_HH

# include <game.hh>
# include <parse_util.hh>
# include <cell.hh>

template <typename G>
void parse_move(G& game, const char ** action)
{
  // p1
  int i1 = eat_number(action);
  int j1 = eat_number(action);

  // p2
  int i2 = eat_number(action);
  int j2 = eat_number(action);

  int player = eat_number(action);

  game.move(cc::cell(i1,j1), cc::cell(i2,j2), player);
}

template <typename G>
void parse(G& game, const char ** action)
{
  if (successful_eat(action, "d"))
  {
    std::cout << "Round: " << game.next_round() << std::endl;
    return;
  }

  if (successful_eat(action, "p"))
  {
    std::cout << "Current player: " << game.curr_player << std::endl;
    return;
  }

  if (successful_eat(action, "move") || successful_eat(action, "m"))
    return parse_move(game, action);

  if (successful_eat(action, "help") || successful_eat(action, "h"))
  {
    std::cout << "help:                          " << std::endl;
    std::cout << "     m/move i1 j1 i2 j2 player " << std::endl;
    std::cout << "     d      - round            " << std::endl;
    std::cout << "     p      - current player   " << std::endl;
    std::cout << "     q/quit - quit             " << std::endl;
    std::cout << "     h/help - this help        " << std::endl;
    return;
  }

  if (successful_eat(action, "q"))
  {
    std::cout << "Quitting game." << std::endl;
    exit(0);
  }
  throw std::runtime_error("Parser: cannot parse input line.");
}


int play(cc::game& game, const std::string& action);


#endif /* _PLAY_HH */
