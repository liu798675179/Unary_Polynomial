#pragma once

#include <algorithm>
#include <iostream>
#include <regex>
#include <list>

struct item {
	friend item operator+(item l, item r) { return item(l.coefficient + r.coefficient, l.power); }
	friend item operator-(item l, item r) { return item(l.coefficient - r.coefficient, l.power); }
	friend bool operator==(item l, item r) { return l.coefficient == r.coefficient && l.power == r.power; }

	item() = default;
	item(const int &co, const int &po) : coefficient(co), power(po) {}
	~item() = default;

	int coefficient = 0;
	int power = 0;
};

class Unary_Polynomial {
	friend Unary_Polynomial operator+(Unary_Polynomial l, Unary_Polynomial r);
	friend Unary_Polynomial operator-(Unary_Polynomial l, Unary_Polynomial r);
	friend bool operator==(Unary_Polynomial l, Unary_Polynomial r);
	friend bool operator!=(Unary_Polynomial l, Unary_Polynomial r);
public:
	Unary_Polynomial() = default;
	~Unary_Polynomial() = default;

	void Input(const std::string &express);
	void Insert(const int &co, const int &po);
	void Print();
	void Clear();

private:
	std::list<item> unary_poly;
};

inline void Unary_Polynomial::Input(const std::string &express) {
	std::regex pat_regex("\\+?(\\-?[[:digit:]]+)x\\^(\\-?[[:digit:]]+)|\\+?(\\-?x)\\^(\\-?[[:digit:]]+)|\\+?(\\-?[[:digit:]]+)x|\\+?(\\-?[[:digit:]]+)|\\+?(\\-?x)");

	for (std::sregex_iterator it(express.begin(), express.end(), pat_regex), end_it; it != end_it; ++it) {
		//std::cout << it->str(0) << std::endl;
		//std::cout << " 1: " << it->str(1) << " 2: " << it->str(2) << " 3: " << it->str(3) << " 4: " << it->str(4) << " 5: " << it->str(5) << " 6: " << it->str(6) << " 7: " << it->str(7) << std::endl;

		if(it->str(1) != "" && it->str(2) != "") {
			auto co = stoi(it->str(1));
			auto po = stoi(it->str(2));
			Insert(co, po);
		}
		if(it->str(4) != "") {
			auto co = 1;
			if(it->str(3) == "-x") {
				co = -1;
			}
			auto po = stoi(it->str(4));
			Insert(co, po);
		}
		if(it->str(5) != "") {
			auto co = stoi(it->str(5));
			auto po = 1;
			Insert(co, po);
		}
		if(it->str(6) != "") {
			auto co = stoi(it->str(6));
			auto po = 0;
			Insert(co, po);
		}
		if (it->str(7) != "") {
			auto co = 1;
			if (it->str(7) == "-x") {
				co = -1;
			}
			auto po = 1;
			Insert(co, po);
		}
	}

	unary_poly.sort([](const item &i, const item &j) {return i.power < j.power; });

	auto temp_count = std::count_if(unary_poly.begin(), unary_poly.end(), [](const item &i) {return i.coefficient == 0; });
	if(temp_count == unary_poly.size()) {
		unary_poly.clear();
	}
}

inline void Unary_Polynomial::Insert(const int& co, const int& po) {
	auto find_it = std::find_if(unary_poly.begin(), unary_poly.end(), [&](const item &j) {return po == j.power; });
	if (find_it != unary_poly.end()) {
		find_it->coefficient += co;
	}
	else {
		item temp_item(co, po);
		unary_poly.push_back(temp_item);
	}
}

inline void Unary_Polynomial::Print() {
	if (!unary_poly.empty()) {
		for (auto i = unary_poly.begin(); i != unary_poly.end(); ++i) {
			if (i->coefficient > 0 && i != unary_poly.begin()) {
				std::cout << "+";
			}
			if(i->coefficient != 0 && i->power == 0) {
				std::cout << i->coefficient;
			}
			if (i->power != 0 && i->power != 1 && i->coefficient != 0 && i->coefficient != 1 && i->coefficient != -1) {
				std::cout << i->coefficient << "x^" << i->power;
			}
			if (i->power != 0 && i->power != 1 && i->coefficient != 0 && (i->coefficient == 1 || i->coefficient == -1)) {
				if (i->coefficient == -1) {
					std::cout << "-";
				}
				std::cout << "x^" << i->power;
			}
			if (i->power == 1 && i->coefficient != 0 && i->coefficient != 1 && i->coefficient != -1) {
				std::cout << i->coefficient << "x";
			}
			if (i->power == 1 && i->coefficient != 0 && (i->coefficient == 1 || i->coefficient == -1)) {
				if(i->coefficient == -1) {
					std::cout << "-";
				}
				std::cout << "x";
			}
		}
	}
	else {
		std::cout << 0;
	}

	std::cout << std::endl << std::endl;
}

inline void Unary_Polynomial::Clear() {
	unary_poly.clear();
}


inline Unary_Polynomial operator+(Unary_Polynomial l, Unary_Polynomial r) {
	Unary_Polynomial temp_UP;
	item temp_item;

	for(auto &i : l.unary_poly) {
		auto find_it = std::find_if(r.unary_poly.begin(), r.unary_poly.end(), [&](const item &j) {return i.power == j.power; });
		if(find_it != r.unary_poly.end()) {
			temp_item = i + *find_it;

			r.unary_poly.erase(find_it); //erase r which with the same power of l
		}
		else {
			temp_item = i;
		}

		temp_UP.unary_poly.push_back(temp_item);
	}

	for(auto &i : r.unary_poly) {
		temp_item = i;
		temp_UP.unary_poly.push_back(temp_item);
	}

	temp_UP.unary_poly.sort([](const item &i, const item &j) {return i.power < j.power; });

	return temp_UP;
}

inline Unary_Polynomial operator-(Unary_Polynomial l, Unary_Polynomial r) {
	Unary_Polynomial temp_UP;
	item temp_item;

	if (r != l) {
		for (auto &i : l.unary_poly) {
			auto find_it = std::find_if(r.unary_poly.begin(), r.unary_poly.end(), [&](const item &j) {return i.power == j.power; });
			if (find_it != r.unary_poly.end()) {
				temp_item = i - *find_it;

				r.unary_poly.erase(find_it); //erase r which with the same power of l
			}
			else {
				temp_item = i;
			}

			temp_UP.unary_poly.push_back(temp_item);
		}

		for (auto &i : r.unary_poly) {
			temp_item.coefficient = -i.coefficient;
			temp_item.power = i.power;
			temp_UP.unary_poly.push_back(temp_item);
		}

		temp_UP.unary_poly.sort([](const item &i, const item &j) {return i.power < j.power; });
	}

	return temp_UP;
}

inline bool operator==(Unary_Polynomial l, Unary_Polynomial r) {
	if(l.unary_poly.size() == r.unary_poly.size()) {
		for(auto i_l = l.unary_poly.begin(), i_r = r.unary_poly.begin(); i_l != l.unary_poly.end(); ++i_l, ++i_r) {
			if(*i_l == *i_r) {
				return true;
			}
		}
	}
	
	return false;
}

inline bool operator!=(Unary_Polynomial l, Unary_Polynomial r){
	return !(l == r);
}
