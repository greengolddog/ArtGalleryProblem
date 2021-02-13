#include <bits/stdc++.h>
//#include <SFML> //TODO
#define elif else if
#define ll long long
using namespace std;

struct point { // точка
	float x;
	float y;
	point(float x = 0.0, float y = 0.0): x(x), y(y){
	}
  friend ostream& operator<< (ostream& out, point p);
  friend istream& operator>> (istream& inp, point& p);
};

ostream& operator<< (ostream& out, point p) {
  out << p.x << ' ' << p.y << "\n";
  return out;
}

istream& operator>> (istream& inp, point& p) {
  inp >> p.x >> p.y;
  return inp;
}

class edge { // сторона
  public:
	point p0;
	point p1;
  edge (point p0 = point(), point p1 = point()): p0(p0), p1(p1) {
  }
	public:
  bool get_intersection(edge line) {    
    float cat1_x = p1.x - p0.x;
    float cat1_y = p1.y - p0.y;
    float cat2_x = line.p1.x - line.p0.x;
    float cat2_y = line.p1.y - line.p0.y;
    float prod1 = (cat1_x * (line.p0.y - p0.y)) - ((line.p0.x - p0.x) * cat1_y);
    float prod2 = (cat1_x * (line.p1.y - p0.y)) - ((line.p1.x - p0.x) * cat1_y);
    if(((prod1 < 0) and (prod2 < 0)) or ((prod1 > 0) and (prod2 > 0))) {
      return false;
    };
    prod1 = (cat2_x * (p0.y - line.p0.y)) - ((p0.x - line.p0.x) * cat2_y);
    prod2 = (cat2_x * (p1.y - line.p0.y)) - ((p1.x - line.p0.x) * cat2_y);
    if(((prod1 < 0) and (prod2 < 0)) or ((prod1 > 0) and (prod2 > 0))){
      return false;
    }
    return true;
  }
  bool pixel_on(point pixel) { // алгоритм Пети
    return (round(((p0.y - p1.y) * pixel.x + (p1.x - p0.x) * pixel.y + (p0.x * p1.y - p1.x * p1.y))) == 0);
  }
  bool pixel_on2(point pixel) { // пиксель лежащий на отрезке (крайние точки выдают true)
    edge self;
    self.p0 = p0;
    self.p1 = p1;
    return (self.pixel_on(pixel) and (((pixel.x <= p0.x and pixel.x >= p1.x) or (pixel.x >= p0.x and pixel.x <= p1.x)) and ((pixel.y <= p0.y and pixel.y >= p1.y) or (pixel.y >= p0.y and pixel.y <= p1.y))));
  }
  point operator* (edge e2) { // пересечение 		двух отрезков
	  edge e1 = *this;
  	float u = ((e2.p1.x - e2.p0.x) * (e1.p0.y - e2.p0.y) - 	(e2.p1.y - e2.p0.y) * (e1.p0.x - e2.p0.x)) / ((e2.p1.y 	- e2.p0.y) * (e1.p1.x - e1.p0.x) - (e2.p1.x - e2.p0.x) 	* (e1.p1.y - e1.p0.y));
  	point ans;
  	ans.x = e1.p0.x + u * (e1.p1.x - e1.p0.x);
  	ans.y = e1.p0.y + u * (e1.p1.y - e1.p0.y);
  	return ans;
	}
  friend ostream& operator<< (ostream& out, edge e);
  friend istream& operator>> (istream& inp, edge& e);
};

ostream& operator<< (ostream& out, edge e) {
  out << "1:" << e.p0 << "2:" << e.p1;
  return out;
}

istream& operator>> (istream& inp, edge& e) {
  inp >> e.p0 >> e.p1;
  return inp;
}

bool get_line_intersection(float x1_1, float y1_1, float x2_1, float y2_1, float x1_2, float y1_2, float x2_2, float y2_2) {
  edge e1, e2;
  e2.p0.x = x1_1;
  e2.p0.y = y1_1;
  e2.p1.x = x2_1;
  e2.p1.y = y2_1;
  e2.p0.x = x1_2;
  e2.p0.y = y1_2;
  e2.p1.x = x2_2;
  e2.p1.y = y2_2;
  return e1.get_intersection(e2);
}

class polygon { // многоугольник
  public:
	vector<point> points;
	int len;
  float area;
	float summ = 0;
  polygon (vector<point> points = vector<point>(0, point())): points(points) {
    len = points.size();
    int j = len - 1;
    for (int i = 0; i < len; i++) {
			summ += points[i].x;
			summ += points[i].y;
      j = i;
    }
    area = len;
  }
	void add(point p) {
		summ += p.x;
		summ += p.y;
		len++;
		area++;
		points.push_back(p);
	}
	bool operator> (const polygon& pol2) const {
		return summ > pol2.summ;
	}
  polygon operator+ (polygon& pol2) {
  	return *this;
	}
  polygon operator* (polygon& pol2) {
  	return pol2;
	}
  friend ostream& operator<< (ostream& out, polygon p);
	friend istream& operator>> (istream& inp, polygon& e);
};

ostream& operator<< (ostream& out, polygon p) {
  for (int i = 0; i < p.len; i++) {
    out << p.points[i];
  }
  return out;
}

istream& operator>> (istream& inp, polygon& e) {
	string str;
	float y;
	inp >> str;
	while (str != ";") {
		inp >> y;
		e.points.push_back(point(stoi(str), y));
	  e.len++;
		inp >> str;
	}
	int j = e.len - 1;
  for (int i = 0; i < e.len; i++) {
		e.summ += e.points[i].x;
		e.summ += e.points[i].y;
    j = i;
  }
  e.area = e.len;
  return inp;
}

set<pair<float, float>> save_be;
map<pair<float, float>, polygon> save;
polygon gallery; // галерея

class guard { // охранник
	float x;
	float y;
  polygon visible_zone;
  public:
  guard (float x = 0.0, float y = 0.0): x(x), y(y) {
		if (save_be.find({x, y}) != save_be.end()) {
			visible_zone = save[{x, y}];
		} else {
			polygon a;
			visible_zone = a;
			for (int k = 0; k < gallery.len; k++) { // часть Петиного алгортма, где находим точки ЗВО
				for (int j = 0; j < gallery.len; j++) {
					if (get_line_intersection(gallery.points[j].x, gallery.points[j].y, gallery.points[j + 1].x, gallery.points[j + 1].y, x, y, x - ((x - 	gallery.points[k].x) * 100000), y - ((y - gallery.points[k].y) * 100000))) {
						point p = edge(gallery.points[j], gallery.points[j + 1]) * edge(point(x, y), point(x - ((x - gallery.points[k].x) * 100000), y - ((y - gallery.points[k].y) * 100000)));
						if (p.x / p.x == 1) {
							visible_zone.add(p);
						}
					}
				}
			}
			int start_point = 0; // начнём с точки 0
			int point_now = 0; // сейчас мы на ней
			bool start = false; // но мы только начали, эта переменная нужна, чтобы while в начале не ломался
			vector<bool> was(visible_zone.len, false); // чтобы не проходить дважды точки
			polygon visible_zone2; // нам будет нужно смотреть на старую зону, этот вектор нужен чтобы её не менять (до поры, до времени)
			while (not (start_point == point_now and start)) { // пока мы не вернулись...
				for (int i = 0; i < visible_zone.len; i++) { // проходимся по всем точкам
					edge e;
					e.p0.x = x;
					e.p0.y = y;
					e.p1 = visible_zone.points[point_now];
          //cout << e << ' ' << visible_zone.points[i];
					if (e.pixel_on(visible_zone.points[i]) and !was[i]) { // и если она лежит на одной прямой с охранником и точкой сейчас...
						point_now = i;
						visible_zone2.add(visible_zone.points[i]);
						was[i] = true;
						break;
					} else { // ...или...
						bool ans = false;
						for (int j; j < gallery.len; j++) {
							if (edge(gallery.points[j], gallery.points[j + 1]).pixel_on2(visible_zone.points[point_now]) and edge(gallery.points[j], gallery.points[j + 1]).pixel_on2(visible_zone.points[i])) { // ...сторона полностью лежит на отрезке (между i и point_now элементами ЗВО)...
								ans = true;
							}
						}
						if (ans) { // ...то нужно ещё проверить, что между этими точками ничего не лежит
							for (int j; j < visible_zone.len; j++) { // идём по всем точкам
								if (j == point_now or j == i) { // если точка равна либо точке i либо point_now, её не рассматриваем
									continue;
								}
								edge e0;// отрезок междк point_now и i
								e0.p0 = visible_zone.points[i];
								e0.p1 = visible_zone.points[point_now];
								if (e0.pixel_on2(visible_zone.points[j])) { // если на нём что-то лежит...
									ans = false; // точка не подходит
								}
							}
							if (ans) { // если мы её ещё не отмели, то добро пожаловать в ЗВО!
								point_now = i;
								visible_zone2.add(visible_zone.points[i]);
								was[i] = true;
								break;
							}
						}
					}
				}
				start = true;
			}
			save_be.insert({x, y});
      //cout << visible_zone;
			visible_zone = visible_zone2; // кладём заполненную ЗВО в соответствующую ячейчку
			save[{x, y}] = visible_zone;
		}
  }
  bool operator< (const guard& g2) const{
  	return ((x+y) < (g2.x+g2.y));
	}
	polygon get_visible_zone() {
		return visible_zone;
	}
	friend ostream& operator<< (ostream& out, guard g);
	friend istream& operator>> (istream& inp, guard& g);
};

ostream& operator<< (ostream& out, guard g) {
  out << g.x << ' ' << g.y << ' ' << g.get_visible_zone();
  return out;
}

istream& operator>> (istream& inp, guard& g) {
	float x, y;
  inp >> x >> y;
	g = guard(x, y);
  return inp;
}

class team { // смена охранников
	vector<guard> guards;
	int size;
  public:
	team (vector<guard> guards) : guards(guards) {
		size = guards.size();
	}
	void add (guard g) { // новый охранник 
		guards.push_back(g);
		size++;
	}
  void del (int pos) { // удалить охранника
    guards.erase(guards.begin() + pos);
    size--;
  }
  void remove (guard new_g, int pos) { // переместить охранника
    del(pos);
    add(new_g);
  }
	team operator+ (team& t2) {
		return *this;
	}
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  cout.tie(0);
  // point p1, p2, p3, p4;
  // cin >> p1 >> p2 >> p3 >> p4;
  // cout << (edge(p1, p2) * edge(p3, p4));
	// edge e1;
	// cin >> e1;
	// cout << e1;
  cin >> gallery;
  guard g;
	cin >> g;
	// field f;
	// cout << f;
}