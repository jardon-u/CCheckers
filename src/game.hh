#ifndef _GAME_HH
# define _GAME_HH

# include <sstream>
# include <list>
# include <vector>
# include <stdexcept>
# include <board.hh>
# include <util.hh>


namespace cc
{
  // This struct was originally nested into struct game
  // but this not yet supported by SWIG
  struct sequence
  {
    sequence() : score(-1) { }
    sequence(cell c) : list(1,c) { }

    sequence concat(sequence s)
    {
      list.insert(list.end(), s.list.begin(), s.list.end());
      score = s.score;
      return *this;
    }

    float score;
    std::list<cell> list;
  };

  //FIXME: Allow nb_player to be changed (done or not ?)
  struct game
  {
    // cons
    //-----------------------------
    game(board& board, int nb_player);


    // debug
    //-----------------------------

    void  set_pointer(cell c); //FIXME: to remove.
    bool  legal_move(cell c, cell d, int player);

    // base method
    //-----------------------------

    void  move(cell c, cell d, int player);
    void  swap(cell a, cell b);
    bool  free_between(cell a, cell b, dir d);
    int   next_round();

    // ia
    //-----------------------------

    int            ia_play(int curr_player);
    sequence ia_find_best_sequence(cell& c, sequence s);
    void           ia_play_sequence(sequence s, int player);

    // methods related to computation of possible moves
    //----------------------------------------------------------

    /// Get possible jumps from one cell
    std::list<cell>  get_possible_jumps(cell c);

    /// Call /update_semiline(c,d)/ for any direction d.
    void  update_lines(cell c);

    /// Update possible moves of cells on the semiline c/d.
    void  update_semiline(cell c, dir d);

    /// Update possible moves (jumps) from c in the direction d.
    void  update_jumps(cell c, dir d);

    /// Remove any jumps directed in d.
    void  remove_jumps_in_dir(cell c, dir d);

    /// Update players information concerning the marble at cell c.
    void  inform_player_about(cell c);

    // Abstract board access
    int get_board_val(unsigned i, unsigned j);

    unsigned round;
    const int nb_players;
    int curr_player;
    std::vector<player> players;
    board& board_;
  };

}
#endif /* _GAME_HH */

