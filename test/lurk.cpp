/// test multigraph

#include "test.hpp"
#include "graph.hpp"

#include <array>

enum faction : unsigned char {
    CPC, KMT, THIRD,
    __SIZE__
};

struct identity {
public:
    identity(faction);
    identity(const identity&) = default;
    identity(identity&&) = default;
    auto operator=(const identity&) -> identity& = default;
    auto operator=(identity&&) -> identity& = default;
    virtual ~identity() = default;
public:
    auto operator==(const identity&) const -> bool = default;
    auto operator!=(const identity&) const -> bool = default;
public:
    auto view(faction) const -> faction;
    auto set_view(faction) -> void;
    auto set_view(faction, faction) -> void;
private:
    std::array<faction, __SIZE__> _view;
};

identity::identity(faction _f) {
    _view.fill(_f);
}
auto identity::view(faction _f = THIRD) const -> faction {
    return _view.at(_f);
}
auto identity::set_view(faction _f) -> void {
    _view.fill(_f);
}
auto identity::set_view(faction _f, faction _site) -> void {
    _view.at(_site) = _f;
}

// `x --"r"--> y` equals `x r y`
enum verb {
    MARRY,
    LOVE,
    RAISE,
    LEAD,
    FOLLOW,
    KILL,
    TEACH, // give a lesson to someone
    INVESTIGATE,
    POST, // bring information to someone
    ARREST,
    BRIBE,
    LOST,
};

using multigraph = icy::multigraph<std::string, identity, verb>;

using key_type = typename multigraph::key_type;
using vertex_type = typename multigraph::vertex_type;
using edge_type = typename multigraph::edge_type;
using cost_type = unsigned;

template <size_t> auto episode() -> multigraph;

template <> auto episode<0>() -> multigraph {
    multigraph _drama;
    _drama.insert("Yu Zecheng", KMT);
    _drama.insert("Zuo Lan", CPC); //
    _drama.insert("Lv Zongfang", CPC); //
    _drama.insert("Dai Li", KMT);
    _drama.insert("Zheng Jiemin", KMT);
    _drama.insert("Li Haifeng", THIRD);
    _drama.insert("Li Kenong", CPC);
    _drama.insert("Deng Ming", CPC);
    _drama.insert("Wu Jinzhong", KMT);
    _drama.insert("Lu Qiaoshan", KMT);
    _drama.insert("Ma Kui", KMT);
    _drama.insert("Mu Wanqiu", THIRD);
    _drama.insert("Mu Liancheng", THIRD);
    _drama.insert("Mr. Qiu", CPC); //
    _drama.insert("Chen Cuiping", CPC); //
    _drama.insert("Li Ya", KMT); //
    ///
    _drama.get_vertex("Zuo Lan")->value().set_view(THIRD, KMT);
    _drama.get_vertex("Lv Zongfang")->value().set_view(KMT, KMT);
    _drama.get_vertex("Mr. Qiu")->value().set_view(THIRD, KMT);
    _drama.get_vertex("Chen Cuiping")->value().set_view(THIRD, KMT);
    _drama.get_vertex("Li Ya")->value().set_view(CPC, CPC);
    ///
    _drama.biconnect("Yu Zecheng", "Zuo Lan", LOVE);
    _drama.connect("Yu Zecheng", "Lv Zongfang", FOLLOW);
    _drama.connect("Wu Jinzhong", "Yu Zecheng", TEACH);
    _drama.connect("Lv Zongfang", "Yu Zecheng", TEACH);
    _drama.connect("Lv Zongfang", "Yu Zecheng", LEAD);
    _drama.connect("Lv Zongfang", "Dai Li", FOLLOW);
    _drama.connect("Dai Li", "Lv Zongfang", LEAD);
    _drama.connect("Deng Ming", "Li Kenong", FOLLOW);
    _drama.connect("Li Kenong", "Deng Ming", LEAD);
    _drama.connect("Mr. Qiu", "Deng Ming", FOLLOW);
    _drama.connect("Deng Ming", "Mr. Qiu", LEAD);
    _drama.connect("Zuo Lan", "Deng Ming", FOLLOW);
    _drama.connect("Deng Ming", "Zuo Lan", LEAD);
    _drama.connect("Lu Qiaoshan", "Zheng Jiemin", FOLLOW);
    _drama.connect("Zheng Jiemin", "Lu Qiaoshan", LEAD);
    _drama.connect("Mu Liancheng", "Mu Wanqiu", RAISE);
    _drama.connect("Li Ya", "Wu Jinzhong", FOLLOW);
    _drama.connect("Wu Jinzhong", "Li Ya", LEAD);
    _drama.connect("Li Ya", "Wu Jinzhong", POST);
    return _drama;
}
template <> auto episode<1>() -> multigraph { // [ kill Li Haifeng ]
    multigraph _drama(episode<0>());
    _drama.get_vertex("Zuo Lan")->value().set_view(CPC);
    _drama.connect("Yu Zecheng", "Li Haifeng", FOLLOW);
    _drama.connect("Li Haifeng", "Yu Zecheng", LEAD);
    _drama.connect("Ma Kui", "Lv Zongfang", KILL);
    _drama.connect("Yu Zecheng", "Li Haifeng", KILL);
    _drama.get_vertex("Yu Zecheng")->value().set_view(CPC, CPC);
    _drama.get_vertex("Yu Zecheng")->value().set_view(CPC, THIRD);
    _drama.connect("Li Kenong", "Yu Zecheng", TEACH);
    return _drama;
}
template <> auto episode<2>() -> multigraph { // [ Fake Wife ]
    multigraph _drama(episode<1>());
    _drama.connect("Wu Jinzhong", "Dai Li", FOLLOW);
    _drama.connect("Dai Li", "Wu Jinzhong", LEAD);
    _drama.connect("Yu Zecheng", "Wu Jinzhong", FOLLOW);
    _drama.connect("Wu Jinzhong", "Yu Zecheng", LEAD);
    _drama.connect("Ma Kui", "Wu Jinzhong", FOLLOW);
    _drama.connect("Wu Jinzhong", "Ma Kui", LEAD);
    _drama.connect("Lu Qiaoshan", "Wu Jinzhong", FOLLOW);
    _drama.connect("Wu Jinzhong", "Lu Qiaoshan", LEAD);
    _drama.connect("Yu Zecheng", "Wu Jinzhong", BRIBE);
    _drama.connect("Yu Zecheng", "Mr. Qiu", POST);
    _drama.biconnect("Yu Zecheng", "Chen Cuiping", MARRY);
    _drama.connect("Mu Wanqiu", "Yu Zecheng", LOVE);
    _drama.connect("Yu Zecheng", "Mu Liancheng", INVESTIGATE);
    _drama.connect("Mu Liancheng", "Wu Jinzhong", BRIBE);
    _drama.connect("Ma Kui", "Yu Zecheng", INVESTIGATE);
    _drama.connect("Li Ya", "Zuo Lan", INVESTIGATE);
    return _drama;
}
template <> auto episode<3>() -> multigraph { // [ Military Mediation ]
    multigraph _drama(episode<2>());
    _drama.connect("Ma Kui", "Mu Liancheng", INVESTIGATE);
    _drama.connect("Ma Kui", "Wu Jinzhong", INVESTIGATE);
    _drama.biconnect("Yu Zecheng", "Zuo Lan", POST);
    _drama.connect("Yu Zecheng", "Chen Cuiping", TEACH);
    return _drama;
}
template <> auto episode<4>() -> multigraph { // [ Death of Dai Li ]
    multigraph _drama(episode<3>());
    _drama.connect("Chen Cuiping", "Yu Zecheng", LOVE);
    _drama.connect("Dai Li", "Dai Li", LOST);
    _drama.connect("Ma Kui", "Mr. Qiu", INVESTIGATE);
    _drama.connect("Ma Kui", "Mr. Qiu", ARREST);
    _drama.get_vertex("Mr. Qiu")->value().set_view(CPC);
    return _drama;
}
template <> auto episode<5>() -> multigraph { // [ Eliminate Ma Kui ]
    multigraph _drama(episode<4>());
    _drama.connect("Lu Qiaoshan", "Ma Kui", ARREST);
    _drama.connect("Deng Ming", "Li Ya", ARREST);
    _drama.get_vertex("Ma Kui")->value().set_view(CPC, KMT);
    _drama.get_vertex("Li Ya")->value().set_view(KMT);
    return _drama;
}
template <> auto episode<6>() -> multigraph { // [ Death of Zuo Lan ]
    multigraph _drama(episode<5>());
    _drama.connect("Ma Kui", "Ma Kui", LOST);
    _drama.biconnect("Ma Kui", "Zuo Lan", KILL);
    return _drama;
}

ICY_CASE("POST") { // send message to Li Kenong
    auto _drama = episode<4>();
    std::unordered_set<key_type> _s;
    const auto _outs = _drama.get_vertex("Mr. Qiu")->out();
    for (auto _o = _outs.first; _o != _outs.second; ++_o) {
        if (_drama.get_vertex(_o->first)->value().view() == CPC) {
            _s.insert(_o->first);
        }
    }
    for (const auto& _k : _s) {
        _drama.disconnect("Mr. Qiu", _k);
        _drama.disconnect(_k, "Mr. Qiu");
    }
    const auto _dijk = _drama.dijkstra<unsigned>("Chen Cuiping", [](const edge_type& _e) -> unsigned {
        switch (_e.value()) {
            case LEAD: case FOLLOW: return 3;
            case POST: case MARRY: case LOVE: case RAISE: return 1;
            default: return std::numeric_limits<unsigned>::max();
        }
    });
    std::vector<key_type> _trail;
    _dijk.trail("Li Kenong", [&_trail](const key_type& _k) {
        _trail.emplace_back(_k);
    });
    EXPECT_EQ(_dijk.cost("Li Kenong"), 8);
    EXPECT_EQ(_trail.size(), 5);
}
ICY_CASE("Six Degrees of Separation") {
    auto _drama = episode<6>();
    const auto _floyd = _drama.floyd<unsigned>([](const edge_type& _e) -> unsigned {
        return 1u;
    });
    for (const auto& [_i, _iv] : _drama.vertices()) {
        for (const auto& [_j, _jv] : _drama.vertices()) {
            EXPECT_LE(_floyd.cost(_i, _j), 6);
        }
    }
}