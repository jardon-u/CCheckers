#include <iostream>
#include <algorithm>

#include <game.hh>
#include <cell.hh>

namespace cc
{

  extern const dir TL;
  extern const dir TR;
  extern const dir L;
  extern const dir R;
  extern const dir BL;
  extern const dir BR;

  game::game(board& b, int n)
    : round(1), nb_players(n), curr_player(1), board_(b)
  {
    players.resize(nb_players);
    std::vector<int> ind(nb_players,0);

    //for all sites
    for (unsigned i = 0; i < b.h; i++)
      for (unsigned j = 0; j < b.w; j++)
      {
        int v = b.board_init(i,j);

        if (v != board::INVALID_SITE &&  //
            v != board::FREE_SITE    &&  // if site must be filled up
            v < nb_players)              //
        {
          cell c(i,j);

          //create a marble of color v, identified by ind[v]
          marble m(v, ind[v]++);

          // assertion
          assert (m.color < nb_players && m.color >= 0);
          assert (m.id < b.nb_player_marbles && m.id >= 0 &&
                  "Invalid marble's identifier or board marble number");

          //fill player info
          players[m.color].marbles[m.id] = c;

          //fill board's sites with marbles
          b(c).m = m;
        }
      }

    // Update moves possibilities
    for (int p = 0; p < nb_players; p++)
      for (int m = 0; m < 10; m++)
        update_lines(players[p].marbles[m]);
  }


  int game::next_round()
  {
    std::cout << "Next round" << std::endl;
    curr_player = ++round % nb_players;

    if (curr_player == 0) //FIXME: is_computer(curr_player)
      round = ia_play(curr_player);

    return round;
  }


  sequence game::ia_find_best_sequence(cell& c, sequence s) // lol
  {
    // if c is already in the sequence
    if (std::find(s.list.begin(), s.list.end(),c) != s.list.end())
    {
      s.score = -1;
      return s;
    }

    std::list<cell>& l = s.list;
    for (std::list<cell>::iterator it = l.begin(); it != l.end(); it++)
      std::cout << *it;
    std::cout << std::endl;

    s.list.push_back(c);
    s.score = c.i;
    std::list<cell> dests = get_possible_jumps(c);
    std::cout << "Possible jumps :";
    std::list<cell>::iterator it;
    for (it = dests.begin(); it != dests.end(); it++)
      std::cout << *it;
    std::cout << std::endl;

    //for each possible dest d
    bool no_better_score = true;
    sequence best = s;
    std::list<cell>::iterator d;
    for (d = dests.begin(); d != dests.end(); d++)
    {
      sequence seq;
      seq = ia_find_best_sequence(*d,s);
      if (seq.score > best.score)
      {
        no_better_score = false;
        best = seq;
      }
    }

    //if no better score has been found return
    if (no_better_score) //FIXME: Check if last cell is allowed.
      best.score = c.i; //FIXME: Use stronger score eval!

    return best;
  }


  void game::ia_play_sequence(sequence s, int player)
  {
    std::list<cell>& l = s.list;

    if (l.size() <= 1)
    {
      std::cout << "Player" << curr_player << " not playing" << std::endl;
      return;
    }

    std::cout << "Player" << curr_player << " playing sequence ";

    cell to = l.front();
    l.pop_front();
    while (!l.empty())
    {
      cell from = to;
      to = l.front();
      l.pop_front();
      std::cout << from << " ";

      move(from,to,player);
    }

    std::cout << " " << to << std::endl;
  }


  int game::ia_play(int curr_player) // lol
  {
    player&  p = players[curr_player];

    std::cout << "Player" << curr_player << " playing." << std::endl;

    sequence best;
    for (int i = 0; i < 10; i++)
    {
      sequence tmp;
      sequence s = ia_find_best_sequence(p.marbles[i], tmp/*, param*/);
      if (s.score > best.score && s.list.size() > 1)
        best = s;
    }

    if (best.score != -1)
    {
      std::cout << "Player" << curr_player << " found a sequence with score "
                << best.score << ": ";
      std::list<cell>& l = best.list;
      for (std::list<cell>::iterator it = l.begin(); it != l.end(); it++)
        std::cout << *it;
      std::cout << std::endl;
    }

    try
    {
      ia_play_sequence(best, curr_player);
    }
    catch (std::exception& e)
    {
      std::cout << "Error: " << std::endl;
      std::cout << e.what() << std::endl;
      return 0;
    }

    return next_round();
  }


  //FIXME: remove this method.
  void game::set_pointer(cell c)
  {
    assert (board_.on_board(c));
    board_.set_color(c.i, c.j, -2);
  }


  void game::inform_player_about(cell c)
  {
    // Retrieve marble m at cell c.
    marble m = board_(c).m;

    assert (not board_.free(c));

    // Update position of marble 'm.id' in the correct player's table.
    players[m.color].marbles[m.id] = c;
  }


  void game::move(cell from, cell to, int player)
  {
    if (legal_move(from,to,player))
    {
      swap(from,to);
      inform_player_about(to);
      update_lines(to);
      update_lines(from);
    }
  }


  bool game::legal_move(cell from, cell to, int player)
  {
    // Test if player can play
    if (player != curr_player)
    {
      throw std::runtime_error("# Game: Player should not play");
      return false; 
      //FIXME: Is this return necessary?
      //FIXME: Not obvious since error management policy may vary.
      //FIXME: Seems paranoid but this kind of shit happens for real.
    }

    if (board_.free(from))
    {
      throw std::runtime_error("# Game: No marble here.");
      return false;
    }

    if (player != board_(from).m.color)
    {
      throw std::runtime_error("# Game: Player does not owe this marble.");
      return false;
    }

    if (not board_.free(to))
    {
      throw std::runtime_error("# Game: Destination not free");
      return false;
    }

    // Test if a jump leads to to /from/ /from/.
    std::list<jump>::iterator il;
    for (il = board_(from).jumps.begin(); il != board_(from).jumps.end(); il++)
      if ((*il).dest == to)
        return true;

    // Test if /to/ is adjacent to /from/.
    if (from + TL == to ||
        from + TR == to ||
        from + L  == to ||
        from + R  == to ||
        from + BR == to ||
        from + BL == to)
      return true;

    std::stringstream ss;
    ss << "# Game: Illegal move. ";
    ss << from << " -> " << to << std::endl;
    ss << "Possible destinations were .." << std::endl;
    for (il = board_(from).jumps.begin(); il != board_(from).jumps.end(); il++)
      ss << (*il).dest << std::endl;
    throw std::runtime_error(ss.str());
    return false;
  }


  void game::update_lines(cell c)
  {
    assert (board_.on_board(c) && "Cell out of board.");

    update_semiline(c,TL);
    update_semiline(c,BL);
    update_semiline(c,L);
    update_semiline(c,R);
    update_semiline(c,TR);
    update_semiline(c,BR);
  }


  void game::remove_jumps_in_dir(cell c, dir d)
  {
    std::list<jump>::iterator il;
    for (il = board_(c).jumps.begin(); il != board_(c).jumps.end(); il++)
      if (il->d == d || il->d == opposite(d))
        il = board_(c).jumps.erase(il);
  }


  void game::update_semiline(cell from, dir d)
  {
    cell p = from;
    while (board_.on_board(p))
    {
      remove_jumps_in_dir(p,d);
      remove_jumps_in_dir(p,opposite(d));
      update_jumps(p,d);
      update_jumps(p,opposite(d));
      p += d;
    }
  }


  void game::update_jumps(cell from, dir d)
  {
    if (not board_.on_board(from))
      return;

    int i = 1;
    cell p = from + d;

    while (board_.on_board(p) && board_.free(p))
    {
      p += d;
      i++;
    }
    if (not board_.on_board(p))
      return;

    cell to = p + d * i;
    if (free_between(p+d, to, d))
    {
      jump j(p, to, d);
      board_(from).jumps.push_back(j);
    }
  }


  std::list<cell> game::get_possible_jumps(cell c)
  {
    std::list<cell> r;

    std::list<jump>::iterator il;
    for (il = board_(c).jumps.begin(); il != board_(c).jumps.end(); il++)
      r.push_back((*il).dest);

    return r;
  }

  void game::swap(cell a, cell b)
  {
    site tmp  = board_(a);
    board_(a) = board_(b);
    board_(b) = tmp;
  }


  //FIXME: warning border include.
  //FIXME: Do not pass d.
  bool game::free_between(cell a, cell b, dir d)
  {
    while (board_.on_board(a) && board_.free(a) && a != b)
      a += d;
    if (not board_.on_board(a) || not board_.free(a))
      return false;
    return true;
  }


  int game::get_board_val(unsigned i, unsigned j)
  {
    return board_.get_color(i,j); //FIXME: deprecated get_color.
  }

}
