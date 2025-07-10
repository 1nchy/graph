#include "test.hpp"

#include "graph.hpp"
#include "main.hpp"

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
    const auto _g = graph_instance<0>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
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
    const auto _g = multigraph_instance<1>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
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