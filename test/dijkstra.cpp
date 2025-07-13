#include "test.hpp"

#include "graph.hpp"
#include "main.hpp"

#include <string>
#include <vector>

ICY_CASE("graph") {
    const auto _g = graph_instance<0>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
    ICY_SUBCASE("from A") {
        const auto _from_a = _g.dijkstra<unsigned>("A", [](const edge_type& _e) -> unsigned {
            return _e.value();
        });
        std::vector<std::string> _trail;
        ICY_SUBCASE("to F") {
            _from_a.trail("F", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
            std::vector<std::string> _expected_trail = {};
            EXPECT_EQ(_trail, _expected_trail);
        }
        ICY_SUBCASE("to B") {
            _from_a.trail("B", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
            std::vector<std::string> _expected_trail = {"A", "C", "D", "B"};
            EXPECT_EQ(_trail, _expected_trail);
        }
    }
    ICY_SUBCASE("from F") {
        const auto _from_f = _g.dijkstra<unsigned>("F", [](const edge_type& _e) -> unsigned {
            return _e.value();
        });
        std::vector<std::string> _trail;
        ICY_SUBCASE("to A") {
            _from_f.trail("A", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
            std::vector<std::string> _expected_trail = {"F", "D", "B", "C", "A"};
            EXPECT_EQ(_trail, _expected_trail);
        }
    }
}
ICY_CASE("multigraph") {
    const auto _g = multigraph_instance<1>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
    ICY_SUBCASE("from A") {
        const auto _from_a = _g.dijkstra<unsigned>("A", [](const edge_type& _e) -> unsigned {
            return _e.value();
        });
        std::vector<std::string> _trail;
        ICY_SUBCASE("to H") {
            _from_a.trail("H", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
            std::vector<std::string> _expected_trail = {"A", "B", "E", "G", "H"};
            EXPECT_EQ(_trail, _expected_trail);
        }
    }
    ICY_SUBCASE("from G") {
        const auto _from_g = _g.dijkstra<unsigned>("G", [](const edge_type& _e) -> unsigned {
            return _e.value();
        });
        std::vector<std::string> _trail;
        ICY_SUBCASE("to F") {
            _from_g.trail("F", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
            std::vector<std::string> _expected_trail = {"G", "D", "A", "B", "C", "F"};
            EXPECT_EQ(_trail, _expected_trail);
        }
    }
    ICY_SUBCASE("from H") {
        const auto _from_h = _g.dijkstra<unsigned>("H", [](const edge_type& _e) -> unsigned {
            return _e.value();
        });
        std::vector<std::string> _trail;
        ICY_SUBCASE("to F") {
            _from_h.trail("F", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
            std::vector<std::string> _expected_trail = {};
            EXPECT_EQ(_trail, _expected_trail);
        }
    }
}