#include "main.hpp"

#include "graph.hpp"

#include <string>
#include <queue>

enum relation_type {
    PREDATION, // 捕食
    MUTUALISM, // 互利共生
    PARASITISM, // 寄生
    COMPETITION, // 竞争
};

int main() {
    icy::graph<std::string, void, relation_type> _bio; // biosphere
    using vertex_type = typename decltype(_bio)::vertex_type;
    using edge_type = typename decltype(_bio)::edge_type;
    using key_type = typename decltype(_bio)::key_type;

    _bio.insert("grass");
    _bio.insert("wheat");
    _bio.insert("insect");
    _bio.insert("rabbit");
    _bio.insert("lark");
    _bio.insert("fox");
    _bio.insert("eagle");
    _bio.insert("parasite"); // 寄生虫

    _bio.connect("grass", "insect", PREDATION);
    _bio.connect("grass", "rabbit", PREDATION);
    _bio.connect("grass", "lark", PREDATION);
    _bio.connect("wheat", "insect", PREDATION);
    _bio.connect("wheat", "lark", PREDATION);

    _bio.connect("insect", "lark", PREDATION);

    _bio.connect("rabbit", "fox", PREDATION);
    _bio.connect("lark", "fox", PREDATION);
    _bio.connect("rabbit", "eagle", PREDATION);
    _bio.connect("lark", "eagle", PREDATION);

    _bio.connect("parasite", "insect", PARASITISM);

    EXPECT_EQ(_bio.order(), 8);
    EXPECT_EQ(_bio.size(), 11);

    // COMPETITION. if two organisms prey on the same species and there is no relationship between them, they are in a competitive relationship.
    for (auto _i = _bio.begin(); _i != _bio.end(); ++_i) {
        vertex_type& _v = *_i->second;
        std::unordered_set<key_type> _s;
        const auto _out = _v.out();
        for (auto _j = _out.first; _j != _out.second; ++_j) {
            if (_j->second->value() == PREDATION) {
                for (const auto& _k : _s) {
                    if (!_bio.adjacent(_j->first, _k) && !_bio.adjacent(_k, _j->first)) {
                        _bio.biconnect(_j->first, _k, COMPETITION);
                    }
                }
                _s.insert(_j->first);
            }
        }
    }
    EXPECT_EQ(_bio.order(), 8);
    EXPECT_EQ(_bio.size(), 17);
    EXPECT_EQ(_bio.get_edge("insect", "rabbit")->value(), COMPETITION);
    EXPECT_EQ(_bio.get_edge("eagle", "fox")->value(), COMPETITION);

    // PARASITISM parasites are transmitted along the predator food chain
    vertex_type& _v = _bio.get_vertex("parasite");
    std::queue<key_type> _q;
    std::unordered_set<key_type> _victim;
    const auto _out = _v.out();
    for (auto _i = _out.first; _i != _out.second; ++_i) {
        if (_i->second->value() == PARASITISM) {
            _q.push(_i->first);
        }
    }
    while (!_q.empty()) {
        const key_type& _k = _q.front(); _q.pop();
        vertex_type& _v = _bio.get_vertex(_k);
        const auto _out = _v.out();
        for (auto _i = _out.first; _i != _out.second; ++_i) {
            if (_i->second->value() == PREDATION && !_victim.contains(_i->first)) {
                _q.push(_i->first);
            }
        }
        if (!_bio.adjacent("parasite", _k)) {
            _bio.connect("parasite", _k, PARASITISM);
            _victim.insert(_k);
        }
    }
    EXPECT_EQ(_bio.order(), 8);
    EXPECT_EQ(_bio.size(), 20);
    EXPECT_EQ(_bio.get_edge("parasite", "rabbit"), nullptr);
    EXPECT_EQ(_bio.get_edge("parasite", "fox")->value(), PARASITISM);
    EXPECT_EQ(_bio.get_vertex("rabbit").indegree(), 3);
    EXPECT_EQ(_bio.get_vertex("rabbit").outdegree(), 4);
    EXPECT_EQ(_bio.get_vertex("lark").indegree(), 5);
    EXPECT_EQ(_bio.get_vertex("lark").outdegree(), 3);

    auto _bio_without_insect = _bio;
    EXPECT_EQ(_bio, _bio_without_insect);

    _bio_without_insect.erase("insect");
    EXPECT_EQ(_bio_without_insect.order(), 7);
    EXPECT_EQ(_bio_without_insect.size(), 14);
    EXPECT_EQ(_bio_without_insect.get_edge("parasite", "rabbit"), nullptr);
    EXPECT_EQ(_bio_without_insect.get_edge("parasite", "fox")->value(), PARASITISM);
    EXPECT_NQ(_bio, _bio_without_insect);

    const auto _floyd = _bio.floyd<unsigned>([](const edge_type& _e) -> unsigned {
        switch (_e.value()) { // wasted energy
            case PREDATION: return 1;
            case PARASITISM: return 2;
            default: return 100;
        }
    });
    std::function<std::vector<key_type>(const key_type& _i, const key_type& _j)> get_trace = 
    [&get_trace, &_floyd](const key_type& _i, const key_type& _j) -> std::vector<key_type> {
        assert(_floyd.contains(_i) && _floyd.contains(_j));
        if (!_floyd.adjacent(_i, _j)) { return {}; }
        if (_i == _j) { return {_i}; }
        const key_type& _k = _floyd.get_edge(_i, _j)->value();
        assert(_k != _j);
        if (_i == _k) { return {_i, _j}; }
        auto _front = get_trace(_i, _k);
        auto _back = get_trace(_k, _j);
        assert(!_front.empty() && !_back.empty());
        assert(_front.back() == _back.front());
        _front.insert(_front.end(), _back.begin() + 1, _back.end());
        return _front;
    };
    const auto _trace_insect_fox = get_trace("insect", "fox");
    const auto _real_trace_insect_fox = std::vector<key_type>{"insect", "lark", "fox"};
    EXPECT_EQ(_trace_insect_fox, _real_trace_insect_fox);

    icy::multigraph<std::string, void, relation_type> _mbio = _bio;
    EXPECT_EQ(_mbio.order(), 8);
    EXPECT_EQ(_mbio.size(), 20);
    _mbio.biconnect("lark", "insect", COMPETITION);
    EXPECT_EQ(_mbio.size(), 22);

    return 0;
}