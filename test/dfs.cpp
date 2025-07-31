#include "test.hpp"

#include "graph.hpp"
#include "main.hpp"

ICY_CASE("graph") {
    auto _g = graph_instance<1>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
    ICY_SUBCASE("from A") {
        size_t _cnt = 0;
        _g.dfs("A", [&_cnt](const key_type& _k, const vertex_type& _v) {
            ++_cnt;
        });
        EXPECT_EQ(_cnt, 7);
    }
    ICY_SUBCASE("from M") {
        size_t _cnt = 0;
        _g.dfs("M", [&_cnt](const key_type& _k, const vertex_type& _v) {
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
            _g.dfs(*_s.begin(), [&_s, &_upper_cnt](const key_type& _k, vertex_type& _v) {
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
ICY_CASE("multigraph") {
    auto _g = multigraph_instance<1>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
    ICY_SUBCASE("from A") {
        size_t _cnt = 0;
        _g.dfs("A", [&_cnt](const key_type& _k, const vertex_type& _v) {
            ++_cnt;
        });
        EXPECT_EQ(_cnt, 8);
    }
    ICY_SUBCASE("from H") {
        size_t _cnt = 0;
        _g.dfs("H", [&_cnt](const key_type& _k, const vertex_type& _v) {
            ++_cnt;
        });
        EXPECT_EQ(_cnt, 1);
    }
    ICY_SUBCASE("to lower case") {
        std::unordered_set<key_type> _s;
        for (const auto& [_k, _v] : _g.vertices()) {
            _s.insert(_k);
        }
        size_t _upper_cnt = 0;
        while (!_s.empty()) { // update all characters to lower case
            _g.dfs(*_s.begin(), [&_s, &_upper_cnt](const key_type& _k, vertex_type& _v) {
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
        EXPECT_EQ(_upper_cnt, 3);
        for (const auto& [_k, _v] : _g.vertices()) {
            EXPECT_TRUE(islower(_v->value()));
        }
    }
}
ICY_CASE("graph preorder and postorder") {
    auto _g = graph_instance<0>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
    std::vector<key_type> _preorder, _preorder2;
    std::vector<key_type> _postorder, _postorder2;
    std::vector<key_type> _backtrace, _backtrace2;
    std::unordered_set<key_type> _s;
    std::function<void(const key_type&)> dfs =
    [&dfs, &_g, &_preorder2, &_postorder2, &_backtrace2, &_s] (const key_type& _k) -> void {
        if (_s.contains(_k)) { return; }
        const vertex_type* const _v = _g.get_vertex(_k);
        _preorder2.push_back(_k);
        _s.insert(_k);
        const auto _outs = _v->out();
        std::vector<key_type> _vec; _vec.reserve(_v->outdegree());
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (_s.contains(_out->first)) { continue; }
            _vec.push_back(_out->first);
        }
        for (auto _i = _vec.crbegin(); _i != _vec.crend(); ++_i) {
            if (!_s.contains(*_i)) {
                dfs(*_i);
                _backtrace2.push_back(_k);
            }
        }
        _postorder2.push_back(_k);
    };
    ICY_SUBCASE("from A") {
        const auto _count = _g.dfs("A", [&_preorder](const key_type& _k, const vertex_type& _v) {
            _preorder.push_back(_k);
        }, [&_postorder](const key_type& _k, const vertex_type& _v) {
            _postorder.push_back(_k);
        }, [&_backtrace](const key_type& _k, const vertex_type& _v, const key_type& _u) {
            _backtrace.push_back(_k);
        });
        dfs("A");
        EXPECT_EQ(_preorder, _preorder2);
        EXPECT_EQ(_postorder, _postorder2);
        EXPECT_EQ(_backtrace, _backtrace2);
    }
    ICY_SUBCASE("from F") {
        const auto _count = _g.dfs("F", [&_preorder](const key_type& _k, const vertex_type& _v) {
            _preorder.push_back(_k);
        }, [&_postorder](const key_type& _k, const vertex_type& _v) {
            _postorder.push_back(_k);
        }, [&_backtrace](const key_type& _k, const vertex_type& _v, const key_type& _u) {
            _backtrace.push_back(_k);
        });
        dfs("F");
        EXPECT_EQ(_preorder, _preorder2);
        EXPECT_EQ(_postorder, _postorder2);
        EXPECT_EQ(_backtrace, _backtrace2);
    }
}
ICY_CASE("multigraph preorder and postorder") {
    auto _g = multigraph_instance<0>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
    std::vector<key_type> _preorder, _preorder2;
    std::vector<key_type> _postorder, _postorder2;
    std::vector<key_type> _backtrace, _backtrace2;
    std::unordered_set<key_type> _s;
    std::function<void(const key_type&)> dfs =
    [&dfs, &_g, &_preorder2, &_postorder2, &_backtrace2, &_s] (const key_type& _k) -> void {
        if (_s.contains(_k)) { return; }
        const vertex_type* const _v = _g.get_vertex(_k);
        _preorder2.push_back(_k);
        _s.insert(_k);
        const auto _outs = _v->out();
        std::vector<key_type> _vec; _vec.reserve(_v->outdegree());
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (_s.contains(_out->first)) { continue; }
            _vec.push_back(_out->first);
        }
        for (auto _i = _vec.crbegin(); _i != _vec.crend(); ++_i) {
            if (!_s.contains(*_i)) {
                dfs(*_i);
                _backtrace2.push_back(_k);
            }
        }
        _postorder2.push_back(_k);
    };
    ICY_SUBCASE("from B") {
        const auto _count = _g.dfs("B", [&_preorder](const key_type& _k, const vertex_type& _v) {
            _preorder.push_back(_k);
        }, [&_postorder](const key_type& _k, const vertex_type& _v) {
            _postorder.push_back(_k);
        }, [&_backtrace](const key_type& _k, const vertex_type& _v, const key_type& _u) {
            _backtrace.push_back(_k);
        });
        dfs("B");
        EXPECT_EQ(_preorder, _preorder2);
        EXPECT_EQ(_postorder, _postorder2);
        EXPECT_EQ(_backtrace, _backtrace2);
    }
    ICY_SUBCASE("from E") {
        const auto _count = _g.dfs("E", [&_preorder](const key_type& _k, const vertex_type& _v) {
            _preorder.push_back(_k);
        }, [&_postorder](const key_type& _k, const vertex_type& _v) {
            _postorder.push_back(_k);
        }, [&_backtrace](const key_type& _k, const vertex_type& _v, const key_type& _u) {
            _backtrace.push_back(_k);
        });
        dfs("E");
        EXPECT_EQ(_preorder, _preorder2);
        EXPECT_EQ(_postorder, _postorder2);
        EXPECT_EQ(_backtrace, _backtrace2);
    }
    ICY_SUBCASE("from I") {
        const auto _count = _g.dfs("I", [&_preorder](const key_type& _k, const vertex_type& _v) {
            _preorder.push_back(_k);
        }, [&_postorder](const key_type& _k, const vertex_type& _v) {
            _postorder.push_back(_k);
        }, [&_backtrace](const key_type& _k, const vertex_type& _v, const key_type& _u) {
            _backtrace.push_back(_k);
        });
        dfs("I");
        std::cout << test::to_string(_preorder) << std::endl;
        std::cout << test::to_string(_postorder) << std::endl;
        EXPECT_EQ(_preorder, _preorder2);
        EXPECT_EQ(_postorder, _postorder2);
        EXPECT_EQ(_backtrace, _backtrace2);
    }
}