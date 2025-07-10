#include "test.hpp"

#include "graph.hpp"

#include <string>
#include <vector>

#include <iostream>

template <typename _Cost, typename _Vk> auto dijkstra_trail(
    const _Vk& _x, const _Vk& _y,
    const std::unordered_map<_Vk, std::pair<_Cost, _Vk>>& _dijk,
    std::function<void(const _Vk&)>&& _trailer
) -> void {
    if (_x == _y) {
        _trailer(_x);
        return;
    }
    if (!_dijk.contains(_y)) { return; }
    dijkstra_trail(_x, _dijk.at(_y).second, _dijk, std::move(_trailer));
    _trailer(_y);
}

ICY_CASE("graph") {
    icy::graph<std::string, void, unsigned> _g;
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
    using key_type = typename decltype(_g)::key_type;
    using cost_type = unsigned;
    using vertex_visitor = typename decltype(_g)::vertex_visitor<cost_type>;
    using edge_visitor = typename decltype(_g)::edge_visitor<cost_type>;
    _g.insert("A");
    _g.insert("B");
    _g.insert("C");
    _g.insert("D");
    _g.insert("E");
    _g.insert("F");
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
        const auto _from_a = _g.dijkstra<unsigned>("A", [](const edge_type& _e) -> unsigned {
            return _e.value();
        });
        EXPECT_FALSE(_from_a.contains("F"));
        std::vector<std::string> _trail;
        dijkstra_trail<unsigned, std::string>("A", "B", _from_a, [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {"A", "C", "D", "B"};
        EXPECT_EQ(_trail, _expected_trail);
    }
    ICY_SUBCASE("from F") {
        const auto _from_f = _g.dijkstra<unsigned>("F", [](const edge_type& _e) -> unsigned {
            return _e.value();
        });
        EXPECT_EQ(_from_f.size(), 5);
        std::vector<std::string> _trail;
        dijkstra_trail<unsigned, std::string>("F", "A", _from_f, [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {"F", "D", "B", "C", "A"};
        EXPECT_EQ(_trail, _expected_trail);
    }
}
ICY_CASE("multigraph") {
    icy::multigraph<std::string, char, unsigned> _g;
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
    _g.insert("G", 'G');
    _g.insert("H", 'h');
    _g.connect("A", "B", 3);
    _g.connect("B", "C", 15);
    _g.connect("B", "E", 12);
    _g.connect("C", "F", 18);
    _g.connect("D", "A", 5);
    _g.connect("D", "E", 18);
    _g.connect("E", "D", 10);
    _g.connect("E", "F", 25);
    _g.connect("E", "G", 9);
    _g.connect("E", "G", 14);
    _g.connect("F", "F", 3);
    _g.connect("F", "G", 8);
    _g.connect("G", "D", 11);
    _g.connect("G", "H", 6);
    ICY_SUBCASE("from A") {
        const auto _from_a = _g.dijkstra<unsigned>("A", [](const edge_type& _e) -> unsigned {
            return _e.value();
        });
        EXPECT_EQ(_from_a.size(), 7);
        std::vector<std::string> _trail;
        dijkstra_trail<unsigned, std::string>("A", "H", _from_a, [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {"A", "B", "E", "G", "H"};
        EXPECT_EQ(_trail, _expected_trail);
    }
    ICY_SUBCASE("from G") {
        const auto _from_g = _g.dijkstra<unsigned>("G", [](const edge_type& _e) -> unsigned {
            return _e.value();
        });
        EXPECT_EQ(_from_g.size(), 7);
        std::vector<std::string> _trail;
        dijkstra_trail<unsigned, std::string>("G", "F", _from_g, [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {"G", "D", "A", "B", "C", "F"};
        EXPECT_EQ(_trail, _expected_trail);
    }
    ICY_SUBCASE("from H") {
        const auto _from_h = _g.dijkstra<unsigned>("H", [](const edge_type& _e) -> unsigned {
            return _e.value();
        });
        EXPECT_EQ(_from_h.size(), 0);
    }
}