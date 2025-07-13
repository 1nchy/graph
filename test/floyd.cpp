#include "test.hpp"

#include "graph.hpp"
#include "main.hpp"

ICY_CASE("graph") {
    const auto _g = graph_instance<0>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
    const auto _floyd = _g.floyd<unsigned>([](const edge_type& _e) -> unsigned {
        return _e.value();
    });
    std::vector<std::string> _trail;
    ICY_SUBCASE("from A to B") {
        _floyd.trail("A", "B", [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        const std::vector<std::string> _expected_trail = {"A", "C", "D", "B"};
        EXPECT_EQ(_trail, _expected_trail);
        EXPECT_EQ(_floyd.cost("A", "B"), 45);
    }
    ICY_SUBCASE("from F to A") {
        _floyd.trail("F", "A", [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        const std::vector<std::string> _expected_trail = {"F", "D", "B", "C", "A"};
        EXPECT_EQ(_trail, _expected_trail);
        EXPECT_EQ(_floyd.cost("F", "A"), 58);
    }
    ICY_SUBCASE("from B to F") {
        _floyd.trail("B", "F", [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        const std::vector<std::string> _expected_trail = {};
        EXPECT_EQ(_trail, _expected_trail);
        EXPECT_EQ(_floyd.cost("B", "F"), std::numeric_limits<cost_type>::max());
    }
    ICY_SUBCASE("from X to Y") {
        EXPECT_THROW(
            std::out_of_range,
            _floyd.trail("X", "Y", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
        );
        EXPECT_THROW(
            std::out_of_range,
            _floyd.cost("X", "Y")
        );
    }
    ICY_SUBCASE("from A to Z") {
        EXPECT_THROW(
            std::out_of_range,
            _floyd.trail("A", "Z", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
        );
        EXPECT_THROW(
            std::out_of_range,
            _floyd.cost("A", "Z")
        );
    }
    ICY_SUBCASE("from Z to A") {
        EXPECT_THROW(
            std::out_of_range,
            _floyd.trail("Z", "A", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
        );
        EXPECT_THROW(
            std::out_of_range,
            _floyd.cost("Z", "A")
        );
    }
}
ICY_CASE("multigraph") {
    const auto _g = multigraph_instance<0>();
    using vertex_type = typename decltype(_g)::vertex_type;
    using edge_type = typename decltype(_g)::edge_type;
    const auto _floyd = _g.floyd<unsigned>([](const edge_type& _e) -> unsigned {
        return _e.value();
    });
    std::vector<std::string> _trail;
    ICY_SUBCASE("from A to E") {
        _floyd.trail("A", "E", [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {"A", "B", "C", "D", "E"};
        EXPECT_EQ(_trail, _expected_trail);
        EXPECT_EQ(_floyd.cost("A", "E"), 10);
    }
    ICY_SUBCASE("from D to G") {
        _floyd.trail("D", "G", [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {"D", "B", "F", "G"};
        EXPECT_EQ(_trail, _expected_trail);
        EXPECT_EQ(_floyd.cost("D", "G"), 8);
    }
    ICY_SUBCASE("from G to I") {
        _floyd.trail("G", "I", [&_trail](const std::string& _k) -> void {
            _trail.push_back(_k);
        });
        std::vector<std::string> _expected_trail = {};
        EXPECT_EQ(_trail, _expected_trail);
        EXPECT_EQ(_floyd.cost("G", "I"), std::numeric_limits<cost_type>::max());
    }
    ICY_SUBCASE("from X to Y") {
        EXPECT_THROW(
            std::out_of_range,
            _floyd.trail("X", "Y", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
        );
        EXPECT_THROW(
            std::out_of_range,
            _floyd.cost("X", "Y")
        );
    }
    ICY_SUBCASE("from A to Z") {
        EXPECT_THROW(
            std::out_of_range,
            _floyd.trail("A", "Z", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
        );
        EXPECT_THROW(
            std::out_of_range,
            _floyd.cost("A", "Z")
        );
    }
    ICY_SUBCASE("from Z to A") {
        EXPECT_THROW(
            std::out_of_range,
            _floyd.trail("Z", "A", [&_trail](const std::string& _k) -> void {
                _trail.push_back(_k);
            });
        );
        EXPECT_THROW(
            std::out_of_range,
            _floyd.cost("Z", "A")
        );
    }
}