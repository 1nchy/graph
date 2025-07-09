#include "test.hpp"

#include "graph.hpp"

ICY_CASE("graph") {
    icy::graph<std::string, char, unsigned> _g;
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
    using key_type = typename decltype(_g)::key_type;
    using cost_type = unsigned;
    using vertex_visitor = typename decltype(_g)::vertex_visitor<cost_type>;
    using edge_visitor = typename decltype(_g)::edge_visitor<cost_type>;
    _g.insert("A", 'a');
    _g.insert("B", 'B');
    _g.insert("C", 'c');
    _g.insert("D", 'd');
    _g.insert("E", 'E');
    _g.insert("F", 'f');
    _g.connect("A", "B", 50);
    _g.connect("A", "C", 10);
    _g.connect("A", "E", 45);
    _g.connect("B", "C", 15);
    _g.connect("B", "E", 10);
    _g.connect("C", "A", 20);
    _g.connect("C", "D", 15);
    _g.connect("D", "B", 20);
    _g.connect("D", "E", 35);
    _g.connect("E", "D", 30);
    _g.connect("F", "D", 3);
    ICY_SUBCASE("from A") {
        size_t _cnt = 0;
        _g.bfs("A", [&_cnt](const vertex_type& _v) {
            ++_cnt;
        });
        EXPECT_EQ(_cnt, 5);
    }
    ICY_SUBCASE("from F") {
        size_t _cnt = 0;
        _g.bfs("F", [&_cnt](const vertex_type& _v) {
            ++_cnt;
        });
        EXPECT_EQ(_cnt, 6);
    }
    ICY_SUBCASE("to lower case") {
        bool _all_lower_case = true;
        _g.bfs("F", [&_all_lower_case](const vertex_type& _v) {
            if (isupper(_v.value())) { _all_lower_case = false; }
        });
        EXPECT_FALSE(_all_lower_case);
        _all_lower_case = true;
        _g.bfs("B", [](vertex_type& _v) {
            if (!islower(_v.value())) {
                _v.set_value(tolower(_v.value()));
            }
        });
        _g.bfs("F", [&_all_lower_case](const vertex_type& _v) {
            if (isupper(_v.value())) { _all_lower_case = false; }
        });
        EXPECT_TRUE(_all_lower_case);
    }
}