#include <board.hh>

namespace cc
{

  board::board()
  {
    for (unsigned i = 0; i < h; i++)
      for (unsigned j = 0; j < w; j++)
      {
        int v = board_init(i,j);

        //FIXME: In order to keep init_board readable as an array (which is
        //seriously not critical). Values used internally are those that are in
        //board_init_ but board_init_[i][j] - 1 (See game constructor). Same
        //thing here, empty sites are labeled -1 but are marked 0 in the init
        //board.
        if (v != INVALID_SITE)
          tab[i][j].m.color = -1; // Fake marble standing for empty site.
        else
          tab[i][j].m.color = INVALID_SITE;
        //Real marbles are placed later by the game constructor.
      };
  }

  site
  board::operator()(const cell& c) const
  {
    if (c.i >= h || c.j >= w)
      throw std::runtime_error("Board: cell out of bound.");
    if (not (c.i % 2 == c.j % 2))
      throw std::runtime_error("Board: invalid cell.");
    return tab[c.i][c.j];
  }

  site&
  board::operator()(const cell& c)
  {
    if (c.i >= h || c.j >= w)
      throw std::runtime_error("Board: cell out of bound.");
    if (not (c.i % 2 == c.j % 2))
      throw std::runtime_error("Board: invalid cell.");
    return tab[c.i][c.j];
  }

  int&
  board::operator[](const cell& c)
  {
    return tab[c.i][c.j].m.color;
  }

  //FIXME:replace operator[] or should be remove or ..
  // useful for UI (operator[] not supported by swift)
  int
  board::get_color(unsigned i, unsigned j) const
  {
    if (i >= h || j >= w)
      return INVALID_SITE;
    return tab[i][j].m.color;
  }

  void
  board::set_color(unsigned i, unsigned j, int color)
  {
    if (i >= h || j >= w)
      throw std::runtime_error("invalid cell");
    tab[i][j].m.color = color;
  }

  bool
  board::on_board(cell c)
  {
    return c.i < h && c.j < w && ((*this)[c] != INVALID_SITE);
  }

  bool
  board::free(cell c)
  {
    assert (on_board(c) && "Probably a bug here");
    return ((*this)[c] == -1 || (*this)[c] == -2);
  }

//   const unsigned board::board_init_[h][w] =
//     {{9,9,9,9,9,9,9,9,9,9,9,9,1,9,9,9,9,9,9,9,9,9,9,9,9},
//      {9,9,9,9,9,9,9,9,9,9,9,1,9,1,9,9,9,9,9,9,9,9,9,9,9},
//      {9,9,9,9,9,9,9,9,9,9,1,9,1,9,1,9,9,9,9,9,9,9,9,9,9},
//      {9,9,9,9,9,9,9,9,9,1,9,1,9,1,9,1,9,9,9,9,9,9,9,9,9},
//      {4,9,4,9,4,9,4,9,0,9,0,9,0,9,0,9,0,9,6,9,6,9,6,9,6},
//      {9,4,9,4,9,4,9,0,9,0,9,0,9,0,9,0,9,0,9,6,9,6,9,6,9},
//      {9,9,4,9,4,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,6,9,6,9,9},
//      {9,9,9,4,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,6,9,9,9},
//      {9,9,9,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,9,9,9},
//      {9,9,9,5,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,3,9,9,9},
//      {9,9,5,9,5,9,0,9,0,9,0,9,0,9,0,9,0,9,0,9,3,9,3,9,9},
//      {9,5,9,5,9,5,9,0,9,0,9,0,9,0,9,0,9,0,9,3,9,3,9,3,9},
//      {5,9,5,9,5,9,5,9,0,9,0,9,0,9,0,9,0,9,3,9,3,9,3,9,3},
//      {9,9,9,9,9,9,9,9,9,2,9,2,9,2,9,2,9,9,9,9,9,9,9,9,9},
//      {9,9,9,9,9,9,9,9,9,9,2,9,2,9,2,9,9,9,9,9,9,9,9,9,9},
//      {9,9,9,9,9,9,9,9,9,9,9,2,9,2,9,9,9,9,9,9,9,9,9,9,9},
//      {9,9,9,9,9,9,9,9,9,9,9,9,2,9,9,9,9,9,9,9,9,9,9,9,9}};

  int board::board_init(size_t i, size_t j) const
  {
    return board_init_[i][j] - 1;
  }

  const unsigned board::board_init_[h][w] =
    {{9,9,9,9,9,9,9,9,9,9,9,9,1,9,9,9,9,9,9,9,9,9,9,9,9},
     {9,9,9,9,9,9,9,9,9,9,9,1,9,0,9,9,9,9,9,9,9,9,9,9,9},
     {9,9,9,9,9,9,9,9,9,9,1,9,1,9,0,9,9,9,9,9,9,9,9,9,9},
     {9,9,9,9,9,9,9,9,9,1,9,1,9,1,9,1,9,9,9,9,9,9,9,9,9},
     {4,9,4,9,4,9,4,9,0,9,0,9,0,9,0,9,0,9,6,9,6,9,6,9,6},
     {9,4,9,4,9,4,9,0,9,0,9,0,9,0,9,0,9,0,9,6,9,6,9,6,9},
     {9,9,4,9,4,9,0,9,0,9,0,9,2,9,0,9,0,9,0,9,6,9,6,9,9},
     {9,9,9,4,9,0,9,0,9,0,9,0,9,0,9,1,9,0,9,0,9,6,9,9,9},
     {9,9,9,9,0,9,0,9,0,9,2,9,0,9,1,9,0,9,0,9,0,9,9,9,9},
     {9,9,9,5,9,0,9,2,9,0,9,0,9,0,9,0,9,0,9,0,9,3,9,9,9},
     {9,9,5,9,5,9,0,9,0,9,0,9,2,9,0,9,2,9,0,9,3,9,3,9,9},
     {9,5,9,5,9,5,9,0,9,0,9,0,9,0,9,0,9,0,9,3,9,3,9,3,9},
     {5,9,5,9,5,9,5,9,0,9,0,9,0,9,0,9,0,9,3,9,3,9,3,9,3},
     {9,9,9,9,9,9,9,9,9,2,9,2,9,2,9,0,9,9,9,9,9,9,9,9,9},
     {9,9,9,9,9,9,9,9,9,9,2,9,0,9,0,9,9,9,9,9,9,9,9,9,9},
     {9,9,9,9,9,9,9,9,9,9,9,2,9,0,9,9,9,9,9,9,9,9,9,9,9},
     {9,9,9,9,9,9,9,9,9,9,9,9,0,9,9,9,9,9,9,9,9,9,9,9,9}};


} // end of namespace cc

