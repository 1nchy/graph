/// metro

#include "test.hpp"
#include "graph.hpp"

#include <map>

enum land_type {
    RESIDENTIAL, // 居住用地
    PUBLIC, // 公共服务用地
    COMMERCIAL, // 商业用地
    INDUSTRIAL, // 工业用地
    TRANSPORTATION, // 交通用地
    SCHOOL, // 学校
    GREEN, // 绿地
};

ICY_CASE("Shenyang") {
    icy::undirected_graph<std::string, land_type, unsigned> _metro;
    using key_type = typename decltype(_metro)::key_type;
    using vertex_type = typename decltype(_metro)::vertex_type;
    using edge_type = typename decltype(_metro)::edge_type;
    using cost_type = unsigned;
    _metro.insert("Yinbin Road", RESIDENTIAL);
    _metro.insert("Tiexi Square", PUBLIC);
    _metro.insert("Shenyang Railway Station", TRANSPORTATION);
    _metro.insert("Taiyuan Street", COMMERCIAL);
    _metro.insert("Teenager Street", COMMERCIAL);
    _metro.insert("Midst Street", COMMERCIAL);
    _metro.insert("Pangjiang Street", COMMERCIAL);
    _metro.insert("Dawn Square", PUBLIC);
    _metro.connect("Yinbin Road", "Tiexi Square", 9);
    _metro.connect("Tiexi Square", "Shenyang Railway Station", 5);
    _metro.connect("Shenyang Railway Station", "Taiyuan Street", 3);
    _metro.connect("Taiyuan Street", "Teenager Street", 4);
    _metro.connect("Teenager Street", "Midst Street", 5);
    _metro.connect("Midst Street", "Pangjiang Street", 5);
    _metro.connect("Pangjiang Street", "Dawn Square", 2);
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
    _metro.connect("Liaoning University", "TCM University", 22);
    _metro.connect("TCM University", "Shenyangbei Railway Station", 4);
    _metro.connect("Shenyangbei Railway Station", "People's Square", 4);
    _metro.connect("People's Square", "Teenager Street", 3);
    _metro.connect("Teenager Street", "Industry Museum", 4);
    _metro.connect("Industry Museum", "City Library", 3);
    _metro.connect("City Library", "Olympic Center", 4);
    _metro.connect("Olympic Center", "Province Museum", 7);
    _metro.connect("Province Museum", "Central Park", 3);
    _metro.connect("Central Park", "Nova 1st Road", 3);
    _metro.connect("Nova 1st Road", "Taoxian Airport", 7);
    /// line 3
    _metro.insert("Industry University", SCHOOL);
    _metro.insert("Datonghu Street", COMMERCIAL);
    _metro.insert("Shayang", RESIDENTIAL);
    _metro.insert("Industry Museum", INDUSTRIAL);
    _metro.insert("Jiangdong Street", COMMERCIAL);
    _metro.connect("Industry University", "Datonghu Street", 16);
    /// line 4
    _metro.insert("Zhengxin Road", RESIDENTIAL);
    _metro.insert("Hezuo Street", COMMERCIAL);
    _metro.insert("Shenyang University", SCHOOL);
    _metro.insert("Shenyangbei Railway Station", TRANSPORTATION);
    _metro.insert("Taiyuan Street", COMMERCIAL);
    _metro.insert("Shayang", RESIDENTIAL);
    _metro.insert("Changbainan", RESIDENTIAL);
    _metro.insert("Shenyangnan Railway Station", TRANSPORTATION);
    _metro.connect("Zhengxin Road", "Hezuo Street", 12);
    _metro.connect("Hezuo Street", "Shenyang University", 4);
    _metro.connect("Shenyang University", "Shenyangbei Railway Station", 4);
    _metro.connect("Shenyangbei Railway Station", "Taiyuan Street", 8);
    _metro.connect("Taiyuan Street", "Shayang", 5);
    _metro.connect("Shayang", "Changbainan", 8);
    _metro.connect("Changbainan", "Shenyangnan Railway Station", 13);
    /// line 6
    _metro.insert("Hero Park", GREEN);
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
    _metro.connect("Nujiang Park", "Shenyang 2nd Hospital", 2);
    _metro.connect("Shenyang 2nd Hospital", "Tiexi Square", 9);
    _metro.connect("Tiexi Square", "Xinghua Park", 2);
    _metro.connect("Xinghua Park", "Datonghu Street", 10);
    _metro.connect("Datonghu Street", "Changbainan", 11);
    _metro.connect("Changbainan", "Olympic Center", 9);
    _metro.connect("Olympic Center", "Changqingnan Street", 8);
    _metro.connect("Changqingnan Street", "Building University", 2);
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
    _metro.connect("Dingxiang Lake", "Shenyang 2nd Hospital", 10);
    _metro.connect("Shenyang 2nd Hospital", "TCM University", 6);
    _metro.connect("TCM University", "Beita", 5);
    _metro.connect("Beita", "Hezuo Street", 3);
    _metro.connect("Hezuo Street", "Pangjiang Street", 5);
    _metro.connect("Pangjiang Street", "Chang'an Road", 3);
    _metro.connect("Chang'an Road", "Jiangdong Street", 7);
    _metro.connect("Jiangdong Street", "Changqingnan Street", 3);
    _metro.connect("Changqingnan Street", "Institute of Technology", 3);
    ///
    EXPECT_EQ(_metro.order(), 38);
    EXPECT_EQ(_metro.size(), 43);
    ICY_SUBCASE("from Shenyang Railway Station to each SCHOOL") {
        const auto _dijk = _metro.dijkstra<cost_type>("Shenyang Railway Station", [](const edge_type& _e) -> cost_type {
            return _e.value();
        });
        std::map<key_type, cost_type> _school;
        for (const auto& [_k, _v] : _metro.vertices()) {
            if (_v->value() == SCHOOL) {
                _school[_k] = _dijk.cost(_k);
            }
        }
        EXPECT_EQ(_school.size(), 6);
        // EXPECT_EQ(test::to_string(_school), "");
    }
    ICY_SUBCASE("from RESIDENTIAL to GREEN") {
        const auto _floyd = _metro.floyd<cost_type>([](const edge_type& _e) -> cost_type {
            return _e.value();
        });
        std::vector<key_type> _green;
        for (const auto& [_k, _v] : _metro.vertices()) {
            if (_v->value() == GREEN) {
                _green.emplace_back(_k);
            }
        }
        std::map<key_type, cost_type> _residential;
        for (const auto& [_k, _v] : _metro.vertices()) {
            if (_v->value() == RESIDENTIAL) {
                _residential[_k] = std::numeric_limits<cost_type>::max();
                for (const auto& _g : _green) {
                    _residential[_k] = std::min(_residential[_k], _floyd.cost(_k, _g));
                }
            }
        }
        EXPECT_EQ(_residential.size(), 7);
        // EXPECT_EQ(test::to_string(_residential), "");
    }
}