#ifndef _CELL_HH
# define _CELL_HH

# include <iostream>

namespace cc
{

  struct dir
  {
    dir(int i, int j) : i_(i), j_(j) {}

    dir operator+(dir d) const
    {
      return dir(i_ + d.i_, j_ + d.j_);
    }

    dir operator*(int i) const
    {
      return dir(i_ * i, j_ * i);
    }

    bool operator==(dir d) const
    {
      return (i_ == d.i_) && (j_ == d.j_);
    }

    int i_;
    int j_;
  };

  // Return opposite direction (exe: opposite(L) returns R.
  inline
  dir opposite(dir d)
  {
    return d * -1;
  }

  //FIXME: *minor issue* As lightweight (2 unsigned) objects, cells are
  //FIXME: generally passed by copy to functions (see game.hh). Should we pass them by
  //FIXME: const ref ? Anyway an aesthetic issue only at this point.

  struct cell
  {
    cell() : i(0), j(0) {}
    cell(unsigned i, unsigned j) : i(i), j(j) {}

    cell operator+(dir d)
    {
      return cell(i + d.i_, j + d.j_);
    }

    cell operator+=(dir d)
    {
      *this = *this + d;
      return (*this);
    }

    bool operator!=(cell c)
    {
      return (i != c.i || j != c.j);
    }

    bool operator==(cell c)
    {
      return (i == c.i && j == c.j);
    }

    friend std::ostream& operator<< (std::ostream& o, const cell& c)
    {
      return o << '(' << c.i << ' ' << c.j << ')';
    }

    unsigned i;
    unsigned j;
  };



} // end of namespace cc

#endif /* _CELL_HH */

