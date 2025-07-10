#include "test.hpp"

#include "graph.hpp"

template <typename _Cost, typename _Vk> auto floyd_trail(
    const _Vk& _x, const _Vk& _y,
    const icy::graph<_Vk, void, std::pair<_Cost, _Vk>>& _floyd,
    std::function<void(const _Vk&)>&& _trailer
) -> void {
    if (!_floyd.adjacent(_x, _y)) { return; }
    _trailer(_x);
    if (_x == _y) { return; }
    std::function<void(const _Vk&, const _Vk&, std::function<void(const _Vk&)>&&)> _M_floyd_trail =
    [&_M_floyd_trail, &_floyd] (const _Vk& _x, const _Vk& _y, std::function<void(const _Vk&)>&& _trailer) -> void {
        const _Vk& _k = _floyd.get_edge(_x, _y)->value().second;
        if (_x == _k) { _trailer(_y); return; }
        _M_floyd_trail(_x, _k, std::move(_trailer));
        _M_floyd_trail(_k, _y, std::move(_trailer));
    };
    _M_floyd_trail(_x, _y, std::move(_trailer));
}

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
    const auto _floyd = _g.floyd<unsigned>([](const edge_type& _e) -> unsigned {
        return _e.value();
    });
    ICY_SUBCASE("from A to B") {
        std::vector<std::string> _trail;
        floyd_trail<unsigned, std::string>("A", "B", _floyd, [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {"A", "C", "D", "B"};
        EXPECT_EQ(_trail, _expected_trail);
    }
    ICY_SUBCASE("from F to A") {
        std::vector<std::string> _trail;
        floyd_trail<unsigned, std::string>("F", "A", _floyd, [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {"F", "D", "B", "C", "A"};
        EXPECT_EQ(_trail, _expected_trail);
    }
    ICY_SUBCASE("from B to F") {
        std::vector<std::string> _trail;
        floyd_trail<unsigned, std::string>("B", "F", _floyd, [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {};
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
    const auto _floyd = _g.floyd<unsigned>([](const edge_type& _e) -> unsigned {
        return _e.value();
    });
    ICY_SUBCASE("from A to H") {
        std::vector<std::string> _trail;
        floyd_trail<unsigned, std::string>("A", "H", _floyd, [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {"A", "B", "E", "G", "H"};
        EXPECT_EQ(_trail, _expected_trail);
    }
    ICY_SUBCASE("from G to F") {
        std::vector<std::string> _trail;
        floyd_trail<unsigned, std::string>("G", "F", _floyd, [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {"G", "D", "A", "B", "C", "F"};
        EXPECT_EQ(_trail, _expected_trail);
    }
    ICY_SUBCASE("from H to D") {
        std::vector<std::string> _trail;
        floyd_trail<unsigned, std::string>("H", "D", _floyd, [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {};
        EXPECT_EQ(_trail, _expected_trail);
    }
}