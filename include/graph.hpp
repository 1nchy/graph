#ifndef _ICY_GRAPH_HPP_
#define _ICY_GRAPH_HPP_

#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include <cassert>
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
    auto operator==(const storage& _rhs) const -> bool { return _v == _rhs._v; }
    auto operator!=(const storage& _rhs) const -> bool { return _v != _rhs._v; }
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
public:
    auto operator==(const storage& _rhs) const -> bool { return true; }
    auto operator!=(const storage& _rhs) const -> bool { return false; }
};
}

template <typename _Ev, typename _Vk, bool _Multi, typename _Vv, typename _Hash> struct edge;
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash> struct vertex;

namespace {
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> struct alloc;
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> struct alloc: public _Alloc {
public:
    using vertex_type = vertex<_Vk, _Multi, _Vv, _Ev, _Hash>;
    using edge_type = edge<_Ev, _Vk, _Multi, _Vv, _Hash>;
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

template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> struct vertex<_Vk, true, _Vv, _Ev, _Hash> : public storage<_Vv> {
public:
    using base = storage<_Vv>;
    using key_type = _Vk;
    using value_type = typename base::value_type;
    using vertex_type = vertex<_Vk, true, _Vv, _Ev, _Hash>;
    using edge_type = edge<_Ev, _Vk, true, _Vv, _Hash>;
    using iterator = typename std::unordered_multimap<key_type, edge_type*>::iterator;
    using const_iterator = typename std::unordered_multimap<key_type, edge_type*>::const_iterator;
public:
    template <typename... _Args> vertex(_Args&&...);
    vertex(const vertex_type&) = delete;
    vertex(vertex_type&&) = default;
    auto operator=(const vertex_type&) -> vertex_type& = delete;
    auto operator=(vertex_type&&) -> vertex_type& = delete;
    virtual ~vertex() = default;
    template <typename __Vk, bool _Multi, typename __Vv, typename __Ev, typename __Hash, typename _Alloc> friend struct graph_base;
    template <typename __Vk, typename __Vv, typename __Ev, typename __Hash, typename _Alloc> friend struct graph;
    template <typename __Vk, typename __Vv, typename __Ev, typename __Hash, typename _Alloc> friend struct multigraph;
public:
    auto operator==(const vertex_type& _rhs) const -> bool { return base::operator==(_rhs); }
    auto operator!=(const vertex_type& _rhs) const -> bool { return base::operator!=(_rhs); }
public:
    inline auto indegree() const -> size_t { return _in.size(); }
    inline auto outdegree() const -> size_t { return _out.size(); }
    /**
     * @brief contains edge from `_k` to this
     */
    inline auto contains_from(const key_type& _k) const -> bool { return _in.contains(_k); }
    /**
     * @brief contains edge from this to `_k`
     */
    inline auto contains_to(const key_type& _k) const -> bool { return _out.contains(_k); }
    /**
     * @brief count edge from `_k` to this
     */
    inline auto count_from(const key_type& _k) const -> size_t { return _in.count(_k); }
    /**
     * @brief count edge from this to `_k`
     */
    inline auto count_to(const key_type& _k) const -> size_t { return _out.count(_k); }
    /**
     * @brief edge range from `_k` to this
     * @return iterator<key_type, edge_type*>
     */
    inline auto edge_from(const key_type& _k) -> std::pair<iterator, iterator> { return _in.equal_range(_k); }
    inline auto edge_from(const key_type& _k) const -> std::pair<const_iterator, const_iterator> { return _in.equal_range(_k); }
    /**
     * @brief edge range from this to `_k`
     * @return iterator<key_type, edge_type*>
     */
    inline auto edge_to(const key_type& _k) -> std::pair<iterator, iterator> { return _out.equal_range(_k); }
    inline auto edge_to(const key_type& _k) const -> std::pair<const_iterator, const_iterator> { return _out.equal_range(_k); }
    inline auto in() -> std::pair<iterator, iterator> { return std::make_pair(_in.begin(), _in.end()); }
    inline auto in() const -> std::pair<const_iterator, const_iterator> { return std::make_pair(_in.cbegin(), _in.cend()); }
    inline auto out() -> std::pair<iterator, iterator> { return std::make_pair(_out.begin(), _out.end()); }
    inline auto out() const -> std::pair<const_iterator, const_iterator> { return std::make_pair(_out.cbegin(), _out.cend()); }
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
    inline auto erase_from(const key_type& _k) -> size_t { return _in.erase(_k); }
    /**
     * @brief erase edge from this to `_k`
     */
    inline auto erase_to(const key_type& _k) -> size_t { return _out.erase(_k); }
    inline auto clear() -> void { _in.clear(); _out.clear(); }
private:
    std::unordered_multimap<key_type, edge_type*, _Hash> _in;
    std::unordered_multimap<key_type, edge_type*, _Hash> _out;
};
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> struct vertex<_Vk, false, _Vv, _Ev, _Hash> : public storage<_Vv> {
public:
    using base = storage<_Vv>;
    using key_type = _Vk;
    using value_type = typename base::value_type;
    using vertex_type = vertex<_Vk, false, _Vv, _Ev, _Hash>;
    using edge_type = edge<_Ev, _Vk, false, _Vv, _Hash>;
    using iterator = typename std::unordered_map<key_type, edge_type*>::iterator;
    using const_iterator = typename std::unordered_map<key_type, edge_type*>::const_iterator;
public:
    template <typename... _Args> vertex(_Args&&...);
    vertex(const vertex_type&) = delete;
    vertex(vertex_type&&) = default;
    auto operator=(const vertex_type&) -> vertex_type& = delete;
    auto operator=(vertex_type&&) -> vertex_type& = delete;
    virtual ~vertex() = default;
    template <typename __Vk, bool _Multi, typename __Vv, typename __Ev, typename __Hash, typename _Alloc> friend struct graph_base;
    template <typename __Vk, typename __Vv, typename __Ev, typename __Hash, typename _Alloc> friend struct graph;
    template <typename __Vk, typename __Vv, typename __Ev, typename __Hash, typename _Alloc> friend struct multigraph;
public:
    auto operator==(const vertex_type& _rhs) const -> bool { return base::operator==(_rhs); }
    auto operator!=(const vertex_type& _rhs) const -> bool { return base::operator!=(_rhs); }
public:
    inline auto indegree() const -> size_t { return _in.size(); }
    inline auto outdegree() const -> size_t { return _out.size(); }
    /**
     * @brief contains edge from `_k` to this
     */
    inline auto contains_from(const key_type& _k) const -> bool { return _in.contains(_k); }
    /**
     * @brief contains edge from this to `_k`
     */
    inline auto contains_to(const key_type& _k) const -> bool { return _out.contains(_k); }
    /**
     * @brief count edge from `_k` to this
     */
    inline auto count_from(const key_type& _k) const -> size_t { return _in.count(_k); }
    /**
     * @brief count edge from this to `_k`
     */
    inline auto count_to(const key_type& _k) const -> size_t { return _out.count(_k); }
    /**
     * @brief edge range from `_k` to this
     * @return iterator<key_type, edge_type*>
     */
    inline auto edge_from(const key_type& _k) -> edge_type* { return _in.contains(_k) ? _in.at(_k) : nullptr; }
    inline auto edge_from(const key_type& _k) const -> const edge_type* { return _in.contains(_k) ? _in.at(_k) : nullptr; }
    /**
     * @brief edge range from this to `_k`
     * @return iterator<key_type, edge_type*>
     */
    inline auto edge_to(const key_type& _k) -> edge_type* { return _out.contains(_k) ? _out.at(_k) : nullptr; }
    inline auto edge_to(const key_type& _k) const -> const edge_type* { return _out.contains(_k) ? _out.at(_k) : nullptr; }
    inline auto in() -> std::pair<iterator, iterator> { return std::make_pair(_in.begin(), _in.end()); }
    inline auto in() const -> std::pair<const_iterator, const_iterator> { return std::make_pair(_in.cbegin(), _in.cend()); }
    inline auto out() -> std::pair<iterator, iterator> { return std::make_pair(_out.begin(), _out.end()); }
    inline auto out() const -> std::pair<const_iterator, const_iterator> { return std::make_pair(_out.cbegin(), _out.cend()); }
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
    inline auto erase_from(const key_type& _k) -> size_t { return _in.erase(_k); }
    /**
     * @brief erase edge from this to `_k`
     */
    inline auto erase_to(const key_type& _k) -> size_t { return _out.erase(_k); }
    inline auto clear() -> void { _in.clear(); _out.clear(); }
private:
    std::unordered_map<key_type, edge_type*, _Hash> _in;
    std::unordered_map<key_type, edge_type*, _Hash> _out;
};
template <typename _Ev, typename _Vk, bool _Multi, typename _Vv, typename _Hash> struct edge : public storage<_Ev> {
public:
    using base = storage<_Ev>;
    using value_type = typename base::value_type;
    using vertex_type = vertex<_Vk, _Multi, _Vv, _Ev, _Hash>;
    using edge_type = edge<_Ev, _Vk, _Multi, _Vv, _Hash>;
    using key_type = typename vertex_type::key_type;
public:
    template <typename... _Args> edge(const key_type&, vertex_type*, const key_type&, vertex_type*, _Args&&...);
    edge(const edge_type&) = delete;
    edge(edge_type&&) = default;
    auto operator=(const edge_type&) -> edge_type& = delete;
    auto operator=(edge_type&&) -> edge_type& = delete;
    virtual ~edge() = default;
public:
    auto operator==(const edge_type& _rhs) const -> bool { return base::operator==(_rhs); }
    auto operator!=(const edge_type& _rhs) const -> bool { return base::operator!=(_rhs); }
public:
    auto in() const -> const vertex_type& { return *_in.second; }
    auto out() const -> const vertex_type& { return *_out.second; }
    auto in_key() const -> const key_type& { return _in.first; }
    auto out_key() const -> const key_type& { return _out.first; }
private:
    const std::pair<key_type, vertex_type*> _in;
    const std::pair<key_type, vertex_type*> _out;
};

// vertex<true> public implement
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> template <typename... _Args>
vertex<_Vk, true, _Vv, _Ev, _Hash>::vertex(_Args&&... _args) : base(std::forward<_Args>(_args)...) {}
// vertex<true> private implement
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, true, _Vv, _Ev, _Hash>::insert_from(const key_type& _k, edge_type* const _e) -> size_t {
    _in.insert({_k, _e});
    return _in.count(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, true, _Vv, _Ev, _Hash>::insert_to(const key_type& _k, edge_type* const _e) -> size_t {
    _out.insert({_k, _e});
    return _out.count(_k);
}

// vertex<false> public implement
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> template <typename... _Args>
vertex<_Vk, false, _Vv, _Ev, _Hash>::vertex(_Args&&... _args) : base(std::forward<_Args>(_args)...) {}
// vertex<false> private implement
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, false, _Vv, _Ev, _Hash>::insert_from(const key_type& _k, edge_type* const _e) -> size_t {
    _in[_k] = _e;
    return _in.count(_k);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> auto
vertex<_Vk, false, _Vv, _Ev, _Hash>::insert_to(const key_type& _k, edge_type* const _e) -> size_t {
    _out[_k] = _e;
    return _out.count(_k);
}

template <typename _Ev, typename _Vk, bool _Multi, typename _Vv, typename _Hash> template <typename... _Args> edge<_Ev, _Vk, _Multi, _Vv, _Hash>::
edge(const key_type& _xk, vertex_type* _x, const key_type& _yk, vertex_type* _y, _Args&&... _args) : 
base(std::forward<_Args>(_args)...), _in(_xk, _x), _out(_yk, _y) {}


template <typename _Vk, typename _Vv = void, typename _Ev = void, typename _Hash = std::hash<_Vk>, typename _Alloc = std::allocator<_Vk>> struct graph;
template <typename _Vk, typename _Vv = void, typename _Ev = void, typename _Hash = std::hash<_Vk>, typename _Alloc = std::allocator<_Vk>> struct multigraph;

namespace {
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> struct graph_base;
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc>
struct graph_base : public alloc<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc> {
public:
    using vertex_type = vertex<_Vk, _Multi, _Vv, _Ev, _Hash>;
    using edge_type = edge<_Ev, _Vk, _Multi, _Vv, _Hash>;
    using key_type = typename vertex_type::key_type;
    template <typename _R> using vertex_visitor = std::function<_R(const vertex_type&)>;
    template <typename _R> using edge_visitor = std::function<_R(const edge_type&)>;
    template <typename _R> using vertex_modifier = std::function<_R(vertex_type&)>;
    template <typename _R> using edge_modifier = std::function<_R(edge_type&)>;
protected:
    graph_base() = default;
    graph_base(const graph_base&) {} // implement in derived class
    graph_base(graph_base&&) {} // implement in derived class
public:
    virtual ~graph_base() { clear(); }
public:
    /**
     * @return the number of vertices
     */
    inline auto order() const -> size_t { return _vertices.size(); }
    /**
     * @return the number of edges
     */
    inline auto size() const -> size_t { return _edges.size(); }
    /**
     * @return whether the %graph is empty
     */
    inline auto empty() const -> bool { return _vertices.empty(); }
    /**
     * @param _k the specific key
     * @return whether there is any element with the specific key
     */
    inline auto contains(const key_type& _k) const -> bool { return _vertices.contains(_k); }
    /**
     * @param _x the key of head vertex
     * @param _y the key of tail vertex
     * @return whether there is any edge between the specific keys
     */
    inline auto adjacent(const key_type& _x, const key_type& _y) const -> bool { return contains(_x) && contains(_y) && get_vertex(_x).contains_to(_y); }
    /**
     * @param _x the key of head vertex
     * @param _y the key of tail vertex
     * @return the number of edge between the specific keys
     */
    inline auto count(const key_type& _x, const key_type& _y) const -> size_t { return (contains(_x) && contains(_y)) ? get_vertex(_x).count_to(_y) : 0; }
    /**
     * @brief access vertex
     * @param _k the specific key
     * @throw std::out_of_range if the specific key not exists
     */
    inline auto get_vertex(const key_type& _k) -> vertex_type& { return *_vertices.at(_k); }
    /**
     * @brief access vertex
     * @param _k the specific key
     * @throw std::out_of_range if the specific key not exists
     */
    inline auto get_vertex(const key_type& _k) const -> const vertex_type& { return *_vertices.at(_k); }
    inline auto begin() { return _vertices.begin(); }
    inline auto end() { return _vertices.end(); }
    inline auto begin() const { return _vertices.begin(); }
    inline auto end() const { return _vertices.end(); }
    inline auto cbegin() const { return _vertices.cbegin(); }
    inline auto cend() const { return _vertices.cend(); }
    inline auto edge_begin() { return _edges.begin(); }
    inline auto edge_end() { return _edges.end(); }
    inline auto edge_begin() const { return _edges.begin(); }
    inline auto edge_end() const { return _edges.end(); }
    inline auto edge_cbegin() const { return _edges.cbegin(); }
    inline auto edge_cend() const { return _edges.cend(); }
    /**
     * @brief insert vertex with specific key
     * @param _k the key of the vertex
     * @param _args the constructor arguments of the vertex
     */
    template <typename... _Args> auto insert(const key_type& _k, _Args&&... _args) -> void {
        if (contains(_k)) { return; }
        vertex_type* const _v = this->_M_allocate_vertex(std::forward<_Args>(_args)...);
        _vertices[_k] = _v;
    }
    /**
     * @brief erase vertex with specific key
     * @param _k the key of the vertex
     */
    auto erase(const key_type& _k) -> void {
        if (!contains(_k)) { return; }
        disconnect(_k);
        this->_M_deallocate_vertex(&get_vertex(_k));
        _vertices.erase(_k);
    }
    /**
     * @brief disconnect vertex with all vertices
     * @param _k the key of the vertex
     */
    auto disconnect(const key_type& _k) -> void {
        if (!contains(_k)) { return; }
        vertex_type& _vertex = get_vertex(_k);
        // erase edges, update other vertices
        const auto _ins = _vertex.in();
        for (auto _in = _ins.first; _in != _ins.second; ++_in) {
            get_vertex(_in->first).erase_to(_k);
            _edges.erase(_in->second);
            this->_M_deallocate_edge(_in->second);
        }
        const auto _outs = _vertex.out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            get_vertex(_out->first).erase_from(_k);
            _edges.erase(_out->second);
            this->_M_deallocate_edge(_out->second);
        }
        // update vertex
        _vertex.clear();
    }
    auto disconnect(const key_type& _x, const key_type& _y) -> void {
        if (!contains(_x) || !contains(_y)) { return; }
        this->for_each(_x, _y, [this](edge_type& _e) {
            this->_edges.erase(std::addressof(_e));
            this->_M_deallocate_edge(std::addressof(_e));
        });
        this->get_vertex(_x).erase_to(_y);
        this->get_vertex(_y).erase_from(_x);
    }
    auto clear() -> void {
        for (auto [_k, _v] : _vertices) { this->_M_deallocate_vertex(_v); }
        for (auto _e : _edges) { this->_M_deallocate_edge(_e); }
        _vertices.clear(); _edges.clear();
    }
public: // algorithm
    /**
     * @brief dijkstra algorithm
     * @param _k start vertex key
     * @param _visitor edge visitor, _R(*)(const edge_type&)
     * @return predecessor edge for each vertex (exclude unreachable vertices)
     * (edge[i][j] == k -> shortest path from i to j via k, k may be equal to i)
     * @note can't handle negative weight edge
     */
    template <typename _R> auto dijkstra(const key_type& _k, edge_visitor<_R>&& _visitor) const -> std::unordered_map<key_type, std::pair<_R, key_type>>;
    /**
     * @brief floyd algorithm
     * @param _visitor edge visitor, _R(*)(const edge_type&)
     * @return intermediate vertex between any two vertices
     * (edge[i][j] == k -> shortest path from i to j via k, k may be equal to i)
     * @note can't handle negative weight loop
     */
    template <typename _R> auto floyd(edge_visitor<_R>&& _visitor) const -> icy::graph<key_type, void, std::pair<_R, key_type>>;
    auto bfs(const key_type& _k, vertex_modifier<void>&& _visitor) -> size_t;
    auto bfs(const key_type& _k, vertex_visitor<void>&& _visitor) const -> size_t;
    auto dfs(const key_type& _k, vertex_modifier<void>&& _visitor) -> size_t;
    auto dfs(const key_type& _k, vertex_visitor<void>&& _visitor) const -> size_t;
protected:
    virtual auto for_each(const key_type& _x, const key_type& _y, edge_modifier<void>&& _modifier) -> void = 0;
    virtual auto for_each(const key_type& _x, const key_type& _y, edge_visitor<void>&& _visitor) const -> void = 0;
protected:
    std::unordered_map<key_type, vertex_type*, _Hash> _vertices;
    std::unordered_set<edge_type*> _edges;
};

/// algorithm implement
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename _R> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::dijkstra(const key_type& _k, edge_visitor<_R>&& _visitor) const -> std::unordered_map<key_type, std::pair<_R, key_type>> {
    using cost_type = _R;
    static_assert(std::is_arithmetic<cost_type>::value);
    if (!this->contains(_k)) { return {}; }
    // _s = {_k}, _u = all - {_k}
    std::vector<std::pair<cost_type, key_type>> _costs; // cost from `_s` to `_u`
    _costs.reserve(this->order() - 1);
    for (const auto& [_key, _vertex] : this->_vertices) {
        if (_k == _key) { continue; }
        _costs.emplace_back(std::numeric_limits<cost_type>::max(), _key);
    }
    std::unordered_map<key_type, std::pair<_R, key_type>> _predecessor;
    const vertex_type* _vertex = &this->get_vertex(_k);
    key_type _key = _k;
    while (!_costs.empty()) {
        // update `_costs` and `_predecessor`
        for (auto _i = _costs.begin(); _i != _costs.end(); ++_i) {
            this->for_each(_key, _i->second, [_i, &_key, &_visitor, &_predecessor](const edge_type& _e) {
                const cost_type _cost = _visitor(_e);
                if (_i->first > _cost) {
                    _i->first = _cost;
                    _predecessor[_i->second] = std::make_pair(_cost, _key);
                }
            });
        }
        // choose lowest one (check max)
        const auto _lowest = std::min_element(_costs.cbegin(), _costs.cend(), [](const auto& _x, const auto& _y) {
            return _x.first < _y.first;
        });
        if (_lowest == _costs.cend() || _lowest->first == std::numeric_limits<cost_type>::max()) {
            break;
        }
        // update `_key`, erase from `_costs`
        _key = _lowest->second;
        _costs.erase(_lowest);
    }
    return _predecessor;
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename _R> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::floyd(edge_visitor<_R>&& _visitor) const -> icy::graph<key_type, void, std::pair<_R, key_type>> {
    using cost_type = _R;
    static_assert(std::is_arithmetic<cost_type>::value);
    graph<key_type, void, std::pair<_R, key_type>> _intermediary;
    graph<key_type, void, cost_type> _costs; // i->i cost, i->j cost
    for (const auto& [_k, _v] : this->_vertices) {
        _intermediary.insert(_k);
        // _costs.insert(_k);
    }
    auto get_cost = [&_intermediary, this](const key_type& _i, const key_type& _j) -> cost_type {
        assert(this->contains(_i) && this->contains(_j));
        if (_i == _j) { return 0; }
        else if (!_intermediary.adjacent(_i, _j)) { return std::numeric_limits<cost_type>::max(); }
        else { return _intermediary.get_edge(_i, _j)->value().first; }
    };
    auto set_intermediary = [&_intermediary, this](const key_type& _i, const key_type& _j, cost_type _cost, const key_type& _k) -> void {
        assert(this->contains(_i) && this->contains(_j) && _i != _j);
        if (!_intermediary.adjacent(_i, _j)) { _intermediary.connect(_i, _j, _cost, _k); }
        else { _intermediary.get_edge(_i, _j)->set_value(std::make_pair(_cost, _k)); }
    };
    for (const edge_type* _e : this->_edges) {
        const key_type& _ik = _e->in_key();
        const key_type& _ok = _e->out_key();
        const cost_type _cost = _visitor(*_e);
        if (get_cost(_ik, _ok) > _cost) {
            set_intermediary(_ik, _ok, _cost, _ik); //for graph and multigraph
        }
    }
    auto greater_than = [](cost_type _s, cost_type _a, cost_type _b) -> bool { // whether `_s > _a + _b`
        if (_s > _a) { return _s - _a > _b; }
        if (_s > _b) { return _s - _b > _a; }
        return false;
    };
    for (const auto& [_k, _kv] : this->_vertices) {
        for (const auto& [_i, _iv] : this->_vertices) {
            if (_i == _k) { continue; }
            for (const auto& [_j, _jv] : this->_vertices) {
                if (_i == _j || _j == _k) continue;
                const cost_type _cost_ij = get_cost(_i, _j);
                const cost_type _cost_ik = get_cost(_i, _k);
                const cost_type _cost_kj = get_cost(_k, _j);
                // if (_cost_ij > _cost_ik + _cost_kj) { // overflow
                if (greater_than(_cost_ij, _cost_ik, _cost_kj)) {
                    // assert(_i != _k && _j != _k);
                    set_intermediary(_i, _j, _cost_ik + _cost_kj, _k);
                }
            }
        }
    }
    return _intermediary;
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::bfs(const key_type& _k, vertex_modifier<void>&& _modifier) -> size_t {
    if (!contains(_k)) { return 0; }
    std::queue<vertex_type*> _q;
    std::unordered_set<vertex_type*> _s; // record vertex whether in queue before
    _q.push(&get_vertex(_k));
    _s.insert(_q.front());
    size_t _cnt = 0;
    while (!_q.empty()) {
        vertex_type* const _vertex = _q.front(); _q.pop();
        _modifier(*_vertex); ++_cnt;
        const auto _outs = _vertex->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            vertex_type* const _v = &get_vertex(_out->first);
            if (_s.contains(_v)) { continue; }
            _q.push(_v);
            _s.insert(_v);
        }
    }
    return _cnt;
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::bfs(const key_type& _k, vertex_visitor<void>&& _visitor) const -> size_t {
    if (!contains(_k)) { return 0; }
    std::queue<const vertex_type*> _q;
    std::unordered_set<const vertex_type*> _s; // record vertex whether in queue before
    _q.push(&get_vertex(_k));
    _s.insert(_q.front());
    size_t _cnt = 0;
    while (!_q.empty()) {
        const vertex_type* const _vertex = _q.front(); _q.pop();
        _visitor(*_vertex); ++_cnt;
        const auto _outs = _vertex->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            const vertex_type* const _v = &get_vertex(_out->first);
            if (_s.contains(_v)) { continue; }
            _q.push(_v);
            _s.insert(_v);
        }
    }
    return _cnt;
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::dfs(const key_type& _k, vertex_modifier<void>&& _modifier) -> size_t {
    if (!contains(_k)) { return 0; }
    std::vector<vertex_type*> _vec;
    std::unordered_set<vertex_type*> _s; // record vertex whether in stack before
    _vec.push_back(&get_vertex(_k));
    _s.insert(_vec.back());
    size_t _cnt = 0;
    while (!_vec.empty()) {
        vertex_type* const _vertex = _vec.back(); _vec.pop_back();
        _modifier(*_vertex); ++_cnt;
        const auto _outs = _vertex->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            vertex_type* const _v = &get_vertex(_out->first);
            if (_s.contains(_v)) { continue; }
            _vec.push_back(_v);
            _s.insert(_v);
        }
    }
    return _cnt;
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::dfs(const key_type& _k, vertex_visitor<void>&& _visitor) const -> size_t {
    if (!contains(_k)) { return 0; }
    std::vector<const vertex_type*> _vec;
    std::unordered_set<const vertex_type*> _s; // record vertex whether in stack before
    _vec.push_back(&get_vertex(_k));
    _s.insert(_vec.back());
    size_t _cnt = 0;
    while (!_vec.empty()) {
        const vertex_type* const _vertex = _vec.back(); _vec.pop_back();
        _modifier(*_vertex); ++_cnt;
        const auto _outs = _vertex->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            const vertex_type* const _v = &get_vertex(_out->first);
            if (_s.contains(_v)) { continue; }
            _vec.push_back(_v);
            _s.insert(_v);
        }
    }
    return _cnt;
}
}

/**
 * @brief simple directed graph
 * @note no multi edge, no loop
 * @tparam _Vk vertex key
 * @tparam _Vv vertex value (`void` by default)
 * @tparam _Ev edge value (`void` by default)
 */
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc>
struct graph : public graph_base<_Vk, false, _Vv, _Ev, _Hash, _Alloc> {
public:
    using base = graph_base<_Vk, false, _Vv, _Ev, _Hash, _Alloc>;
    using key_type = typename base::key_type;
    using vertex_type = typename base::vertex_type;
    using edge_type = typename base::edge_type;
    template <typename _R> using vertex_visitor = typename base::vertex_visitor<_R>;
    template <typename _R> using edge_visitor = typename base::edge_visitor<_R>;
    template <typename _R> using vertex_modifier = typename base::vertex_modifier<_R>;
    template <typename _R> using edge_modifier = typename base::edge_modifier<_R>;
public:
    graph() = default;
    graph(const graph&);
    auto operator=(const graph&) -> graph&;
    virtual ~graph() = default;
public:
    auto operator==(const graph&) const -> bool;
    auto operator!=(const graph&) const -> bool;
public:
    /**
     * @brief access edge
     * @param _x the key of head vertex
     * @param _y the key of tail vertex
     * @return pointer of the edge (nullptr if the edge not exist)
     */
    auto get_edge(const key_type& _x, const key_type& _y) -> edge_type*;
    /**
     * @brief access edge
     * @param _x the key of head vertex
     * @param _y the key of tail vertex
     * @return pointer of the edge (nullptr if the edge not exist)
     */
    auto get_edge(const key_type& _x, const key_type& _y) const -> const edge_type*;
    /**
     * @brief connect the specific vertices
     * @param _x the key of head vertex
     * @param _y the key of tail vertex
     * @param _args the constructor arguments of the edge
     */
    template <typename... _Args> auto connect(const key_type& _x, const key_type& _y, _Args&&... _args) -> void;
    /**
     * @brief bidirectionally connect the specific vertices 
     * @param _x the key of one vertex
     * @param _y the key of the other vertex
     * @param _args the constructor arguments of the edge
     */
    template <typename... _Args> auto biconnect(const key_type& _x, const key_type& _y, _Args&&... _args) -> void;
private:
    auto _M_assign(const graph&) -> void;
    auto for_each(const key_type& _x, const key_type& _y, edge_modifier<void>&& _modifier) -> void override;
    auto for_each(const key_type& _x, const key_type& _y, edge_visitor<void>&& _visitor) const -> void override;
};
/**
 * @brief multi directed graph
 * @tparam _Vk vertex key
 * @tparam _Vv vertex value (`void` by default)
 * @tparam _Ev edge value (`void` by default)
 */
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc>
struct multigraph : public graph_base<_Vk, true, _Vv, _Ev, _Hash, _Alloc> {
public:
    using base = graph_base<_Vk, true, _Vv, _Ev, _Hash, _Alloc>;
    using key_type = typename base::key_type;
    using vertex_type = typename base::vertex_type;
    using edge_type = typename base::edge_type;
    template <typename _R> using vertex_visitor = typename base::vertex_visitor<_R>;
    template <typename _R> using edge_visitor = typename base::edge_visitor<_R>;
    template <typename _R> using vertex_modifier = typename base::vertex_modifier<_R>;
    template <typename _R> using edge_modifier = typename base::edge_modifier<_R>;
private:
    using iterator = typename vertex_type::iterator;
    using const_iterator = typename vertex_type::const_iterator;
public:
    multigraph() = default;
    template <typename __Hash, typename __Alloc> multigraph(const graph<_Vk, _Vv, _Ev, __Hash, __Alloc>&);
    multigraph(const multigraph&);
    auto operator=(const multigraph&) -> multigraph&;
    virtual ~multigraph() = default;
public:
    auto operator==(const multigraph&) const -> bool;
    auto operator!=(const multigraph&) const -> bool;
public:
    /**
     * @brief access edge
     * @param _x the key of head vertex
     * @param _y the key of tail vertex
     * @return pair of the edge iterator <key_type, edge_type*>
     */
    auto get_edge(const key_type&, const key_type&) -> std::pair<iterator, iterator>;
    /**
     * @brief access edge
     * @param _x the key of head vertex
     * @param _y the key of tail vertex
     * @return pair of the edge iterator <key_type, edge_type*>
     */
    auto get_edge(const key_type&, const key_type&) const -> std::pair<const_iterator, const_iterator>;
    /**
     * @brief connect the specific vertices
     * @param _x the key of head vertex
     * @param _y the key of tail vertex
     * @param _args the constructor arguments of the edge
     */
    template <typename... _Args> auto connect(const key_type&, const key_type&, _Args&&...) -> void;
    /**
     * @brief bidirectionally connect the specific vertices 
     * @param _x the key of one vertex
     * @param _y the key of the other vertex
     * @param _args the constructor arguments of the edge
     */
    template <typename... _Args> auto biconnect(const key_type&, const key_type&, _Args&&...) -> void;
private:
    auto _M_assign(const multigraph&) -> void;
    auto for_each(const key_type& _x, const key_type& _y, edge_modifier<void>&& _modifier) -> void override;
    auto for_each(const key_type& _x, const key_type& _y, edge_visitor<void>&& _visitor) const -> void override;
};



template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc>
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::graph(const graph& _rhs) {
    _M_assign(_rhs);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::operator=(const graph& _rhs) -> graph& {
    if (&_rhs == this) { return *this; }
    this->clear();
    _M_assign(_rhs);
    return *this;
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::operator==(const graph& _rhs) const -> bool {
    if (this->order() != _rhs.order() || this->size() != _rhs.size()) {
        return false;
    }
    std::queue<key_type> _remains;
    for (const auto& [_k, _v] : _rhs._vertices) {
        if (!this->contains(_k)) {
            return false;
        }
        const vertex_type& _vertex = this->get_vertex(_k);
        const vertex_type& _rhs_vertex = _rhs.get_vertex(_k);
        if (_vertex != _rhs_vertex || _vertex.indegree() != _rhs_vertex.indegree() || _vertex.outdegree() != _rhs_vertex.outdegree()) {
            return false;
        }
        _remains.push(_k);
    }
    while (!_remains.empty()) {
        const key_type& _k = _remains.front(); _remains.pop();
        const auto _outs = _rhs.get_vertex(_k).out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            const edge_type* _e = this->get_edge(_k, _out->first);
            if (_e == nullptr || *_e != *_out->second) {
                return false;
            }
        }
    }
    return true;
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::operator!=(const graph& _rhs) const -> bool {
    return !this->operator==(_rhs);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::get_edge(const key_type& _x, const key_type& _y) -> edge_type* {
    return this->get_vertex(_x).edge_to(_y);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::get_edge(const key_type& _x, const key_type& _y) const -> const edge_type* {
    return this->get_vertex(_x).edge_to(_y);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename... _Args> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::connect(const key_type& _x, const key_type& _y, _Args&&... _args) -> void {
    if (!this->contains(_x) || !this->contains(_y) || _x == _y || this->adjacent(_x, _y)) { return; }
    vertex_type& _from = this->get_vertex(_x);
    vertex_type& _to = this->get_vertex(_y);
    edge_type* const _e = this->_M_allocate_edge(_x, &_from, _y, &_to, std::forward<_Args>(_args)...);
    this->_edges.insert(_e);
    _from.insert_to(_y, _e);
    _to.insert_from(_x, _e);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename... _Args> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::biconnect(const key_type& _x, const key_type& _y, _Args&&... _args) -> void {
    this->connect(_x, _y, std::forward<_Args>(_args)...);
    this->connect(_y, _x, std::forward<_Args>(_args)...);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::_M_assign(const graph& _rhs) -> void {
    assert(this->empty());
    std::queue<key_type> _remains;
    for (const auto& [_k, _v] : _rhs._vertices) {
        _remains.push(_k);
    }
    while (!_remains.empty()) {
        const key_type& _k = _remains.front(); _remains.pop();
        if (!this->contains(_k)) {
            this->insert(_k, static_cast<const typename vertex_type::base&>(_rhs.get_vertex(_k)));
        }
        const auto _outs = _rhs.get_vertex(_k).out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (!this->contains(_out->first)) {
                this->insert(_out->first, static_cast<const typename vertex_type::base&>(_rhs.get_vertex(_out->first)));
            }
            this->connect(_k, _out->first, static_cast<const typename edge_type::base&>(*_out->second));
        }
    }
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::for_each(const key_type& _x, const key_type& _y, edge_modifier<void>&& _modifier) -> void {
    if (!this->contains(_x) || !this->contains(_y)) { return; }
    auto _edge = this->get_edge(_x, _y);
    if (_edge != nullptr) {
        _modifier(*_edge);
    }
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::for_each(const key_type& _x, const key_type& _y, edge_visitor<void>&& _visitor) const -> void {
    if (!this->contains(_x) || !this->contains(_y)) { return; }
    auto _edge = this->get_edge(_x, _y);
    if (_edge != nullptr) {
        _visitor(*_edge);
    }
}


template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename __Hash, typename __Alloc>
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::multigraph(const graph<_Vk, _Vv, _Ev, __Hash, __Alloc>& _rhs) {
    std::queue<key_type> _remains;
    for (const auto& [_k, _v] : _rhs) {
        _remains.push(_k);
    }
    while (!_remains.empty()) {
        const key_type& _k = _remains.front(); _remains.pop();
        if (!this->contains(_k)) {
            this->insert(_k, static_cast<const typename vertex_type::base&>(_rhs.get_vertex(_k)));
        }
        const auto _outs = _rhs.get_vertex(_k).out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (!this->contains(_out->first)) {
                this->insert(_out->first, static_cast<const typename vertex_type::base&>(_rhs.get_vertex(_out->first)));
            }
            this->connect(_k, _out->first, static_cast<const typename edge_type::base&>(*_out->second));
        }
    }
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc>
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::multigraph(const multigraph& _rhs) {
    _M_assign(_rhs);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::operator=(const multigraph& _rhs) -> multigraph& {
    if (&_rhs == this) { return *this; }
    this->clear();
    _M_assign(_rhs);
    return *this;
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::operator==(const multigraph& _rhs) const -> bool {
    if (this->order() != _rhs.order() || this->size() != _rhs.size()) {
        return false;
    }
    std::queue<key_type> _remains;
    for (const auto& [_k, _v] : _rhs._vertices) {
        if (!this->contains(_k)) {
            return false;
        }
        const vertex_type& _vertex = this->get_vertex(_k);
        const vertex_type& _rhs_vertex = _rhs.get_vertex(_k);
        if (_vertex != _rhs_vertex || _vertex.indegree() != _rhs_vertex.indegree() || _vertex.outdegree() != _rhs_vertex.outdegree()) {
            return false;
        }
        _remains.push(_k);
    }
    while (!_remains.empty()) {
        const key_type& _k = _remains.front(); _remains.pop();
        const auto _outs = _rhs.get_vertex(_k).out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            const auto _edges = this->get_edge(_k, _out->first);
            if (std::none_of(_edges.first, _edges.second, [_out](const auto& _iter) {
                return *_out->second == *_iter.second;
            })) {
                return false;
            }
        }
    }
    return true;
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::operator!=(const multigraph& _rhs) const -> bool {
    return !this->operator==(_rhs);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::get_edge(const key_type& _x, const key_type& _y) -> std::pair<iterator, iterator> {
    return this->get_vertex(_x).edge_to(_y);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::get_edge(const key_type& _x, const key_type& _y) const -> std::pair<const_iterator, const_iterator> {
    return this->get_vertex(_x).edge_to(_y);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename... _Args> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::connect(const key_type& _x, const key_type& _y, _Args&&... _args) -> void {
    if (!this->contains(_x) || !this->contains(_y)) { return; }
    vertex_type& _from = this->get_vertex(_x);
    vertex_type& _to = this->get_vertex(_y);
    edge_type* const _e = this->_M_allocate_edge(_x, &_from, _y, &_to, std::forward<_Args>(_args)...);
    this->_edges.insert(_e);
    _from.insert_to(_y, _e);
    _to.insert_from(_x, _e);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename... _Args> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::biconnect(const key_type& _x, const key_type& _y, _Args&&... _args) -> void {
    this->connect(_x, _y, std::forward<_Args>(_args)...);
    this->connect(_y, _x, std::forward<_Args>(_args)...);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::_M_assign(const multigraph& _rhs) -> void {
    assert(this->empty());
    std::queue<key_type> _remains;
    for (const auto& [_k, _v] : _rhs._vertices) {
        _remains.push(_k);
    }
    while (!_remains.empty()) {
        const key_type& _k = _remains.front(); _remains.pop();
        if (!this->contains(_k)) {
            this->insert(_k, static_cast<const typename vertex_type::base&>(_rhs.get_vertex(_k)));
        }
        const auto _outs = _rhs.get_vertex(_k).out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (!this->contains(_out->first)) {
                this->insert(_out->first, static_cast<const typename vertex_type::base&>(_rhs.get_vertex(_out->first)));
            }
            this->connect(_k, _out->first, static_cast<const typename edge_type::base&>(*_out->second));
        }
    }
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::for_each(const key_type& _x, const key_type& _y, edge_modifier<void>&& _modifier) -> void {
    if (!this->contains(_x) || !this->contains(_y)) { return; }
    const auto _range = this->get_edge(_x, _y);
    for (auto _e = _range.first; _e != _range.second; ++_e) {
        _modifier(*_e->second);
    }
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::for_each(const key_type& _x, const key_type& _y, edge_visitor<void>&& _visitor) const -> void {
    if (!this->contains(_x) || !this->contains(_y)) { return; }
    const auto _range = this->get_edge(_x, _y);
    for (auto _e = _range.first; _e != _range.second; ++_e) {
        _visitor(*_e->second);
    }
}

}

#endif // _ICY_GRAPH_HPP_