#include "main.hpp"

#include "graph.hpp"

#include <string>
#include <queue>

#include <iostream>

enum relation_type {
    PREDATION, // 捕食
    MUTUALISM, // 互利共生
    PARASITISM, // 寄生
    COMPETITION, // 竞争
};

int main() {
    icy::graph<std::string, void, relation_type> _biosphere;
    using vertex_type = typename decltype(_biosphere)::vertex_type;
    using edge_type = typename decltype(_biosphere)::edge_type;

    _biosphere.insert("grass");
    _biosphere.insert("wheat");
    _biosphere.insert("insect");
    _biosphere.insert("rabbit");
    _biosphere.insert("lark");
    _biosphere.insert("fox");
    _biosphere.insert("eagle");
    _biosphere.insert("parasite"); // 寄生虫

    _biosphere.connect("grass", "insect", PREDATION);
    _biosphere.connect("grass", "rabbit", PREDATION);
    _biosphere.connect("grass", "lark", PREDATION);
    _biosphere.connect("wheat", "insect", PREDATION);
    _biosphere.connect("wheat", "lark", PREDATION);

    _biosphere.connect("insect", "lark", PREDATION);

    _biosphere.connect("rabbit", "fox", PREDATION);
    _biosphere.connect("lark", "fox", PREDATION);
    _biosphere.connect("rabbit", "eagle", PREDATION);
    _biosphere.connect("lark", "eagle", PREDATION);

    _biosphere.connect("parasite", "insect", PARASITISM);

    EXPECT_EQ(_biosphere.vertices(), 8);
    EXPECT_EQ(_biosphere.edges(), 11);

    // COMPETITION. if two organisms prey on the same species and there is no relationship between them, they are in a competitive relationship.
    for (auto _i = _biosphere.begin(); _i != _biosphere.end(); ++_i) {
        vertex_type& _v = *_i->second;
        std::unordered_set<std::string> _s;
        const auto _out = _v.out();
        for (auto _j = _out.first; _j != _out.second; ++_j) {
            if (_j->second->value() == PREDATION) {
                for (const auto& _k : _s) {
                    if (!_biosphere.adjacent(_j->first, _k) && !_biosphere.adjacent(_k, _j->first)) {
                        _biosphere.biconnect(_j->first, _k, COMPETITION);
                    }
                }
                _s.insert(_j->first);
            }
        }
    }
    EXPECT_EQ(_biosphere.vertices(), 8);
    EXPECT_EQ(_biosphere.edges(), 17);
    EXPECT_EQ(_biosphere.get_edge("insect", "rabbit")->value(), COMPETITION);
    EXPECT_EQ(_biosphere.get_edge("eagle", "fox")->value(), COMPETITION);

    // PARASITISM parasites are transmitted along the predator food chain
    vertex_type& _v = _biosphere.get_vertex("parasite");
    std::queue<std::string> _q;
    std::unordered_set<std::string> _victim;
    const auto _out = _v.out();
    for (auto _i = _out.first; _i != _out.second; ++_i) {
        if (_i->second->value() == PARASITISM) {
            _q.push(_i->first);
        }
    }
    while (!_q.empty()) {
        const std::string& _k = _q.front(); _q.pop();
        vertex_type& _v = _biosphere.get_vertex(_k);
        const auto _out = _v.out();
        for (auto _i = _out.first; _i != _out.second; ++_i) {
            if (_i->second->value() == PREDATION && !_victim.contains(_i->first)) {
                _q.push(_i->first);
            }
        }
        if (!_biosphere.adjacent("parasite", _k)) {
            _biosphere.connect("parasite", _k, PARASITISM);
            _victim.insert(_k);
        }
    }
    EXPECT_EQ(_biosphere.vertices(), 8);
    EXPECT_EQ(_biosphere.edges(), 20);
    EXPECT_EQ(_biosphere.get_edge("parasite", "rabbit"), nullptr);
    EXPECT_EQ(_biosphere.get_edge("parasite", "fox")->value(), PARASITISM);
    EXPECT_EQ(_biosphere.get_vertex("rabbit").indegree(), 3);
    EXPECT_EQ(_biosphere.get_vertex("rabbit").outdegree(), 4);
    EXPECT_EQ(_biosphere.get_vertex("lark").indegree(), 5);
    EXPECT_EQ(_biosphere.get_vertex("lark").outdegree(), 3);

    _biosphere.erase("insect");
    EXPECT_EQ(_biosphere.vertices(), 7);
    EXPECT_EQ(_biosphere.edges(), 14);

    return 0;
}