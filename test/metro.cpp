/// test undirected_graph and undirected_multigraph

#include "test.hpp"
#include "graph.hpp"

enum land_type {
    RESIDENTIAL, // 居住用地
    PUBLIC, // 公共服务用地
    COMMERCIAL, // 商业用地
    INDUSTRIAL, // 工业用地
    TRANSPORTATION, // 交通用地
    SCHOOL, // 学校
    GREEN, // 绿地
    NONE,
};

struct road {
public:
    enum vehicle { WALK, METRO, CAR };
public:
    road(enum vehicle, unsigned);
    road(const road&) = default;
    virtual ~road() = default;
public:
    auto operator==(const road&) const -> bool = default;
    auto operator!=(const road&) const -> bool = default;
public:
    auto vehicle() const { return _vehicle; }
    auto time() const { return _time; }
    auto set_time(unsigned _time) { this->_time = _time; }
private:
    const enum vehicle _vehicle;
    unsigned _time;
};

road::road(enum vehicle _vehicle, unsigned _time) : _vehicle(_vehicle), _time(_time) {}

using undirected_graph = icy::undirected_graph<std::string, land_type, std::pair<unsigned, unsigned>>;
using undirected_multigraph = icy::undirected_multigraph<std::string, land_type, road>;

using key_type = std::string;

auto metro() -> undirected_graph {
    undirected_graph _metro;
    /// line 1
    _metro.insert("Yinbin Road", RESIDENTIAL);
    _metro.insert("Tiexi Square", PUBLIC);
    _metro.insert("Shenyang Railway Station", TRANSPORTATION);
    _metro.insert("Taiyuan Street", COMMERCIAL);
    _metro.insert("Teenager Street", COMMERCIAL);
    _metro.insert("Midst Street", COMMERCIAL);
    _metro.insert("Pangjiang Street", COMMERCIAL);
    _metro.insert("Dawn Square", PUBLIC);
    _metro.connect("Yinbin Road", "Tiexi Square", 1, 9);
    _metro.connect("Tiexi Square", "Shenyang Railway Station", 1, 5);
    _metro.connect("Shenyang Railway Station", "Taiyuan Street", 1, 3);
    _metro.connect("Taiyuan Street", "Teenager Street", 1, 4);
    _metro.connect("Teenager Street", "Midst Street", 1, 5);
    _metro.connect("Midst Street", "Pangjiang Street", 1, 5);
    _metro.connect("Pangjiang Street", "Dawn Square", 1, 2);
    /// line 2
    _metro.insert("Liaoning University", SCHOOL);
    _metro.insert("TCM University", SCHOOL); // Traditional Chinese Medicine
    _metro.insert("Shenyangbei Railway Station", TRANSPORTATION);
    _metro.insert("People's Square", PUBLIC);
    _metro.insert("Teenager Street", COMMERCIAL);
    _metro.insert("Industry Museum", INDUSTRIAL);
    _metro.insert("City Library", PUBLIC);
    _metro.insert("Olympic Center", PUBLIC);
    _metro.insert("Province Museum", PUBLIC);
    _metro.insert("Central Park", GREEN);
    _metro.insert("Nova 1st Road", RESIDENTIAL);
    _metro.insert("Taoxian Airport", TRANSPORTATION);
    _metro.connect("Liaoning University", "TCM University", 2, 22);
    _metro.connect("TCM University", "Shenyangbei Railway Station", 2, 4);
    _metro.connect("Shenyangbei Railway Station", "People's Square", 2, 4);
    _metro.connect("People's Square", "Teenager Street", 2, 3);
    _metro.connect("Teenager Street", "Industry Museum", 2, 4);
    _metro.connect("Industry Museum", "City Library", 2, 3);
    _metro.connect("City Library", "Olympic Center", 2, 4);
    _metro.connect("Olympic Center", "Province Museum", 2, 7);
    _metro.connect("Province Museum", "Central Park", 2, 3);
    _metro.connect("Central Park", "Nova 1st Road", 2, 3);
    _metro.connect("Nova 1st Road", "Taoxian Airport", 2, 7);
    /// line 3
    _metro.insert("Industry University", SCHOOL);
    _metro.insert("Datonghu Street", COMMERCIAL);
    _metro.insert("Shayang", RESIDENTIAL);
    _metro.insert("Industry Museum", INDUSTRIAL);
    _metro.insert("Jiangdong Street", COMMERCIAL);
    _metro.connect("Industry University", "Datonghu Street", 3, 16);
    /// line 4
    _metro.insert("Zhengxin Road", RESIDENTIAL);
    _metro.insert("Hezuo Street", COMMERCIAL);
    _metro.insert("Shenyang University", SCHOOL);
    _metro.insert("Shenyangbei Railway Station", TRANSPORTATION);
    _metro.insert("Taiyuan Street", COMMERCIAL);
    _metro.insert("Shayang", RESIDENTIAL);
    _metro.insert("Changbainan", RESIDENTIAL);
    _metro.insert("Shenyangnan Railway Station", TRANSPORTATION);
    _metro.connect("Zhengxin Road", "Hezuo Street", 4, 12);
    _metro.connect("Hezuo Street", "Shenyang University", 4, 4);
    _metro.connect("Shenyang University", "Shenyangbei Railway Station", 4, 4);
    _metro.connect("Shenyangbei Railway Station", "Taiyuan Street", 4, 8);
    _metro.connect("Taiyuan Street", "Shayang", 4, 5);
    _metro.connect("Shayang", "Changbainan", 4, 8);
    _metro.connect("Changbainan", "Shenyangnan Railway Station", 4, 13);
    /// line 6
    _metro.insert("Hero Park", GREEN); // island
    _metro.insert("Beita", RESIDENTIAL);
    _metro.insert("Shenyang University", SCHOOL);
    _metro.insert("Midst Street", COMMERCIAL);
    _metro.insert("City Library", PUBLIC);
    /// line 9
    _metro.insert("Nujiang Park", GREEN);
    _metro.insert("Shenyang 2nd Hospital", PUBLIC);
    _metro.insert("Tiexi Square", PUBLIC);
    _metro.insert("Xinghua Park", GREEN);
    _metro.insert("Datonghu Street", COMMERCIAL);
    _metro.insert("Changbainan", RESIDENTIAL);
    _metro.insert("Olympic Center", PUBLIC);
    _metro.insert("Changqingnan Street", COMMERCIAL);
    _metro.insert("Building University", SCHOOL);
    _metro.connect("Nujiang Park", "Shenyang 2nd Hospital", 9, 2);
    _metro.connect("Shenyang 2nd Hospital", "Tiexi Square", 9, 9);
    _metro.connect("Tiexi Square", "Xinghua Park", 9, 2);
    _metro.connect("Xinghua Park", "Datonghu Street", 9, 10);
    _metro.connect("Datonghu Street", "Changbainan", 9, 11);
    _metro.connect("Changbainan", "Olympic Center", 9, 9);
    _metro.connect("Olympic Center", "Changqingnan Street", 9, 8);
    _metro.connect("Changqingnan Street", "Building University", 9, 2);
    /// line 10
    _metro.insert("Dingxiang Lake", GREEN);
    _metro.insert("Shenyang 2nd Hospital", PUBLIC);
    _metro.insert("TCM University", SCHOOL);
    _metro.insert("Beita", RESIDENTIAL);
    _metro.insert("Hezuo Street", COMMERCIAL);
    _metro.insert("Pangjiang Street", COMMERCIAL);
    _metro.insert("Chang'an Road", RESIDENTIAL);
    _metro.insert("Jiangdong Street", COMMERCIAL);
    _metro.insert("Changqingnan Street", COMMERCIAL);
    _metro.insert("Institute of Technology", SCHOOL);
    _metro.insert("Nova 1st Road", RESIDENTIAL);
    _metro.insert("Shenyangnan Railway Station", TRANSPORTATION);
    _metro.connect("Dingxiang Lake", "Shenyang 2nd Hospital", 10, 10);
    _metro.connect("Shenyang 2nd Hospital", "TCM University", 10, 6);
    _metro.connect("TCM University", "Beita", 10, 5);
    _metro.connect("Beita", "Hezuo Street", 10, 3);
    _metro.connect("Hezuo Street", "Pangjiang Street", 10, 5);
    _metro.connect("Pangjiang Street", "Chang'an Road", 10, 3);
    _metro.connect("Chang'an Road", "Jiangdong Street", 10, 7);
    _metro.connect("Jiangdong Street", "Changqingnan Street", 10, 3);
    _metro.connect("Changqingnan Street", "Institute of Technology", 10, 3);
    return _metro;
}

auto comprehensive() -> undirected_multigraph {
    undirected_multigraph _comprehensive;
    const auto _metro = metro();
    auto metro_suffix = [](const std::string& _s, unsigned _line) -> const std::string {
        return "METRO/" + std::to_string(_line) + "/" + _s;
    };
    for (const auto& [_k, _v] : _metro.vertices()) {
        std::unordered_set<unsigned> _lines;
        const auto _ins = _v->in();
        for (auto _i = _ins.first; _i != _ins.second; ++_i) {
            _lines.insert(_i->second->value().first);
        }
        const auto _outs = _v->out();
        for (auto _o = _outs.first; _o != _outs.second; ++_o) {
            _lines.insert(_o->second->value().first);
        }
        const std::vector<unsigned> _s(_lines.cbegin(), _lines.cend());
        _comprehensive.insert(_k, _v->value());
        for (const auto _l : _s) {
            _comprehensive.insert(metro_suffix(_k, _l), NONE);
        }
        for (size_t _i = 0; _i != _s.size(); ++_i) {
            const std::string _u = metro_suffix(_k, _s[_i]); // metro_suffix(_k, _s[_i])
            _comprehensive.connect(_u, _k, road::WALK, 3);
            for (size_t _j = _i + 1; _j != _s.size(); ++_j) {
                _comprehensive.connect(_u, metro_suffix(_k, _s[_j]), road::WALK, 4);
            }
        }
    }
    EXPECT_EQ(_comprehensive.size(), 61);
    for (const auto& _e : _metro.edges()) {
        const auto& _uk = _e->either_key();
        const auto& _vk = _e->other_key(_uk);
        const auto _l = _e->value().first;
        _comprehensive.connect(metro_suffix(_uk, _l), metro_suffix(_vk, _l), road::METRO, _e->value().second);
        _comprehensive.connect(_uk, _vk, road::WALK, _e->value().second * 3);
        _comprehensive.connect(_uk, _vk, road::CAR, _e->value().second * 3 / 4);
    }
    return _comprehensive;
}

ICY_CASE("undirected_graph constructor / assignment") {
    using vertex_type = typename undirected_graph::vertex_type;
    using edge_type = typename undirected_graph::edge_type;
    using vertex_value_type = typename vertex_type::value_type;
    using edge_value_type = typename edge_type::value_type;
    undirected_graph _metro = metro();
    EXPECT_EQ(_metro.order(), 38);
    EXPECT_EQ(_metro.size(), 43);
    ICY_SUBCASE("undirected_graph") {
        icy::undirected_graph<key_type, vertex_value_type, edge_value_type> _g(_metro);
        EXPECT_EQ(_g.order(), 38);
        EXPECT_EQ(_g.size(), 43);
        EXPECT_EQ(_g, _metro);
        _g = _metro;
        EXPECT_EQ(_g.order(), 38);
        EXPECT_EQ(_g.size(), 43);
        EXPECT_EQ(_g, _metro);
    }
    ICY_SUBCASE("graph") {
        icy::graph<key_type, vertex_value_type, edge_value_type> _g(_metro);
        EXPECT_EQ(_g.order(), 38);
        EXPECT_EQ(_g.size(), 86);
    }
    ICY_SUBCASE("undirected_multigraph") {
        icy::undirected_multigraph<key_type, vertex_value_type, edge_value_type> _g(_metro);
        EXPECT_EQ(_g.order(), 38);
        EXPECT_EQ(_g.size(), 43);
    }
    ICY_SUBCASE("multigraph") {
        icy::multigraph<key_type, vertex_value_type, edge_value_type> _g(_metro);
        EXPECT_EQ(_g.order(), 38);
        EXPECT_EQ(_g.size(), 86);
    }
}
ICY_CASE("undirected_graph") {
    using vertex_type = typename undirected_graph::vertex_type;
    using edge_type = typename undirected_graph::edge_type;
    using cost_type = unsigned;
    undirected_graph _metro = metro();
    EXPECT_EQ(_metro.order(), 38);
    EXPECT_EQ(_metro.size(), 43);
    ICY_SUBCASE("epidemic") {
        // epidemic in Taiyuan Street
        const key_type _source = "Taiyuan Street";
        const cost_type _release = 8;
        const auto _dijk = _metro.dijkstra<cost_type>(_source, [](const edge_type& _e) -> cost_type {
            return _e.value().second;
        });
        std::vector<key_type> _lockdown;
        std::vector<key_type> _nostop;
        _metro.bfs(_source, [&_lockdown, &_nostop, &_dijk, _release](const key_type& _k, const vertex_type& _v) -> void {
            if (_dijk.cost(_k) < _release) { _lockdown.emplace_back(_k); }
            else if (_dijk.cost(_k) < _release * 2) { _nostop.emplace_back(_k); }
        });
        EXPECT_EQ(_lockdown.size(), 5);
        EXPECT_EQ(_nostop.size(), 9);
        for (const auto& _k : _lockdown) {
            _metro.disconnect(_k);
        }
        EXPECT_EQ(_metro.size(), 33);
        for (const auto& _k : _nostop) {
            vertex_type* const _v = _metro.get_vertex(_k);
            EXPECT_EQ(_v->indegree(), _v->outdegree());
            std::set<unsigned> _lines;
            std::unordered_multimap<unsigned, key_type> _adjacency; _adjacency.reserve(_v->indegree());
            const auto _outs = _v->out();
            for (auto _o = _outs.first; _o != _outs.second; ++_o) {
                _lines.insert(_o->second->value().first);
                _adjacency.emplace(_o->second->value().first, _o->first);
            }
            for (const auto& _line : _lines) {
                EXPECT_LE(_adjacency.count(_line), 2);
                if (_adjacency.count(_line) == 2) {
                    auto _iters = _adjacency.equal_range(_line);
                    const key_type& _x = (_iters.first++)->second;
                    const key_type& _y = (_iters.first++)->second;
                    EXPECT_EQ(_iters.first, _iters.second);
                    const edge_type* const _xe = _metro.get_edge(_k, _x);
                    const edge_type* const _ye = _metro.get_edge(_k, _y);
                    EXPECT_NE(_xe, nullptr);
                    EXPECT_NE(_ye, nullptr);
                    _metro.connect(_x, _y, _xe->value().first, _xe->value().second + _ye->value().second - 1);
                    _metro.disconnect(_k, _x);
                    _metro.disconnect(_k, _y);
                }
                else {
                    auto _iters = _adjacency.equal_range(_line);
                    const key_type& _x = (_iters.first++)->second;
                    EXPECT_EQ(_iters.first, _iters.second);
                    _metro.disconnect(_k, _x);
                }
            }
        }
        EXPECT_EQ(_metro.size(), 19);
    }
    ICY_SUBCASE("from RESIDENTIAL to GREEN") {
        const auto _floyd = _metro.floyd<cost_type>([](const edge_type& _e) -> cost_type {
            return _e.value().second;
        });
        std::vector<key_type> _green;
        for (const auto& [_k, _v] : _metro.vertices()) {
            if (_v->value() == GREEN) {
                _green.emplace_back(_k);
            }
        }
        std::unordered_map<key_type, cost_type> _residential;
        for (const auto& [_k, _v] : _metro.vertices()) {
            if (_v->value() == RESIDENTIAL) {
                _residential[_k] = std::numeric_limits<cost_type>::max();
                for (const auto& _g : _green) {
                    _residential[_k] = std::min(_residential[_k], _floyd.cost(_k, _g));
                }
            }
        }
        EXPECT_EQ(_residential.size(), 7);
        const auto _minmax = std::minmax_element(_residential.cbegin(), _residential.cend(), [](const auto& _x, const auto& _y) -> bool {
            return _x.second < _y.second;
        });
        EXPECT_EQ(_minmax.first->first, "Nova 1st Road");
        EXPECT_EQ(_minmax.second->first, "Zhengxin Road");
        // EXPECT_EQ(test::to_string(_residential), "");
    }
    ICY_SUBCASE("high load") {
        const auto _tarjan = _metro.tarjan();
        EXPECT_EQ(_tarjan.vertices().size(), 12);
        EXPECT_EQ(_tarjan.edges().size(), 14);
    }
}
ICY_CASE("undirected_multigraph constructor / assignment") {
    using vertex_type = typename undirected_multigraph::vertex_type;
    using edge_type = typename undirected_multigraph::edge_type;
    using vertex_value_type = typename vertex_type::value_type;
    using edge_value_type = typename edge_type::value_type;
    undirected_multigraph _comprehensive = comprehensive();
    EXPECT_EQ(_comprehensive.order(), 87);
    EXPECT_EQ(_comprehensive.size(), 190);
    ICY_SUBCASE("undirected_multigraph") {
        icy::undirected_multigraph<key_type, vertex_value_type, edge_value_type> _g(_comprehensive);
        EXPECT_EQ(_g.order(), 87);
        EXPECT_EQ(_g.size(), 190);
        _g = _comprehensive;
        EXPECT_EQ(_g.order(), 87);
        EXPECT_EQ(_g.size(), 190);
        EXPECT_EQ(_g, _comprehensive);
    }
    ICY_SUBCASE("multigraph") {
        icy::multigraph<key_type, vertex_value_type, edge_value_type> _g(_comprehensive);
        EXPECT_EQ(_g.order(), 87);
        EXPECT_EQ(_g.size(), 380);
    }
}
ICY_CASE("undirected_multigraph") {
    using vertex_type = typename undirected_multigraph::vertex_type;
    using edge_type = typename undirected_multigraph::edge_type;
    using cost_type = unsigned;
    undirected_multigraph _comprehensive = comprehensive();
    EXPECT_EQ(_comprehensive.order(), 87);
    EXPECT_EQ(_comprehensive.size(), 190);
    std::unordered_set<key_type> _transportations;
    std::unordered_set<key_type> _residentials;
    _comprehensive.bfs("Shenyang Railway Station",
        [&_transportations, &_residentials](const key_type& _k, const vertex_type& _v) -> void {
        if (_v.value() == TRANSPORTATION) {
            _transportations.insert(_k);
        }
        if (_v.value() == RESIDENTIAL) {
            _residentials.insert(_k);
        }
    });
    EXPECT_EQ(_transportations.size(), 4);
    EXPECT_EQ(_residentials.size(), 7);
    ICY_SUBCASE("nearest and farthest railway station") {
        const auto _floyd = _comprehensive.floyd<cost_type>([](const edge_type& _e) -> cost_type {
            return _e.value().time();
        });
        std::unordered_map<key_type, cost_type> _statistics;
        for (const auto& _t : _transportations) {
            for (const auto& _r : _residentials) {
                _statistics[_t] += _floyd.cost(_t, _r);
            }
        }
        const auto _minmax = std::minmax_element(_statistics.cbegin(), _statistics.cend(), [](const auto& _x, const auto& _y) -> bool {
            return _x.second < _y.second;
        });
        EXPECT_EQ(_minmax.first->first, "Shenyangbei Railway Station");
        EXPECT_EQ(_minmax.second->first, "Taoxian Airport");
        const std::string _from = "Yinbin Road";
        ICY_SEQCASE("to Shenyangbei Railway Station") {
            const std::string _to = "Shenyangbei Railway Station";
            std::vector<key_type> _trail;
            _floyd.trail(_from, _to, [&_trail](const key_type& _k) {
                _trail.emplace_back(_k);
            });
            EXPECT_EQ(_trail.size(), 5);
            EXPECT_EQ(_floyd.cost(_from, _to), 17);
        }
        ICY_SEQCASE("to Shenyangnan Railway Station") {
            const std::string _to = "Shenyangnan Railway Station";
            std::vector<key_type> _trail;
            _floyd.trail(_from, _to, [&_trail](const key_type& _k) {
                _trail.emplace_back(_k);
            });
            EXPECT_EQ(_trail.size(), 7);
            EXPECT_EQ(_floyd.cost(_from, _to), 29);
        }
        ICY_SEQCASE("Taoxian Airport") {
            const std::string _to = "Taoxian Airport";
            std::vector<key_type> _trail;
            _floyd.trail(_from, _to, [&_trail](const key_type& _k) {
                _trail.emplace_back(_k);
            });
            EXPECT_EQ(_trail.size(), 12);
            EXPECT_EQ(_floyd.cost(_from, _to), 36);
        }
    }
    ICY_SUBCASE("evening peak") {
        const std::unordered_set<key_type> _block = {
            "Taiyuan Street"
        };
        const auto _range = 8;
        std::unordered_set<edge_type*> _mild, _severe;
        for (const auto& _k : _block) {
            const auto _dijk = _comprehensive.dijkstra<cost_type>(_k, [](const edge_type& _e) -> cost_type {
                return _e.value().time();
            });
            _comprehensive.bfs(_k, [&_dijk, &_mild, &_severe](const key_type& _k, vertex_type& _v) -> void {
                auto _outs = _v.out();
                for (auto _o = _outs.first; _o != _outs.second; ++_o) {
                    if (_o->second->value().vehicle() == road::CAR) {
                        if (_dijk.cost(_o->first) <= _range) {
                            _severe.insert(_o->second);
                        }
                        else if (_dijk.cost(_o->first) <= _range * 2) {
                            _mild.insert(_o->second);
                        }
                    }
                }
            });
        }
        for (auto* _e : _mild) {
            const auto _t = _e->value().time();
            if (_severe.contains(_e)) { continue; }
            _e->value().set_time(_t * 3);
        }
        for (auto* _e : _severe) {
            const auto _t = _e->value().time();
            _e->value().set_time(_t * 6);
        }
        EXPECT_NE(_comprehensive, comprehensive());
        const auto _floyd = _comprehensive.floyd<cost_type>([](const edge_type& _e) -> cost_type {
            return _e.value().time();
        });
        const std::string _from = "Yinbin Road";
        ICY_SEQCASE("to Shenyangbei Railway Station") {
            const std::string _to = "Shenyangbei Railway Station";
            std::vector<key_type> _trail;
            _floyd.trail(_from, _to, [&_trail](const key_type& _k) {
                _trail.emplace_back(_k);
            });
            EXPECT_EQ(_trail.size(), 8);
            EXPECT_EQ(_floyd.cost(_from, _to), 35);
        }
        ICY_SEQCASE("to Shenyangnan Railway Station") {
            const std::string _to = "Shenyangnan Railway Station";
            std::vector<key_type> _trail;
            _floyd.trail(_from, _to, [&_trail](const key_type& _k) {
                _trail.emplace_back(_k);
            });
            EXPECT_EQ(_trail.size(), 10);
            EXPECT_EQ(_floyd.cost(_from, _to), 53);
        }
        ICY_SEQCASE("Taoxian Airport") {
            const std::string _to = "Taoxian Airport";
            std::vector<key_type> _trail;
            _floyd.trail(_from, _to, [&_trail](const key_type& _k) {
                _trail.emplace_back(_k);
            });
            EXPECT_EQ(_trail.size(), 15);
            EXPECT_EQ(_floyd.cost(_from, _to), 59);
        }
    }
}