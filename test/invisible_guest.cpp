#include "test.hpp"

#include "graph.hpp"

#include <string>
#include <vector>

enum relation_type : unsigned {
    NONE = 0x0,
    WIFE = 0x1 << 0,
    LOVER = 0x1 << 1,
    SON = 0x1 << 2,
    FATHER = 0x1 << 3,
    MOTHER = 0x1 << 4,
    MURDER = 0x1 << 5,
    HIRE = 0x1 << 6,
    HELP = 0x1 << 7,
    INVESTIGATE = 0x1 << 8,
};

ICY_CASE("invisible_guest") {
    // vertex<name, alive>
    icy::multigraph<std::string, bool, relation_type> _movie;
    using vertex_type = typename decltype(_movie)::vertex_type;
    using edge_type = typename decltype(_movie)::edge_type;
    using key_type = typename decltype(_movie)::key_type;
    _movie.insert("Doria", true);
    _movie.insert("Laura", true);
    _movie.insert("Goodman", true);
    _movie.insert("Daniel", false);
    _movie.insert("Tomas", true);
    _movie.insert("Virginia", true);

    _movie.biconnect("Tomas", "Virginia", WIFE);
    _movie.connect("Tomas", "Daniel", FATHER);
    _movie.connect("Virginia", "Daniel", FATHER);
    _movie.connect("Daniel", "Tomas", SON);
    _movie.connect("Daniel", "Virginia", SON);
    _movie.biconnect("Doria", "Laura", LOVER);
    _movie.connect("Doria", "Goodman", HIRE);
    _movie.connect("Goodman", "Doria", HELP);
    _movie.connect("Tomas", "Laura", HELP);

    _movie.get_vertex("Laura").set_value(false);
    _movie.connect("Tomas", "Doria", INVESTIGATE);

    EXPECT_EQ(_movie.order(), 6);
    EXPECT_EQ(_movie.size(), 12);

    _movie.connect("Laura", "Daniel", MURDER);
    _movie.connect("Doria", "Laura", MURDER);
    _movie.disconnect("Laura", "Daniel");
    _movie.connect("Doria", "Daniel", MURDER);

    ICY_SUBCASE("relation") {
        auto _range = _movie.get_edge("Doria", "Laura");
        unsigned _relationship = NONE;
        for (auto _i = _range.first; _i != _range.second; ++_i) {
            _relationship = _relationship | _i->second->value();
        }
        EXPECT_EQ(_relationship, LOVER | MURDER);
    }

    _movie.disconnect("Tomas", "Doria");
    EXPECT_EQ(_movie.order(), 6);
    EXPECT_EQ(_movie.size(), 13);

    ICY_SUBCASE("floyd") {
        const auto _floyd = _movie.floyd<unsigned>([](const edge_type&) -> unsigned {
            return 1u;
        });
        std::function<std::vector<key_type>(const key_type& _i, const key_type& _j)> get_trace = 
        [&get_trace, &_floyd](const key_type& _i, const key_type& _j) -> std::vector<key_type> {
            assert(_floyd.contains(_i) && _floyd.contains(_j));
            if (!_floyd.adjacent(_i, _j)) { return {}; }
            if (_i == _j) { return {_i}; }
            const key_type& _k = _floyd.get_edge(_i, _j)->value().second;
            assert(_k != _j);
            if (_i == _k) { return {_i, _j}; }
            auto _front = get_trace(_i, _k);
            auto _back = get_trace(_k, _j);
            assert(!_front.empty() && !_back.empty());
            assert(_front.back() == _back.front());
            _front.insert(_front.end(), _back.begin() + 1, _back.end());
            return _front;
        };
        const auto _trace = get_trace("Daniel", "Doria");
        const std::vector<key_type> _expected_trace = {"Daniel", "Tomas", "Laura", "Doria"};
        EXPECT_EQ(_trace, _expected_trace);
    }
    ICY_SUBCASE("bfs") {
        std::vector<bool> _alive;
        _alive.reserve(_movie.order());
        _movie.bfs("Laura", [&_alive](const vertex_type& _v) {
            _alive.push_back(_v.value());
        });
        EXPECT_EQ(_alive.size(), 6);
        EXPECT_EQ(std::count(_alive.cbegin(), _alive.cend(), false), 2);
    }
    ICY_SUBCASE("operator=") {
        auto _movie_alive = _movie;
        EXPECT_EQ(_movie, _movie_alive);

        _movie_alive.erase("Laura");
        const auto _from_tomas = _movie_alive.dijkstra<unsigned>("Tomas", [](const edge_type&) -> unsigned {
            return 1u;
        });
        EXPECT_FALSE(_from_tomas.contains("Doria"));
        EXPECT_EQ(_movie_alive.order(), 5);
        EXPECT_EQ(_movie_alive.size(), 9);

        _movie_alive.erase("Daniel");
        EXPECT_EQ(_movie_alive.order(), 4);
        EXPECT_EQ(_movie_alive.size(), 4);

        EXPECT_NE(_movie, _movie_alive);
    }
}