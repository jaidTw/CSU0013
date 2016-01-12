/*
 * CSU0013 Data Structures
 * Project 2
 * Kruskal's MST Algorithm
 * Please compile this program with flag '-std=c++14' or '-std=gnu++14'
 */

#include <cstdio>
#include <vector>
#include <tuple>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>

// following codes in std are from Boost library, provides hash function for C++ STL tuple type.
namespace std{
    namespace {
        // Code from boost
        // Reciprocal of the golden ratio helps spread entropy
        //     and handles duplicates.
        // See Mike Seymour in magic-numbers-in-boosthash-combine:
        //     http://stackoverflow.com/questions/4948780

        template <class T>
        inline void hash_combine(std::size_t& seed, T const& v) {
            seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        // Recursive template code derived from Matthieu M.
        template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl {
            static void apply(size_t& seed, Tuple const& tuple) {
                HashValueImpl<Tuple, Index-1>::apply(seed, tuple);
                hash_combine(seed, std::get<Index>(tuple));
            }
        };

        template <class Tuple>
        struct HashValueImpl<Tuple,0> {
            static void apply(size_t& seed, Tuple const& tuple) {
                hash_combine(seed, std::get<0>(tuple));
            }
        };
    }

    template <typename ... TT>
    struct hash<std::tuple<TT...>> {
        size_t operator()(std::tuple<TT...> const& tt) const {                                              
            size_t seed = 0;                             
            HashValueImpl<std::tuple<TT...> >::apply(seed, tt);    
            return seed;                                 
        } 
    };
}

using namespace std;

// The template parameter type should be hashable.
template<typename T>
class disjoint_sets {
public:
    disjoint_sets(void) : _set_count(0), err(0) {};
    size_t make_set(const T &elem);
    size_t find_set(const T &elem);
    size_t union_set(size_t setA, size_t setB);
    unordered_set<T> get_set(size_t set);
    short err;
private:
	static const short ERR_NOTEXIST = 1;
	static const short ERR_EXIST = 2;
	static const short ERR_SAME = 3;
    size_t _set_count;
    unordered_map<T, size_t> _elem_lst;
    vector<unordered_set<T>> _set_lst;
    vector<size_t> _empty_lst;
};

typedef size_t vertex_t;
typedef int weight_t;
typedef tuple<vertex_t, vertex_t> edge_t;
typedef vector<vector<vertex_t>> graph_t;
typedef map<edge_t, weight_t> wtable_t;

struct tree_t{
	tree_t() : weight(0) {};
	vector<edge_t> edges;
	weight_t weight;
};

int Read_Graph(graph_t &G, wtable_t &W);
tree_t Kruskal(const graph_t &G, wtable_t &W);
int Print_MST(const tree_t &T, wtable_t &W);

int main(void) {
	graph_t G;
	wtable_t W;
	Read_Graph(G, W);
	tree_t MST = Kruskal(G, W);
	Print_MST(MST, W);
    return 0;
}

int Read_Graph(graph_t &G, wtable_t &W) {
	int vertex_num, edge_num;
	scanf("%d %d", &vertex_num, &edge_num);
	G.resize(vertex_num);

	vertex_t vertex1, vertex2;
	weight_t weight;	
	for(int edge_count = 0; edge_count < edge_num; ++edge_count) {
		scanf("%zd %zd %d", &vertex1, &vertex2, &weight);
		if(vertex1 > vertex2) {
			vertex_t temp = vertex1;
			vertex1 = vertex2;
			vertex2 = temp;
		}
		G[vertex1 - 1].push_back(vertex2 - 1);
		W[make_tuple(vertex1 - 1, vertex2 - 1)] = weight;
	}
	return 0;
}

tree_t Kruskal(const graph_t &G, wtable_t &W) {
	tree_t MST;
	disjoint_sets<vertex_t> s;
	for(vertex_t v = 0; v != G.size(); ++v)
		s.make_set(v);

	// sort edges by weight.
	vector<pair<edge_t, weight_t>> Wvec(W.begin(), W.end());
	sort(Wvec.begin(), Wvec.end(), [&](auto A, auto B) { return get<1>(A) < get<1>(B); });
	for(auto &edge : Wvec) {
		auto u = get<0>(get<0>(edge)), v = get<1>(get<0>(edge));
		if(s.find_set(u) != s.find_set(v)) {
			MST.edges.push_back(make_tuple(u, v));
			MST.weight += W[make_tuple(u, v)];
			s.union_set(s.find_set(u), s.find_set(v));
		}
	}
	return MST;
}

int Print_MST(const tree_t &MST, wtable_t &W) {
	printf("%d\n", MST.weight);
	for(auto &edge : MST.edges)
		printf("%zd %zd %d\n", get<0>(edge) + 1, get<1>(edge) + 1, W[edge]);
	
	return 0;
}

// return set number on success.
// return 0 and set err to ERR_EXIST on error.
template<typename T>
size_t disjoint_sets<T>::make_set(const T &elem) {
	// check if already in the set
	if(_elem_lst.find(elem) != _elem_lst.end()) {
		err = ERR_EXIST;
		return 0;
	}

	// check if there are unused entries.
	if(!_empty_lst.empty()) {
		_elem_lst[elem] = _empty_lst.back();
		_set_lst[_empty_lst.back() - 1].insert(elem);
		_empty_lst.pop_back();
	}
	else {
		_elem_lst[elem] = _set_count + 1;
		unordered_set<T> tmp_set;
		tmp_set.insert(elem);
		_set_lst.push_back(tmp_set);
	}
	++_set_count;
	err = 0;
	return _elem_lst[elem];
}

// return set number on success.
// return 0 and set err to ERR_NOTEXIST on error.
template<typename T>
size_t disjoint_sets<T>::find_set(const T &elem) {
	if(_elem_lst.find(elem) == _elem_lst.end()) {
		err = ERR_NOTEXIST;
		return 0;
	}
	err = 0;
	return _elem_lst[elem];
}

// return unioned set number on success.
// return 0 and set err to ERR_SAME or ERR_NOTEXIST on error.
template<typename T>
size_t disjoint_sets<T>::union_set(size_t setA, size_t setB) {
	// if two sets are equal.
	if(setA == setB) {
		err = ERR_SAME;
		return 0;
	}
	// if any set is not exist.
	if(setA - 1 >= _set_lst.size() || setB - 1 >= _set_lst.size() ||
		_set_lst[setA - 1].empty() || _set_lst[setB - 1].empty()) {
		err = ERR_NOTEXIST;
		return 0;
	}

	--_set_count;
	err = 0;
	// move the elements in smaller set to larger one.
	if(_set_lst[setA - 1].size() >= _set_lst[setB - 1].size()) {
		for(auto &elemB : _set_lst[setB - 1]) {
			_elem_lst[elemB] = setA;
			_set_lst[setA - 1].insert(elemB);
		}
		_set_lst[setB - 1].clear();
		_empty_lst.push_back(setB);
		return setA;
	}
	else {
		for(auto &elemA : _set_lst[setA - 1]) {
			_elem_lst[elemA] = setB;
			_set_lst[setB - 1].insert(elemA);
		}
		_set_lst[setA - 1].clear();
		_empty_lst.push_back(setA);
		return setB;
	}
}

