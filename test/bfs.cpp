#include "test.hpp"

#include "graph.hpp"
#include "main.hpp"

ICY_CASE("graph") {
    auto _g = graph_instance<0>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
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