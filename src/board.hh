#ifndef CC_BOARD_HH
# define CC_BOARD_HH

# include <stdexcept>
# include <cassert>
# include <cstring>
# include <util.hh>
# include <player.hh>

namespace cc
{

  // Standard Chinese Checker Board
  struct board
  {
    // cons
    //-----------------------------
    board();

    // interface
    //-----------------------------

    /// Provide const access to site
    site  operator()(const cell& c) const;

    /// Provide access to site
    site&  operator()(const cell& c);

    /// Give access to the color of a cell (0 or 9) if no marbles
    int& operator[](const cell& c);

    /// Set the color of a cell
    void set_color(unsigned i, unsigned j, int color);

    /// Return the color of a cell (0 or 9) if no marbles
    int get_color(unsigned i, unsigned j) const;

    // method
    //-----------------------------

    bool  on_board(cell c);
    bool  free(cell c);

    static const size_t h = 17;
    static const size_t w = 25;
    static const int nb_player_marbles = 10;

    //FIXME: confusion between site / color / cell
    static const int INVALID_SITE = 8;
    static const int FREE_SITE = -1;

    /// Current State and marbles positions.
    /// with  site = { marble m, list<jump> }
    site tab[h][w];

    int board_init(size_t i, size_t j) const;

  private:
    /// Initialization board. Only tab MUST be modified.
    static const unsigned board_init_[h][w];
  };

} // end of namespace cc

#endif /* CC_BOARD_HH */

