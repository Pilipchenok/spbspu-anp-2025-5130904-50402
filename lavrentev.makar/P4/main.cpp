#include <iostream>
#include <cstring>
#include <ios>

namespace lavrentev
{
  size_t dif_lat(char* ans, char* s1, size_t& ex);
  void uni_two(char* s1, const char s2[], size_t ex, size_t ex2, char* result);
  char * getline(std::istream & in, size_t& n);
}

int main()
{
  char* s1 = nullptr;
  size_t ex = 0;
  s1 = lavrentev::getline(std::cin, ex);
  if (s1 == nullptr)
  {
    std::cerr << "Memory allocation fail" << '\n';
    return 1;
  }

  if(ex == 0)
  {
    std::cerr << "Invalid string" << '\n';
    delete[] s1;
    return 1;
  }

  char* buf1 = new char[ex];
  if (buf1 == nullptr)
  {
    std::cerr << "Memory allocation fail" << '\n';
    delete[] s1;
    return 1;
  }

  const char s2[] = "def ";
  size_t ex2 = 4;

  char* result = new char[ex + ex2];
  if (result == nullptr)
  {
    std::cerr << "Memory allocation fail" << '\n';
    delete[] s1;
    delete[] buf1;
    return 1;
  }

  int ans_7 = lavrentev::dif_lat(buf1, s1, ex);
  std::cout << "Ans 7: " << ans_7 << '\n';

  lavrentev::uni_two(s1, s2, ex, ex2, result);
  std::cout << "Ans 12: ";
  for (size_t i = 0; i < ex + ex2; ++i)
  {
    std::cout << result[i];
  }
  std::cout << '\n';

  delete[] s1;
  delete[] buf1;
  delete[] result;
}

size_t lavrentev::dif_lat(char* buf1, char* s1, size_t& ex)
{
  size_t answer = 0;

  for (size_t i = 0; i < ex; ++i)
  {
    buf1[i] = '#';
  }

  for (size_t i = 0; i < ex; ++i)
  {
    bool flag = false;
    for (size_t j = 0; j < ex; ++j)
    {
      if (buf1[j] == s1[i])
      {
        flag = true;
        break;
      }
    }

    if (!flag && isalpha(s1[i]))
    {
      buf1[i] = s1[i];
    }
  }

  for (size_t i = 0; i < ex; ++i)
  {
    if (buf1[i] != '#')
    {
      ++answer;
    }
  }

  return answer;
}

void lavrentev::uni_two(char* s1, const char s2[], size_t ex, size_t ex2, char* result)
{
  size_t min;
  std::string max_s;
  if (ex < ex2)
  {
    min = ex;
    max_s = s2;
  }
  else
  {
    min = ex2;
    max_s = s1;
  }

  size_t t = 0;

  for (size_t i = 0; i < min; ++i)
  {
    result[t] =  s1[i];
    result[t + 1] =  s2[i];
    t += 2;
  }

  t = min;

  for (size_t i = min * 2; i < ex + ex2; ++i)
  {
    result[i] = max_s[min];
    ++min;
  }
}

char * lavrentev::getline(std::istream & in, size_t & n)
{
  size_t t = 10;
  size_t size = 0;

  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws)
  {
    in >> std::noskipws;
  }

  char* s = nullptr;

  try {
    s = new char[t];
  } catch (const std::bad_alloc&) {
    n = 0;
    return nullptr;
  }

  char st;
  while (in >> st && st != '\n')
  {
    if (size == t - 1)
    {
      size_t new_t = t + t / 2;
      char* new_s = nullptr;

      try {
        new_s = new char[new_t];
      } catch (const std::bad_alloc&) {
        delete[] s;
        n = 0;
        return nullptr;
      }

      for (size_t j = 0; j < size; ++j)
      {
        new_s[j] = s[j];
      }

      delete[] s;
      s = new_s;
      t = new_t;
    }

    s[size] = st;
    ++size;
  }

  s[size] = '\0';
  n = size;

  if (is_skipws)
  {
    in >> std::skipws;
  }

  return s;
}
