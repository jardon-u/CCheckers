#ifndef _UTIL_HH
# define _UTIL_HH

//FIXME: UTIL is an inappropriate name.

# include <list>
# include <player.hh>
# include <cell.hh>

namespace cc
{

  struct marble
  {
    marble()
      : color(9), id(-1) {}
    marble(int c, int i)
      : color(c), id(i) {}
    int color;
    int id;
  };

  struct jump
  {
    jump(cell p, cell de, dir di)
      : pivot(p), dest(de), d(di) {}
    cell pivot;
    cell dest;
    // hack: in order to never have to compute direction (eg. remove_jumps_in_dir).
    dir d;
  };

  struct site
  {
    marble m;
    std::list<jump> jumps;
  };

} // end of namespace cc

#endif /* _UTIL_HH */
