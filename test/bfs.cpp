#include "test.hpp"

#include "graph.hpp"
#include "main.hpp"

ICY_CASE("graph") {
    auto _g = graph_instance<1>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
    ICY_SUBCASE("from A") {
        size_t _cnt = 0;
        _g.bfs("A", [&_cnt](const key_type& _k, const vertex_type& _v) {
            ++_cnt;
        });
        EXPECT_EQ(_cnt, 7);
    }
    ICY_SUBCASE("from M") {
        size_t _cnt = 0;
        _g.bfs("M", [&_cnt](const key_type& _k, const vertex_type& _v) {
            ++_cnt;
        });
        EXPECT_EQ(_cnt, 3);
    }
    ICY_SUBCASE("to lower case") {
        std::unordered_set<key_type> _s;
        for (const auto& [_k, _v] : _g.vertices()) {
            _s.insert(_k);
        }
        size_t _upper_cnt = 0;
        while (!_s.empty()) { // update all characters to lower case
            _g.bfs(*_s.begin(), [&_s, &_upper_cnt](const key_type& _k, vertex_type& _v) {
                if (!_s.contains(_k)) {
                    return;
                }
                if (!islower(_v.value())) {
                    ++_upper_cnt;
                    _v.set_value(tolower(_v.value()));
                }
                _s.erase(_k);
            });
        }
        EXPECT_EQ(_upper_cnt, 4);
        for (const auto& [_k, _v] : _g.vertices()) {
            EXPECT_TRUE(islower(_v->value()));
        }
    }
}