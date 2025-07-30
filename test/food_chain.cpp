/// test graph

#include "test.hpp"
#include "graph.hpp"

enum role {
    PRODUCER,
    CONSUMER,
    DECOMPOSER
};

using graph = icy::graph<std::string, role, void>;

using key_type = typename graph::key_type;
using vertex_type = typename graph::vertex_type;
using edge_type = typename graph::edge_type;
using cost_type = unsigned;

auto initialize(graph& _bio) -> void { // Africa
    _bio.clear();
    // producer
    _bio.insert("grass", PRODUCER);
    _bio.insert("bush", PRODUCER);
    _bio.insert("tree", PRODUCER);
    // consumer
    _bio.insert("giraffe", CONSUMER);
    _bio.insert("elephant", CONSUMER);
    _bio.insert("lion", CONSUMER);
    _bio.insert("rat", CONSUMER);
    _bio.insert("insect", CONSUMER);
    _bio.insert("lark", CONSUMER);
    _bio.insert("badger", CONSUMER);
    _bio.insert("fox", CONSUMER);
    _bio.insert("snake", CONSUMER);
    _bio.insert("lynx", CONSUMER);
    _bio.insert("panther", CONSUMER);
    _bio.insert("antelope", CONSUMER);
    _bio.insert("zebra", CONSUMER);
    // decomposers
    _bio.insert("vulture", DECOMPOSER);
    _bio.insert("termite", DECOMPOSER);
    // relation
    _bio.connect("grass", "rat");
    _bio.connect("grass", "insect");
    _bio.connect("grass", "elephant");
    _bio.connect("grass", "antelope");
    _bio.connect("grass", "zebra");
    _bio.connect("grass", "lark");
    _bio.connect("bush", "antelope");
    _bio.connect("bush", "zebra");
    _bio.connect("bush", "elephant");
    _bio.connect("bush", "giraffe");
    _bio.connect("tree", "giraffe");
    _bio.connect("tree", "elephant");

    _bio.connect("giraffe", "lion");
    _bio.connect("giraffe", "panther");
    _bio.connect("zebra", "lion");
    _bio.connect("zebra", "panther");
    _bio.connect("zebra", "lynx");
    _bio.connect("antelope", "lynx");
    _bio.connect("antelope", "panther");
    _bio.connect("antelope", "lion");

    _bio.connect("insect", "lark");
    _bio.connect("insect", "badger");
    _bio.connect("rat", "fox");
    _bio.connect("rat", "snake");
    _bio.connect("rat", "badger");
    _bio.connect("lark", "badger");
    _bio.connect("lark", "fox");

    _bio.connect("snake", "lynx");
    _bio.connect("snake", "badger");
    _bio.connect("fox", "lynx");
    _bio.connect("fox", "panther");
    _bio.connect("fox", "lion");
    _bio.connect("badger", "lynx");
    _bio.connect("badger", "panther");
    _bio.connect("badger", "lion");
    _bio.connect("lynx", "lion");
}

ICY_CASE("constructor / assignment") {
    graph _bio; initialize(_bio);
    EXPECT_EQ(_bio.order(), 18);
    EXPECT_EQ(_bio.size(), 36);
    ICY_SUBCASE("graph") {
        graph _g(_bio);
        EXPECT_EQ(_g.order(), 18);
        EXPECT_EQ(_g.size(), 36);
        EXPECT_EQ(_g, _bio);
        _g = _bio;
        EXPECT_EQ(_g.order(), 18);
        EXPECT_EQ(_g.size(), 36);
        EXPECT_EQ(_g, _bio);
    }
    ICY_SUBCASE("multigraph") {
        icy::multigraph<std::string, role, void> _g(_bio);
        EXPECT_EQ(_g.order(), 18);
        EXPECT_EQ(_g.size(), 36);
    }
}
ICY_CASE("research") {
    graph _bio; initialize(_bio);
    EXPECT_EQ(_bio.order(), 18);
    EXPECT_EQ(_bio.size(), 36);
    ICY_SUBCASE("no loop") {
        std::unordered_set<key_type> _indegree;
        for (const auto& [_k, _v] : _bio.vertices()) {
            if (_v->indegree() == 0) {
                _indegree.insert(_k);
            }
        }
        while (!_indegree.empty()) {
            const auto _k = *_indegree.cbegin(); _indegree.erase(_k);
            const auto _outs = _bio.get_vertex(_k)->out();
            std::unordered_set<key_type> _s;
            for (auto _o = _outs.first; _o != _outs.second; ++_o) {
                _s.insert(_o->first);
            }
            _bio.disconnect(_k);
            for (const auto& _k : _s) {
                if (_bio.get_vertex(_k)->indegree() == 0) {
                    _indegree.insert(_k);
                }
            }
        }
        EXPECT_EQ(_bio.size(), 0);
    }
    ICY_SUBCASE("competition") {
        graph _competition(_bio);
        for (const auto& [_k, _v] : _competition.vertices()) {
            _competition.disconnect(_k);
        }
        for (const auto& [_k, _v] : _bio.vertices()) {
            std::unordered_set<key_type> _s;
            const auto _outs = _v->out();
            for (auto _o = _outs.first; _o != _outs.second; ++_o) {
                for (const auto& _k : _s) {
                    if (!_bio.adjacent(_o->first, _k) && !_bio.adjacent(_k, _o->first)) {
                        _competition.biconnect(_k, _o->first);
                    }
                }
                _s.insert(_o->first);
            }
        }
        EXPECT_EQ(_competition.order(), 18);
        EXPECT_EQ(_competition.size(), 42);
    }
    ICY_SUBCASE("parasitism") { // the parasitic creature has no appetite, will not take food
        graph _parasitism; // tree at the end
        for (const auto& [_k, _v] : _bio.vertices()) {
            _parasitism.insert(_k);
        }
        _parasitism.insert("parasite");
        ICY_SUBCASE("from rat") {
            std::vector<key_type> _trail;
            _trail.emplace_back("parasite");
            _bio.dfs("rat", [&_trail, &_parasitism](const key_type& _k, const vertex_type& _v) -> void {
                _parasitism.connect(_trail.back(), _k);
                _trail.emplace_back(_k);
            }, [&_trail](const key_type& _k, const vertex_type& _v) -> void {
                _trail.pop_back();
            });
            EXPECT_EQ(_parasitism.size(), 7);
        }
        ICY_SUBCASE("from insect") {
            _parasitism.connect("parasite", "insect");
            _bio.bfs("insect", [&_parasitism](const key_type& _k, const vertex_type& _v) -> void {
                const auto _outs = _v.out();
                for (auto _o = _outs.first; _o != _outs.second; ++_o) {
                    if (_parasitism.get_vertex(_o->first)->indegree() > 0) { continue; }
                    _parasitism.connect(_k, _o->first);
                }
            });
            EXPECT_EQ(_parasitism.size(), 7);
        }
    }
    ICY_SUBCASE("the most / the least") {
        ICY_SUBCASE("the most omnivorous") {
            const auto _most = std::max_element(
                _bio.vertices().cbegin(), _bio.vertices().cend(),
                [](const auto& _x, const auto& _y) -> bool {
                    return _x.second->indegree() < _y.second->indegree();
                }
            );
            EXPECT_EQ(_most->first, "lion");
        }
        ICY_SUBCASE("longest food chain") {
            const auto _floyd = _bio.floyd<long>([](const edge_type& _e) -> long {
                return -1;
            });
            EXPECT_EQ(_floyd.cost("grass", "lion"), -5);
        }
    }
}