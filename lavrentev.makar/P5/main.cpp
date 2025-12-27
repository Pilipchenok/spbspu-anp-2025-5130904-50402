#include <cmath>
#include <iostream>
#include <new>
#include <stdexcept>

namespace lavrentev {
const size_t n = 3;

struct point_t {
  double x, y;
};

struct rectangle_t {
  double width, height;
  point_t pos;
};

class Shape {
public:
  virtual ~Shape() noexcept = default;
  virtual double getArea() const noexcept = 0;
  virtual rectangle_t getFrameRect() const noexcept = 0;
  virtual void move(const point_t &c) noexcept = 0;
  virtual void move(double d_x, double d_y) noexcept = 0;

  void scale(double coef);

private:
  virtual void scaleImpl(double coef) = 0;
};

class Rectangle final: public Shape {
public:
  explicit Rectangle(point_t pos, double width, double height);

  double getArea() const noexcept override;
  lavrentev::rectangle_t getFrameRect() const noexcept override;
  void move(const lavrentev::point_t &c) noexcept override;
  void move(double d_x, double d_y) noexcept override;

private:
  void scaleImpl(double coef) override;

  point_t pos_;
  double width_, height_;
};

class Rubber final: public Shape {
public:
  explicit Rubber(point_t pos, point_t outCenter, double rPos, double rOut);

  double getArea() const noexcept override;
  lavrentev::rectangle_t getFrameRect() const noexcept override;
  void move(const lavrentev::point_t &c) noexcept override;
  void move(double d_x, double d_y) noexcept override;

private:
  void scaleImpl(double coef) override;

  point_t pos_;
  point_t outCenter_;
  double rPos_;
  double rOut_;
};

class Polygon final: public Shape {
public:
  explicit Polygon(point_t *vertexes, size_t n);
  ~Polygon();

  double getArea() const noexcept override;
  lavrentev::rectangle_t getFrameRect() const noexcept override;
  void move(const lavrentev::point_t &c) noexcept override;
  void move(double d_x, double d_y) noexcept override;

private:
  void scaleImpl(double coef) override;

  point_t pos_;
  size_t n_;
  point_t *vertexes_;
};

int polyPos(point_t &pos, size_t n, point_t *vertexes);
rectangle_t fullFrame(Shape *const *figures, size_t n);
void userShape(Shape **figures, point_t user_dot, double coef, const size_t n);
void printInfo(Shape **figures);
}

int main()
{
  size_t k = 7;
  lavrentev::point_t *vrtxs = nullptr;

  lavrentev::Shape *figures[lavrentev::n] = {nullptr, nullptr, nullptr};
  try {
    figures[0] = new lavrentev::Rectangle({3, 3}, 8, 5);
    figures[1] = new lavrentev::Rubber({-7, -2}, {-5, 0}, 3.5, 9);
    figures[2] = new lavrentev::Polygon(vrtxs, k);
  } catch (...) {
    for (size_t i = 0; i < lavrentev::n; ++i) {
      delete figures[i];
    }
    delete[] vrtxs;
    return 1;
  }

  lavrentev::printInfo(figures);

  double x, y, coef;
  std::cin >> x >> y >> coef;
  if (std::cin.fail()) {
    std::cerr << "Invalid input" << '\n';
    for (size_t i = 0; i < lavrentev::n; ++i) {
      delete figures[i];
    }
    return 1;
  }
  lavrentev::point_t user_dot = {x, y};
  lavrentev::userShape(figures, user_dot, coef, lavrentev::n);

  std::cout << "Новые данные: " << "\n\n";
  lavrentev::printInfo(figures);

  for (size_t i = 0; i < lavrentev::n; ++i) {
    delete figures[i];
  }
  return 0;
}

void lavrentev::Shape::scale(double coef) {
  if (coef <= 0) {
    throw std::invalid_argument("Coef must be positive");
  }
  scaleImpl(coef);
}

lavrentev::Rectangle::Rectangle(point_t pos, double width, double height):
  pos_(pos),
  width_(width),
  height_(height)
{
  if (width_ <= 0 || height_ <= 0) {
    throw std::invalid_argument("Invalid weight or height");
  }
}

double lavrentev::Rectangle::getArea() const noexcept
{
  return width_ * height_;
}

lavrentev::rectangle_t lavrentev::Rectangle::getFrameRect() const noexcept
{
  rectangle_t data;
  data.height = height_;
  data.width = width_;
  data.pos = pos_;
  return data;
}

void lavrentev::Rectangle::move(const lavrentev::point_t &c) noexcept
{
  pos_ = c;
}

void lavrentev::Rectangle::move(double d_x, double d_y) noexcept
{
  pos_.x += d_x;
  pos_.y += d_y;
}

void lavrentev::Rectangle::scaleImpl(double coef)
{
  width_ *= coef;
  height_ *= coef;
}

lavrentev::Rubber::Rubber(point_t pos, point_t outCenter, double rPos, double rOut):
  pos_(pos),
  outCenter_(outCenter),
  rPos_(rPos),
  rOut_(rOut)
{
  if (rOut_ <= 0 || rPos_ <= 0) {
    throw std::invalid_argument("Invalid value of radius");
  }
}

double lavrentev::Rubber::getArea() const noexcept
{
  return M_PI * rOut_ * rOut_ - M_PI * rPos_ * rPos_;
}

lavrentev::rectangle_t lavrentev::Rubber::getFrameRect() const noexcept
{
  lavrentev::rectangle_t ans;
  ans.pos = outCenter_;
  ans.height = rOut_ * 2;
  ans.width = rOut_ * 2;
  return ans;
}

void lavrentev::Rubber::move(const lavrentev::point_t &c) noexcept
{
  pos_ = c;
}

void lavrentev::Rubber::move(double d_x, double d_y) noexcept {
  pos_.x += d_x;
  pos_.y += d_y;
}

void lavrentev::Rubber::scaleImpl(double coef) {
  rPos_ *= coef;
  rOut_ *= coef;
}

lavrentev::Polygon::Polygon(point_t *vertexes, size_t n):
  vertexes_(vertexes),
  n_(n)
{
  if (n_ <= 2) {
    throw std::invalid_argument("Invalid amount of vertexes");
  }
  try {
    vertexes_ = new lavrentev::point_t[n]{
      {1.2, 5.6}, {3.3, -4.7},
      {1.1, 9.3}, {-5.5, -3.0},
      {-7.3, -0.3}, {-2.1, 4.8}, {3.6, 8.3}
    };
  } catch (std::bad_alloc &e) {
    throw std::bad_alloc();
  }

  point_t pos = {};
  int k = polyPos(pos, n, vertexes_);
  if (k == 1) {
    delete[] vertexes_;
    throw std::logic_error("Polygon not exists");
  }
  pos_ = pos;
}

lavrentev::Polygon::~Polygon()
{
  delete[] vertexes_;
}

double lavrentev::Polygon::getArea() const noexcept
{
  double buf1 = 0.0, buf2 = 0.0, s = 0.0;
  for (size_t i = 0; i < n_ - 1; ++i) {
    buf1 += (vertexes_[i].x * vertexes_[i + 1].y);
    buf2 += (vertexes_[i].y * vertexes_[i + 1].x);
  }
  buf1 += vertexes_[n_ - 1].x * vertexes_[0].y;
  buf2 += vertexes_[n_ - 1].y * vertexes_[0].x;
  s = 0.5 * abs(buf1 - buf2);
  return s;
}

lavrentev::rectangle_t lavrentev::Polygon::getFrameRect() const noexcept
{
  point_t l_u = vertexes_[0], r_b = vertexes_[0];
  for (size_t i = 0; i < n_; ++i) {
    if (vertexes_[i].x < l_u.x) {
      l_u.x = vertexes_[i].x;
    }
    if (vertexes_[i].y > l_u.y) {
      l_u.y = vertexes_[i].y;
    }
    if (vertexes_[i].x > r_b.x) {
      r_b.x = vertexes_[i].x;
    }
    if (vertexes_[i].y < r_b.y) {
      r_b.y = vertexes_[i].y;
    }
  }
  rectangle_t ans;
  double w = r_b.x - l_u.x;
  double h = l_u.y - r_b.y;
  point_t p;
  p.x = (r_b.x + l_u.x) * 0.5;
  p.y = (r_b.y + l_u.y) * 0.5;
  ans.pos = p;
  ans.width = w;
  ans.height = h;
  return ans;
}

void lavrentev::Polygon::move(const lavrentev::point_t &c) noexcept
{
  double dx = c.x - pos_.x;
  double dy = c.y - pos_.y;

  pos_ = c;

  for (size_t i = 0; i < n_; ++i) {
    vertexes_[i].x += dx;
    vertexes_[i].y += dy;
  }
}

void lavrentev::Polygon::move(double d_x, double d_y) noexcept
{
  pos_.x += d_x;
  pos_.y += d_y;

  for (size_t i = 0; i < n_; ++i) {
    vertexes_[i].x += d_x;
    vertexes_[i].y += d_y;
  }
}

void lavrentev::Polygon::scaleImpl(double coef)
{
  for (size_t i = 0; i < n_; ++i) {
    double d_x = vertexes_[i].x - pos_.x;
    double d_y = vertexes_[i].y - pos_.y;
    d_x *= coef;
    d_y *= coef;
    vertexes_[i].x = pos_.x + d_x;
    vertexes_[i].y = pos_.y + d_y;
  }
}

int lavrentev::polyPos(point_t &pos, size_t n, point_t *vertexes)
{
  if (n < 3) {
    return 1;
  }

  double square = 0.0;
  for (size_t i = 0; i < n; ++i) {
    size_t j = (i + 1) % n;
    square += (vertexes[i].x * vertexes[j].y) - (vertexes[j].x * vertexes[i].y);
  }
  square *= 0.5;

  if (square != 0.0) {
    double c_x = 0.0;
    for (size_t i = 0; i < n; ++i) {
      size_t j = (i + 1) % n;
      double buf_1 = vertexes[i].x + vertexes[j].x;
      double buf_2 = vertexes[i].x * vertexes[j].y;
      double buf_3 = vertexes[j].x * vertexes[i].y;
      c_x += buf_1 * (buf_2 - buf_3);
    }
    c_x /= (6 * square);

    double c_y = 0.0;
    for (size_t i = 0; i < n; ++i) {
      size_t j = (i + 1) % n;
      double buf_1 = vertexes[i].y + vertexes[j].y;
      double buf_2 = vertexes[i].x * vertexes[j].y;
      double buf_3 = vertexes[j].x * vertexes[i].y;
      c_y += buf_1 * (buf_2 - buf_3);
    }
    c_y /= (6 * square);
    pos.x = c_x;
    pos.y = c_y;
  } else {
    return 1;
  }
  return 0;
}

lavrentev::rectangle_t lavrentev::fullFrame(lavrentev::Shape *const *figures, size_t n)
{
  double left = 0.0, right = 0.0, up = 0.0, down = 0.0;
  for (size_t i = 0; i < n; ++i) {
    lavrentev::rectangle_t buf = figures[i]->getFrameRect();
    if (buf.pos.x - buf.width * 0.5 < left) {
      left = buf.pos.x - buf.width * 0.5;
    }
    if (buf.pos.x + buf.width * 0.5 > right) {
      right = buf.pos.x + buf.width * 0.5;
    }
    if (buf.pos.y - buf.height * 0.5 < down) {
      down = buf.pos.y - buf.height * 0.5;
    }
    if (buf.pos.y + buf.height * 0.5 > up) {
      up = buf.pos.y + buf.height * 0.5;
    }
  }
  lavrentev::rectangle_t ff;
  ff.height = up - down;
  ff.width = right - left;
  ff.pos.y = (up + down) * 0.5;
  ff.pos.x = (right + left) * 0.5;
  return ff;
}

void lavrentev::userShape(
  lavrentev::Shape **figures,
  point_t user_dot,
  double coef,
  const size_t n)
{
  for (size_t i = 0; i < n; ++i) {
    point_t point1 = figures[i]->getFrameRect().pos;
    figures[i]->move(user_dot);
    point_t delta = {user_dot.x - point1.x, user_dot.y - point1.y};
    figures[i]->scale(coef);
    point_t res = {user_dot.x - delta.x * coef, user_dot.y - delta.y * coef};
    figures[i]->move(res);
  }
}

void lavrentev::printInfo(lavrentev::Shape **figures)
{
  std::cout << "Площадь Rectangle: " << figures[0]->getArea() << '\n';
  std::cout << "Площадь Rubber: " << figures[1]->getArea() << '\n';
  std::cout << "Площадь Polygon " << figures[2]->getArea() << '\n';
  double buf_1 = figures[0]->getArea();
  double buf_2 = figures[1]->getArea();
  double buf_3 = figures[2]->getArea();
  std::cout << "Суммарная площадь: " << buf_1 + buf_2 + buf_3;
  std::cout << "\n\n";

  lavrentev::rectangle_t rg = figures[0]->getFrameRect();
  std::cout << "Ограничивающий прямоугольник Rectangle:" << '\n';
  std::cout << '\t' << "Центр: {" << rg.pos.x << ", " << rg.pos.y << '}' << '\n';
  std::cout << '\t' << "Длина: " << rg.width << '\n';
  std::cout << '\t' << "Высота: " << rg.height << '\n';

  lavrentev::rectangle_t rug = figures[1]->getFrameRect();
  std::cout << "Ограничивающий прямоугольник Rubber:" << '\n';
  std::cout << '\t' << "Центр: {" << rug.pos.x << ", " << rug.pos.y << '}' << '\n';
  std::cout << '\t' << "Длина: " << rug.width << '\n';
  std::cout << '\t' << "Высота: " << rug.height << '\n';

  lavrentev::rectangle_t pg = figures[2]->getFrameRect();
  std::cout << "Ограничивающий прямоугольник Polygon:" << '\n';
  std::cout << '\t' << "Центр: {" << pg.pos.x << ", " << pg.pos.y << '}' << '\n';
  std::cout << '\t' << "Длина: " << pg.width << '\n';
  std::cout << '\t' << "Высота: " << pg.height << "\n\n";

  lavrentev::rectangle_t ff = lavrentev::fullFrame(figures, lavrentev::n);
  std::cout << "Общий ограничивающий прямоугольник:" << '\n';
  std::cout << '\t' << "Центр: {" << ff.pos.x << ", " << ff.pos.y << '}' << '\n';
  std::cout << '\t' << "Длина: " << ff.width << '\n';
  std::cout << '\t' << "Высота: " << ff.height << "\n\n";
}
