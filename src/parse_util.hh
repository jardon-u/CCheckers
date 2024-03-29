#ifndef PARSER_UTIL_H
# define PARSER_UTIL_H

# include <cstdlib>
# include <iostream>
# include <cstring>

//FIXME:compile this.

inline
bool is_space(char s)
{
  return (s == '\t' || s == ' ' || s == '\n' || s == '\r');
}

inline
int match(const char * s, const char * token)
{
  unsigned i;
  for (i = 0; token[i] != '\0'; i++)
    if (s[i] != token[i])
      return 0;
  return i; // diff 0 equal token found
}

inline
bool is_next_token(char ** s, const char * token)
{
  //char word[10];
  //std::std::cerr <<  strncpy(word,*s,10) << std::endl;
  while (is_space(**s))
    (*s)++;
  return (!!match(*s,token)); // int -> bool
}

inline
void eat(const char ** s, const char * token)
{
  while (is_space(**s))
    (*s)++;

  unsigned len;
  if (!(len = match(*s,token)))
  {
    throw std::string("\"") + token + "\" exepted. Get " + std::string(*s,15);
  }
  *s += len;

  //std::cerr << token;
}

inline
bool successful_eat(const char ** s, const char * token)
{
  while (is_space(**s))
    (*s)++;

  unsigned len;
  if (!(len = match(*s,token)))
    return false;
  *s += len;
  return true;
}

inline
void eat_until(char ** s, const char * token)
{
  //int tmp = 0;
  while (not is_next_token(s,token))
    (*s)++;
  *s += std::strlen(token);
  //std::cerr << "..." << token << std::endl;
}


inline
int eat_number(const char ** s)
{
  while (is_space(**s))
    (*s)++;

  char a[5];
  int k = 0;
  while ((*s != '\0' && **s >= '0' && **s <= '9') || (**s == '-'))
    a[k++] = *(*s)++;
  a[k] = 0;

  int i = atoi(a);
  //std::cerr << i;
  return i;
}

#endif // PARSER_UTIL_H
