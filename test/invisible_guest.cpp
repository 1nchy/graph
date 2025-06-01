#include "main.hpp"

#include "graph.hpp"

#include <string>
#include <vector>
#include <queue>

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

int main() {
    // vertex<name, alive>
    icy::graph<std::string, bool, relation_type> _movie;
    using vertex_type = typename decltype(_movie)::vertex_type;
    using edge_type = typename decltype(_movie)::edge_type;
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

    EXPECT_EQ(_movie.vertices(), 6);
    EXPECT_EQ(_movie.edges(), 12);

    _movie.connect("Doria", "Daniel", MURDER);
    _movie.connect("Doria", "Laura", MURDER);
    auto _range = _movie.get_edge("Doria", "Laura");
    unsigned _relationship = NONE;
    for (auto _i = _range.first; _i != _range.second; ++_i) {
        _relationship = _relationship | _i->second->value();
    }
    EXPECT_EQ(_relationship, LOVER | MURDER);

    _movie.erase("Laura");
    _movie.erase("Daniel");
    EXPECT_EQ(_movie.vertices(), 4);
    EXPECT_EQ(_movie.edges(), 5);
    return 0;
}