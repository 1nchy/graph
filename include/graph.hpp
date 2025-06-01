#ifndef _ICY_GRAPH_HPP_
#define _ICY_GRAPH_HPP_

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <memory>
#include <limits>
#include <functional>
#include <type_traits>

namespace icy {

namespace {
template <typename _Tp> struct storage;
template <typename _Tp> struct storage {
public:
    using value_type = _Tp;
public:
    template <typename... _Args> storage(_Args&&... _args) : _v(std::forward<_Args>(_args)...) {}
    storage(const storage&) = default;
    virtual ~storage() = default;
public:
    inline auto value() -> value_type& { return _v; }
    inline auto value() const -> const value_type& { return _v; }
    inline auto set_value(const value_type& _v) -> void { this->_v = _v; }
private:
    value_type _v;
};
template <> struct storage<void> {
public:
    using value_type = void;
public:
    storage() = default;
    storage(const storage&) = default;
    virtual ~storage() = default;
};
}

template <typename _Ev, typename _Vk, typename _Vv, typename _Hash> struct edge;
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> struct vertex;

namespace {
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> struct alloc;
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> struct alloc: public _Alloc {
public:
    using vertex_type = vertex<_Vk, _Vv, _Ev, _Hash>;
    using edge_type = edge<_Ev, _Vk, _Vv, _Hash>;
    using allocator_type = _Alloc;
    using alloc_traits = std::allocator_traits<allocator_type>;
    using vertex_allocator_type = typename alloc_traits::template rebind_alloc<vertex_type>;
    using vertex_alloc_traits = std::allocator_traits<vertex_allocator_type>;
    using edge_allocator_type = typename alloc_traits::template rebind_alloc<edge_type>;
    using edge_alloc_traits = std::allocator_traits<edge_allocator_type>;
private:
    auto _M_get_allocator() -> allocator_type& { return *static_cast<allocator_type*>(this); }
    auto _M_get_allocator() const -> const allocator_type& { return *static_cast<const allocator_type*>(this); }
    auto _M_get_vertex_allocator() const -> vertex_allocator_type { return vertex_allocator_type(_M_get_allocator()); }
    auto _M_get_edge_allocator() const -> edge_allocator_type { return edge_allocator_type(_M_get_allocator()); }
public:
    template <typename... _Args> auto _M_allocate_vertex(_Args&&... _args) const -> vertex_type* {
        vertex_allocator_type _alloc = _M_get_vertex_allocator();
        auto _ptr = vertex_alloc_traits::allocate(_alloc, 1);
        vertex_type* _p = std::addressof(*_ptr);
        vertex_alloc_traits::construct(_alloc, _p, std::forward<_Args>(_args)...);
        return _p;
    }
    auto _M_deallocate_vertex(vertex_type* _p) const -> void {
        vertex_allocator_type _alloc = _M_get_vertex_allocator();
        vertex_alloc_traits::destroy(_alloc, _p);
        vertex_alloc_traits::deallocate(_alloc, _p, 1);
    }
    template <typename... _Args> auto _M_allocate_edge(_Args&&... _args) const -> edge_type* {
        edge_allocator_type _alloc = _M_get_edge_allocator();
        auto _ptr = edge_alloc_traits::allocate(_alloc, 1);
        edge_type* _p = std::addressof(*_ptr);
        edge_alloc_traits::construct(_alloc, _p, std::forward<_Args>(_args)...);
        return _p;
    }
    auto _M_deallocate_edge(edge_type* _p) const -> void {
        edge_allocator_type _alloc = _M_get_edge_allocator();
        edge_alloc_traits::destroy(_alloc, _p);
        edge_alloc_traits::deallocate(_alloc, _p, 1);
    }
};
}

template <typename _Vk, typename _Vv = void, typename _Ev = void,
typename _Hash = std::hash<_Vk>, typename _Alloc = std::allocator<_Vk>> struct graph;

template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> struct vertex : public storage<_Vv> {
public:
    using base = storage<_Vv>;
    using key_type = _Vk;
    using value_type = typename base::value_type;
    using vertex_type = vertex<_Vk, _Vv, _Ev, _Hash>;
    using edge_type = edge<_Ev, _Vk, _Vv, _Hash>;
    using iterator = typename std::unordered_multimap<key_type, edge_type*>::iterator;
    using const_iterator = typename std::unordered_multimap<key_type, edge_type*>::const_iterator;
public:
    template <typename... _Args> vertex(_Args&&...);
    vertex(const vertex_type&) = delete;
    vertex(vertex_type&&) = default;
    auto operator=(const vertex_type&) -> vertex_type& = delete;
    auto operator=(vertex_type&&) -> vertex_type& = delete;
    virtual ~vertex() = default;
    template <typename __Vk, typename __Vv, typename __Ev, typename __Hash, typename _Alloc> friend struct graph;
public:
    auto operator==(const vertex_type&) const -> bool;
    auto operator!=(const vertex_type&) const -> bool;
public:
    inline auto indegree() const -> size_t;
    inline auto outdegree() const -> size_t;
    /**
     * @brief contains edge from `_k` to this
     */
    inline auto contains_from(const key_type& _k) const -> bool;
    /**
     * @brief contains edge from this to `_k`
     */
    inline auto contains_to(const key_type& _k) const -> bool;
    /**
     * @brief count edge from `_k` to this
     */
    inline auto count_from(const key_type& _k) const -> size_t;
    /**
     * @brief count edge from this to `_k`
     */
    inline auto count_to(const key_type& _k) const -> size_t;
    /**
     * @brief edge range from `_k` to this
     * @return iterator<key_type, edge_type*>
     */
    inline auto range_from(const key_type& _k) -> std::pair<iterator, iterator>;
    inline auto range_from(const key_type& _k) const -> std::pair<const_iterator, const_iterator>;
    /**
     * @brief edge range from this to `_k`
     * @return iterator<key_type, edge_type*>
     */
    inline auto range_to(const key_type& _k) -> std::pair<iterator, iterator>;
    inline auto range_to(const key_type& _k) const -> std::pair<const_iterator, const_iterator>;
    inline auto in() -> std::pair<iterator, iterator>;
    inline auto in() const -> std::pair<const_iterator, const_iterator>;
    inline auto out() -> std::pair<iterator, iterator>;
    inline auto out() const -> std::pair<const_iterator, const_iterator>;
private:
    /**
     * @brief insert edge from `_k` to this
     */
    inline auto insert_from(const key_type& _k, edge_type* const _e) -> size_t;
    /**
     * @brief insert edge from this to `_k`
     */
    inline auto insert_to(const key_type& _k, edge_type* const _e) -> size_t;
    /**
     * @brief erase edge from `_k` to this
     */
    inline auto erase_from(const key_type& _k) -> size_t;
    /**
     * @brief erase edge from this to `_k`
     */
    inline auto erase_to(const key_type& _k) -> size_t;
    inline auto clear() -> void;
private:
    std::unordered_multimap<key_type, edge_type*, _Hash> _in;
    std::unordered_multimap<key_type, edge_type*, _Hash> _out;
};
template <typename _Ev, typename _Vk, typename _Vv, typename _Hash> struct edge : public storage<_Ev> {
public:
    using base = storage<_Ev>;
    using value_type = typename base::value_type;
    using vertex_type = vertex<_Vk, _Vv, _Ev, _Hash>;
    using edge_type = edge<_Ev, _Vk, _Vv, _Hash>;
    using key_type = typename vertex_type::key_type;
public:
    template <typename... _Args> edge(const key_type&, vertex_type*, const key_type&, vertex_type*, _Args&&...);
    edge(const edge_type&) = delete;
    edge(edge_type&&) = default;
    auto operator=(const edge_type&) -> edge_type& = delete;
    auto operator=(edge_type&&) -> edge_type& = delete;
    virtual ~edge() = default;
public:
    auto operator==(const edge_type&) const -> bool;
    auto operator!=(const edge_type&) const -> bool;
public:
    auto in() const -> const vertex_type&;
    auto out() const -> const vertex_type&;
    auto in_key() const -> const key_type&;
    auto out_key() const -> const key_type&;
private:
    const std::pair<key_type, vertex_type*> _in;
    const std::pair<key_type, vertex_type*> _out;
};


template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> template <typename... _Args>
vertex<_Vk, _Vv, _Ev, _Hash>::vertex(_Args&&... _args) : base(std::forward<_Args>(_args)...) {}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::operator==(const vertex_type& _rhs) const -> bool {
    return &_rhs == this;
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::operator!=(const vertex_type& _rhs) const -> bool {
    return !operator==(_rhs);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::indegree() const -> size_t {
    return _in.size();
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::outdegree() const -> size_t {
    return _out.size();
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::contains_from(const key_type& _k) const -> bool {
    return _in.contains(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::contains_to(const key_type& _k) const -> bool {
    return _out.contains(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::count_from(const key_type& _k) const -> size_t {
    return _in.count(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::count_to(const key_type& _k) const -> size_t {
    return _out.count(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::range_from(const key_type& _k) -> std::pair<iterator, iterator> {
    return _in.equal_range(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::range_from(const key_type& _k) const -> std::pair<const_iterator, const_iterator> {
    return _in.equal_range(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::range_to(const key_type& _k) -> std::pair<iterator, iterator> {
    return _out.equal_range(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::range_to(const key_type& _k) const -> std::pair<const_iterator, const_iterator> {
    return _out.equal_range(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::in() -> std::pair<iterator, iterator> {
    return std::make_pair(_in.begin(), _in.end());
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::in() const -> std::pair<const_iterator, const_iterator> {
    return std::make_pair(_in.cbegin(), _in.cend());
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::out() -> std::pair<iterator, iterator> {
    return std::make_pair(_out.begin(), _out.end());
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::out() const -> std::pair<const_iterator, const_iterator> {
    return std::make_pair(_out.cbegin(), _out.cend());
}


template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::insert_from(const key_type& _k, edge_type* const _e) -> size_t {
    _in.insert({_k, _e});
    return _in.count(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::insert_to(const key_type& _k, edge_type* const _e) -> size_t {
    _out.insert({_k, _e});
    return _out.count(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::erase_from(const key_type& _k) -> size_t {
    return _in.erase(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::erase_to(const key_type& _k) -> size_t {
    return _out.erase(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, _Vv, _Ev, _Hash>::clear() -> void {
    _in.clear(); _out.clear();
}

template <typename _Ev, typename _Vk, typename _Vv, typename _Hash> template <typename... _Args> edge<_Ev, _Vk, _Vv, _Hash>::
edge(const key_type& _xk, vertex_type* _x, const key_type& _yk, vertex_type* _y, _Args&&... _args) : 
base(std::forward<_Args>(_args)...), _in(_xk, _x), _out(_yk, _y) {}
template <typename _Ev, typename _Vk, typename _Vv, typename _Hash> auto
edge<_Ev, _Vk, _Vv, _Hash>::operator==(const edge_type& _rhs) const -> bool {
    return &_rhs == this;
}
template <typename _Ev, typename _Vk, typename _Vv, typename _Hash> auto
edge<_Ev, _Vk, _Vv, _Hash>::operator!=(const edge_type& _rhs) const -> bool {
    return !operator==(_rhs);
}
template <typename _Ev, typename _Vk, typename _Vv, typename _Hash> auto
edge<_Ev, _Vk, _Vv, _Hash>::in() const -> const vertex_type& {
    return *(_in.second);
}
template <typename _Ev, typename _Vk, typename _Vv, typename _Hash> auto
edge<_Ev, _Vk, _Vv, _Hash>::out() const -> const vertex_type& {
    return *(_out.second);
}
template <typename _Ev, typename _Vk, typename _Vv, typename _Hash> auto
edge<_Ev, _Vk, _Vv, _Hash>::in_key() const -> const key_type& {
    return _in.first;
}
template <typename _Ev, typename _Vk, typename _Vv, typename _Hash> auto
edge<_Ev, _Vk, _Vv, _Hash>::out_key() const -> const key_type& {
    return _out.first;
}



template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc>
struct graph : public alloc<_Vk, _Vv, _Ev, _Hash, _Alloc> {
public:
    using vertex_type = vertex<_Vk, _Vv, _Ev, _Hash>;
    using edge_type = edge<_Ev, _Vk, _Vv, _Hash>;
    using key_type = typename vertex_type::key_type;
    using edge_iterator = typename vertex_type::iterator;
    using edge_const_iterator = typename vertex_type::const_iterator;
    template <typename _R> using vertex_visitor = std::function<_R(const vertex_type&)>;
    template <typename _R> using edge_visitor = std::function<_R(const edge_type&)>;
public:
    inline auto vertices() const -> size_t;
    inline auto edges() const -> size_t;
    inline auto empty() const -> bool;
    inline auto contains(const key_type&) const -> bool;
    inline auto contains(const key_type&, const key_type&) const -> bool;
    inline auto count(const key_type&, const key_type&) const -> size_t;
    inline auto get_vertex(const key_type&) -> vertex_type&;
    inline auto get_vertex(const key_type&) const -> const vertex_type&;
    inline auto get_edge(const key_type&, const key_type&) -> std::pair<edge_iterator, edge_iterator>;
    inline auto get_edge(const key_type&, const key_type&) const -> std::pair<edge_const_iterator, edge_const_iterator>;
    template <typename... _Args> auto insert(const key_type&, _Args&&...) -> void;
    auto erase(const key_type&) -> void;
    template <typename... _Args> auto connect(const key_type&, const key_type&, _Args&&...) -> void;
    template <typename... _Args> auto biconnect(const key_type&, const key_type&, _Args&&...) -> void;
    auto disconnect(const key_type&) -> void;
    auto disconnect(const key_type&, const key_type&) -> void;
    auto clear() -> void;
public: // algorithm
    /**
     * @brief dijkstra algorithm
     * @param _k start vertex key
     * @param _visitor edge visitor, _R(*)(const edge_type&)
     * @return predecessor edge for each vertex (exclude unreachable vertices)
     */
    template <typename _R> auto dijkstra(const key_type& _k, edge_visitor<_R>&& _visitor) const -> std::unordered_map<key_type, const edge_type*>;
private:
    std::unordered_map<key_type, vertex_type*, _Hash> _vertices;
    std::unordered_set<edge_type*> _edges;
};




template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::vertices() const -> size_t {
    return _vertices.size();
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::edges() const -> size_t {
    return _edges.size();
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::empty() const -> bool {
    return _vertices.empty();
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::contains(const key_type& _k) const -> bool {
    return _vertices.contains(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::contains(const key_type& _x, const key_type& _y) const -> bool {
    return contains(_x) && contains(_y) && get_vertex(_x)->contains_to(_y);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::count(const key_type& _x, const key_type& _y) const -> size_t {
    return (contains(_x) && contains(_y)) ? get_vertex(_x)->count_to(_y) : 0;
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::get_vertex(const key_type& _k) -> vertex_type& {
    return *_vertices.at(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::get_vertex(const key_type& _k) const -> const vertex_type& {
    return *_vertices.at(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::get_edge(const key_type& _x, const key_type& _y) -> std::pair<edge_iterator, edge_iterator> {
    return _vertices.at(_x)->range_to(_y);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::get_edge(const key_type& _x, const key_type& _y) const -> std::pair<edge_const_iterator, edge_const_iterator> {
    return _vertices.at(_x)->range_to(_y);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename... _Args> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::insert(const key_type& _k, _Args&&... _args) -> void {
    if (contains(_k)) { return; }
    vertex_type* const _v = this->_M_allocate_vertex(std::forward<_Args>(_args)...);
    _vertices[_k] = _v;
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::erase(const key_type& _k) -> void {
    if (!contains(_k)) { return; }
    disconnect(_k);
    vertex_type& _vertex = get_vertex(_k);
    _vertices.erase(_k);
    this->_M_deallocate_vertex(&_vertex);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename... _Args> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::connect(const key_type& _x, const key_type& _y, _Args&&... _args) -> void {
    if (!contains(_x) || !contains(_y)) { return; }
    vertex_type& _from = get_vertex(_x);
    vertex_type& _to = get_vertex(_y);
    edge_type* const _e = this->_M_allocate_edge(_x, &_from, _y, &_to, std::forward<_Args>(_args)...);
    _edges.insert(_e);
    _from.insert_to(_y, _e);
    _to.insert_from(_x, _e);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename... _Args> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::biconnect(const key_type& _x, const key_type& _y, _Args&&... _args) -> void {
    connect(_x, _y, std::forward<_Args>(_args)...);
    connect(_y, _x, std::forward<_Args>(_args)...);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::disconnect(const key_type& _k) -> void {
    if (!contains(_k)) { return; }
    vertex_type& _vertex = get_vertex(_k);
    // erase edges, update other vertices
    const auto _in_range = _vertex.in();
    for (auto _i = _in_range.first; _i != _in_range.second; ++_i) {
        get_vertex(_i->first).erase_to(_k);
        _edges.erase(_i->second);
        this->_M_deallocate_edge(_i->second);
    }
    const auto _out_range = _vertex.out();
    for (auto _i = _out_range.first; _i != _out_range.second; ++_i) {
        get_vertex(_i->first).erase_from(_k);
        _edges.erase(_i->second);
        this->_M_deallocate_edge(_i->second);
    }
    // update vertex
    _vertex.clear();
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::disconnect(const key_type& _x, const key_type& _y) -> void {
    if (!contains(_x) || !contains(_y)) { return; }
    // erase edges
    const auto _range = get_edge(_x, _y);
    for (auto _i = _range.first; _i != _range.second; ++_i) {
        _edges.erase(_i->second);
        this->_M_deallocate_edge(_i->second);
    }
    // update vertex
    get_vertex(_x).erase_to(_y);
    get_vertex(_y).erase_from(_x);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::clear() -> void {
    for (auto [_k, _v] : _vertices) {
        this->_M_deallocate_vertex(_v);
    }
    for (auto _e : _edges) {
        this->_M_deallocate_edge(_e);
    }
    _vertices.clear();
    _edges.clear();
}


/// algorithm implement
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename _R> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::dijkstra(const key_type& _k, edge_visitor<_R>&& _visitor) const -> std::unordered_map<key_type, const edge_type*> {
    using cost_type = _R;
    static_assert(std::is_arithmetic<cost_type>::value);
    if (!contains(_k)) { return {}; }
    std::unordered_set<key_type> _u; // _s({_k});
    for (const auto& [_k, _v] : _vertices) {
        _u.insert(_k);
    }
    _u.erase(_k);
    std::vector<std::pair<cost_type, key_type>> _costs; // cost from `_s` to `_u`, always greater ordered
    _costs.reserve(_u.size());
    for (const auto& _k : _u) {
        _costs.emplace_back(std::numeric_limits<cost_type>::max(), _k);
    }
    std::unordered_map<key_type, const edge_type*> _predecessor;
    const vertex_type* _vertex = &get_vertex(_k);
    while (!_u.empty()) {
        // update `_costs` and `_predecessor`
        for (auto _i = _costs.begin(); _i != _costs.end(); ++_i) {
            const auto _out = _vertex->range_to(_i->second);
            // update cost from `_vertex` to `_i->second`
            for (auto _j = _out.first; _j != _out.second; ++_j) {
                const cost_type _cost = _visitor(*_j->second);
                if (_i->first > _cost) {
                    _i->first = _cost;
                    _predecessor[_i->second] = _j->second;
                }
            }
        }
        std::sort(_costs.begin(), _costs.end(), [](const auto& _x, const auto& _y) {
            return _x.first > _y.first;
        });
        // choose lowest one (check max)
        if (_costs.back().first == std::numeric_limits<cost_type>::max()) {
            break;
        }
        const key_type& _k = _costs.back().second;
        // update `_vertex`, erase from `_u` and `_costs`
        _vertex = &get_vertex(_k);
        _u.erase(_k);
        _costs.pop_back();
    }
    return _predecessor;
}

}

#endif // _ICY_GRAPH_HPP_