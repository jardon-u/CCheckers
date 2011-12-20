#include <iostream>

#include <board.hh>
#include <game.hh>
#include <play.hh>

//FIXME: This part should be a module.
void print_cell(int c)
{
  //std::cout << c << std::endl;
  switch (c)
  {
  case -2: std::cout << "X "; break; // pointer

  case -1: std::cout << ". "; break; // empty site
  case  0: std::cout << "\033[31m\033[1m0 \033[m"; break;
  case  1: std::cout << "\033[32m\033[1m1 \033[m"; break;
  case  2: std::cout << "\033[33m\033[1m2 \033[m"; break;
  case  3: std::cout << "\033[34m\033[1m3 \033[m"; break;
  case  4: std::cout << "\033[35m\033[1m4 \033[m"; break;
  case  5: std::cout << "\033[36m\033[1m5 \033[m"; break;

  default:
  case 8: std::cout <<"  "; break; // no site
  };
}

void print(const cc::board& b)
{
  using namespace std;
  cout << "\t00:                 10:                 20:      " << endl;
  cout << "\t0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4" << endl;

  for (unsigned i = 0; i < b.h; i++)
  {
    std::cout <<  i << "\t";
    for (unsigned j = 0; j < b.w; j++)
      print_cell(b.get_color(i,j));
    std::cout << std::endl;
  }
}

bool usage(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cout << "Chinese Checker usage:" << std::endl;
    std::cout << argv[0] << " nb_players" << std::endl;
    return true;
  }

  // check parameters
  unsigned nb_players = atoi(argv[1]);
  if (nb_players == 0 || nb_players > 6)
  {
    std::cout << "Chinese Checker error:" << std::endl;
    std::cout << "nb_players should be between 1 and 6." << std::endl;
    return 1;
  }

  return false;
}

int main(int argc, char ** argv)
{
  if (usage(argc,argv))
    return 1;

  using namespace cc;
  unsigned nb_players = atoi(argv[1]);
  board cc;
  game g(cc,nb_players);

  //cc.set_pointer(cell(8,12));
  //g.move(cell(12,22), cell(11,17), 3);
  //g.mark_possible_move(cell(13,9));

  //std::cout << g.players.size() << std::endl;

  for (int p = 0; p < g.nb_players; p++)
  {
    std::cout << p << std::endl;
    for (int m = 0; m < 10; m++)
      std::cout << g.players[p].marbles[m] << std::endl;
  }

  print(cc);

  char cmdline[256];
  std::cout << "ccheckers $ ";
  while (std::cin.getline(cmdline, 256))
  {
    try {
      play(g, cmdline);
    } catch (std::exception& e) {
      std::cout << "/!\\ \033[31m\033[1m An Error Occurred:\033[m "
                << e.what() << std::endl << std::endl;
    } catch (...)  {
      std::cout << "Unexpected error." << std::endl;
    }

//     std::vector< std::list<cell> > moves(10);
//     for (int m = 0; m < 10; m++)
//       moves[m] = g.get_possible_jumps(g.players[1].marbles[m]);
    print(cc);
    std::cout << "ccheckers $ ";
  }
}
