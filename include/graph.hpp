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
/**
 * @return whether `_s > _a + _b`, no overflow
 */
template <typename _Tp> auto greater_than(_Tp _s, _Tp _a, _Tp _b) -> bool {
    static_assert(std::is_arithmetic<_Tp>::value);
    if (_s > _a) { return _s - _a > _b; }
    if (_s > _b) { return _s - _b > _a; }
    return false;
};
}

namespace __details__::graph {
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
namespace __details__::graph {
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> struct graph_base;
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Cost, typename _Hash, typename _Alloc> struct dijkstra;
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Cost, typename _Hash, typename _Alloc> struct floyd;
}
template <typename _Vk, typename _Vv = void, typename _Ev = void, typename _Hash = std::hash<_Vk>, typename _Alloc = std::allocator<_Vk>> struct graph;
template <typename _Vk, typename _Vv = void, typename _Ev = void, typename _Hash = std::hash<_Vk>, typename _Alloc = std::allocator<_Vk>> struct multigraph;

namespace __details__::graph {
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

template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> struct vertex<_Vk, true, _Vv, _Ev, _Hash> : public __details__::graph::storage<_Vv> {
public:
    using base = __details__::graph::storage<_Vv>;
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
    template <typename __Vk, bool __Multi, typename __Vv, typename __Ev, typename __Hash, typename __Alloc> friend struct __details__::graph::graph_base;
    template <typename __Vk, typename __Vv, typename __Ev, typename __Hash, typename __Alloc> friend struct graph;
    template <typename __Vk, typename __Vv, typename __Ev, typename __Hash, typename __Alloc> friend struct multigraph;
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
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash> struct vertex<_Vk, false, _Vv, _Ev, _Hash> : public __details__::graph::storage<_Vv> {
public:
    using base = __details__::graph::storage<_Vv>;
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
    template <typename __Vk, bool _Multi, typename __Vv, typename __Ev, typename __Hash, typename _Alloc> friend struct __details__::graph::graph_base;
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
template <typename _Ev, typename _Vk, bool _Multi, typename _Vv, typename _Hash> struct edge : public __details__::graph::storage<_Ev> {
public:
    using base = __details__::graph::storage<_Ev>;
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


namespace __details__::graph {
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc>
struct graph_base : public alloc<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc> {
public:
    using vertex_type = vertex<_Vk, _Multi, _Vv, _Ev, _Hash>;
    using edge_type = edge<_Ev, _Vk, _Multi, _Vv, _Hash>;
    using key_type = typename vertex_type::key_type;
    template <typename _R> using vertex_visitor = std::function<_R(const key_type&, const vertex_type&)>;
    template <typename _R> using edge_visitor = std::function<_R(const edge_type&)>;
    template <typename _R> using vertex_modifier = std::function<_R(const key_type&, vertex_type&)>;
    template <typename _R> using edge_modifier = std::function<_R(edge_type&)>;
    template <typename __Vk, bool __Multi, typename __Vv, typename __Ev, typename __Cost, typename __Hash, typename __Alloc> friend struct dijkstra;
    template <typename __Vk, bool __Multi, typename __Vv, typename __Ev, typename __Cost, typename __Hash, typename __Alloc> friend struct floyd;
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
    inline auto adjacent(const key_type& _x, const key_type& _y) const -> bool { return contains(_x) && contains(_y) && get_vertex(_x)->contains_to(_y); }
    /**
     * @param _x the key of head vertex
     * @param _y the key of tail vertex
     * @return the number of edge between the specific keys
     */
    inline auto count(const key_type& _x, const key_type& _y) const -> size_t { return (contains(_x) && contains(_y)) ? get_vertex(_x)->count_to(_y) : 0; }
    /**
     * @brief access vertex
     * @param _k the specific key
     * @return vertex pointer
     */
    inline auto get_vertex(const key_type& _k) -> vertex_type* { return _vertices.contains(_k) ? _vertices.at(_k) : nullptr; }
    /**
     * @brief access vertex
     * @param _k the specific key
     * @return vertex pointer
     */
    inline auto get_vertex(const key_type& _k) const -> const vertex_type* { return _vertices.contains(_k) ? _vertices.at(_k) : nullptr; }
    /**
     * @brief unordered_map<key_type, vertex_type*>
     */
    inline auto& vertices() const { return _vertices; }
    /**
     * @brief unordered_set<edge_type*>
     */
    inline auto& edges() const { return _edges; }
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
        this->_M_deallocate_vertex(get_vertex(_k));
        _vertices.erase(_k);
    }
    /**
     * @brief disconnect vertex with all vertices
     * @param _k the key of the vertex
     */
    auto disconnect(const key_type& _k) -> void {
        if (!contains(_k)) { return; }
        vertex_type* _vertex = get_vertex(_k);
        // erase edges, update other vertices
        const auto _ins = _vertex->in();
        for (auto _in = _ins.first; _in != _ins.second; ++_in) {
            get_vertex(_in->first)->erase_to(_k);
            _edges.erase(_in->second);
            this->_M_deallocate_edge(_in->second);
        }
        const auto _outs = _vertex->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            get_vertex(_out->first)->erase_from(_k);
            _edges.erase(_out->second);
            this->_M_deallocate_edge(_out->second);
        }
        // update vertex
        _vertex->clear();
    }
    auto disconnect(const key_type& _x, const key_type& _y) -> void {
        if (!contains(_x) || !contains(_y)) { return; }
        this->for_each(_x, _y, [this](edge_type& _e) {
            this->_edges.erase(std::addressof(_e));
            this->_M_deallocate_edge(std::addressof(_e));
        });
        this->get_vertex(_x)->erase_to(_y);
        this->get_vertex(_y)->erase_from(_x);
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
     * @return dijkstra result wrapper
     * @throw out_of_range if the key not exists
     * @note can't handle negative weight edge
     */
    template <typename _R> auto dijkstra(const key_type& _k, edge_visitor<_R>&& _visitor) const -> graph::dijkstra<_Vk, _Multi, _Vv, _Ev, _R, _Hash, _Alloc>;
    /**
     * @brief floyd algorithm
     * @tparam _R cost type
     * @param _visitor edge visitor, _R(*)(const edge_type&)
     * @return floyd result wrapper
     * @note can't handle negative weight loop
     */
    template <typename _R> auto floyd(edge_visitor<_R>&& _visitor) const -> graph::floyd<_Vk, _Multi, _Vv, _Ev, _R, _Hash, _Alloc>;
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
/**
 * @brief dijkstra result wrapper
 */
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Cost, typename _Hash, typename _Alloc> struct dijkstra {
public:
    using base = graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>;
    using vertex_type = typename base::vertex_type;
    using edge_type = typename base::edge_type;
    using key_type = typename base::key_type;
    using cost_type = _Cost;
    using trailer = std::function<void(const key_type&)>;
    template <typename __Vk, bool __Multi, typename __Vv, typename __Ev, typename __Hash, typename __Alloc> friend struct graph_base;
private:
    dijkstra(const base&, const key_type& _k, typename base::template edge_visitor<cost_type>&&);
public:
    dijkstra(const dijkstra&) = default;
    dijkstra(dijkstra&&) = default;
    auto operator=(const dijkstra&) -> dijkstra& = delete;
    auto operator=(dijkstra&&) -> dijkstra& = delete;
    ~dijkstra() = default;
public:
    /**
     * @brief process each vertex in trail (including begin` and end vertex)
     * @param _x end vertex key
     * @param _trailer vertex handler, void(*)(const key_type&)
     * @throw out_of_range if the key not exists
     * @note T(n) = O(n)
     */
    auto trail(const key_type& _x, trailer&& _trailer) const -> void;
    /**
     * @brief cost from `begin` to `end`
     * @param _x end vertex key
     * @throw out_of_range if the key not exists
     * @note T(n) = O(1)
     */
    auto cost(const key_type& _x) const -> cost_type;
private:
    const base& _g;
    /**
     * @brief predecessor edge for each vertex (exclude unreachable vertices)
     * @note edge[i][j] == k -> shortest path from i to j via k, k may be equal to i
     */
    std::unordered_map<key_type, std::pair<cost_type, key_type>> _predecessor;
    const key_type _k;
};
/**
 * @brief floyd result wrapper
 */
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Cost, typename _Hash, typename _Alloc> struct floyd {
public:
    using base = graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>;
    using vertex_type = typename base::vertex_type;
    using edge_type = typename base::edge_type;
    using key_type = typename base::key_type;
    using cost_type = _Cost;
    using trailer = std::function<void(const key_type&)>;
    template <typename __Vk, bool __Multi, typename __Vv, typename __Ev, typename __Hash, typename __Alloc> friend struct graph_base;
private:
    floyd(const base&, typename base::template edge_visitor<cost_type>&&);
public:
    floyd(const floyd&) = default;
    floyd(floyd&&) = default;
    auto operator=(const floyd&) -> floyd& = delete;
    auto operator=(floyd&&) -> floyd& = delete;
    ~floyd() = default;
public:
    /**
     * @brief process each vertex in trail (including begin` and end vertex)
     * @param _x begin vertex key
     * @param _y end vertex key
     * @param _trailer vertex handler, void(*)(const key_type&)
     * @throw out_of_range if the key not exists
     * @note T(n) = O(n)
     */
    auto trail(const key_type& _x, const key_type& _y, trailer&& _trailer) const -> void;
    /**
     * @brief cost from `begin` to `end`
     * @param _x begin vertex key
     * @param _y end vertex key
     * @retval 0 if _x == _y
     * @throw out_of_range if the key not exists
     * @note T(n) = O(1)
     */
    auto cost(const key_type& _x, const key_type& _y) const -> cost_type;
private:
    const base& _g;
    /**
     * @brief intermediate vertex between any two vertices
     * @note edge[i][j] == k -> shortest path from i to j via k, k may be equal to i
     */
    icy::graph<key_type, void, std::pair<cost_type, key_type>> _intermediary;
};

/// algorithm implement
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename _R> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::dijkstra(const key_type& _k, edge_visitor<_R>&& _visitor) const -> graph::dijkstra<_Vk, _Multi, _Vv, _Ev, _R, _Hash, _Alloc> {
    static_assert(std::is_arithmetic<_R>::value);
    if (!this->contains(_k)) {
        throw std::out_of_range("graph_base::dijkstra");
    }
    return graph::dijkstra<_Vk, _Multi, _Vv, _Ev, _R, _Hash, _Alloc>(*this, _k, std::move(_visitor));
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename _R> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::floyd(edge_visitor<_R>&& _visitor) const -> graph::floyd<_Vk, _Multi, _Vv, _Ev, _R, _Hash, _Alloc> {
    static_assert(std::is_arithmetic<_R>::value);
    return graph::floyd<_Vk, _Multi, _Vv, _Ev, _R, _Hash, _Alloc>(*this, std::move(_visitor));
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::bfs(const key_type& _key, vertex_modifier<void>&& _modifier) -> size_t {
    if (!contains(_key)) { return 0; }
    std::queue<key_type> _q;
    std::unordered_set<key_type> _s; // record vertex whether in queue before
    _q.emplace(_key);
    _s.insert(_key);
    size_t _cnt = 0;
    while (!_q.empty()) {
        const key_type& _k = _q.front(); _q.pop();
        vertex_type* const _v = get_vertex(_k);
        _modifier(_k, *_v); ++_cnt;
        const auto _outs = _v->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (_s.contains(_out->first)) { continue; }
            _q.emplace(_out->first);
            _s.insert(_out->first);
        }
    }
    return _cnt;
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::bfs(const key_type& _key, vertex_visitor<void>&& _visitor) const -> size_t {
    if (!contains(_key)) { return 0; }
    std::queue<key_type> _q;
    std::unordered_set<key_type> _s; // record vertex whether in queue before
    _q.emplace(_key);
    _s.insert(_key);
    size_t _cnt = 0;
    while (!_q.empty()) {
        const key_type& _k = _q.front(); _q.pop();
        const vertex_type* const _v = get_vertex(_k);
        _visitor(_k, *_v); ++_cnt;
        const auto _outs = _v->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (_s.contains(_out->first)) { continue; }
            _q.emplace(_out->first);
            _s.insert(_out->first);
        }
    }
    return _cnt;
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::dfs(const key_type& _key, vertex_modifier<void>&& _modifier) -> size_t {
    if (!contains(_key)) { return 0; }
    std::vector<key_type> _vec;
    std::unordered_set<key_type> _s; // record vertex whether in stack before
    _vec.push_back(_key);
    _s.insert(_key);
    size_t _cnt = 0;
    while (!_vec.empty()) {
        const key_type& _k = _vec.back(); _vec.pop_back();
        vertex_type* const _v = get_vertex(_k);
        _modifier(_k, *_v); ++_cnt;
        const auto _outs = _v->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (_s.contains(_out->first)) { continue; }
            _vec.push_back(_out->first);
            _s.insert(_out->first);
        }
    }
    return _cnt;
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph_base<_Vk, _Multi, _Vv, _Ev, _Hash, _Alloc>::dfs(const key_type& _key, vertex_visitor<void>&& _visitor) const -> size_t {
    if (!contains(_key)) { return 0; }
    std::vector<key_type> _vec;
    std::unordered_set<key_type> _s; // record vertex whether in stack before
    _vec.push_back(_key);
    _s.insert(_key);
    size_t _cnt = 0;
    while (!_vec.empty()) {
        const key_type& _k = _vec.back(); _vec.pop_back();
        const vertex_type* const _v = get_vertex(_k);
        _visitor(_k, *_v); ++_cnt;
        const auto _outs = _v->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (_s.contains(_out->first)) { continue; }
            _vec.push_back(_out->first);
            _s.insert(_out->first);
        }
    }
    return _cnt;
}

template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Cost, typename _Hash, typename _Alloc>
dijkstra<_Vk, _Multi, _Vv, _Ev, _Cost, _Hash, _Alloc>::
dijkstra(const base& _g, const key_type& _k, typename base::template edge_visitor<cost_type>&& _visitor) : _g(_g), _k(_k) {
    if (!_g.contains(_k)) { return; }
    // _s = {_k}, _u = all - {_k}
    using element_type = std::pair<cost_type, key_type>;
    auto greater_cost = [](const element_type& _x, const element_type& _y) -> bool {
        return _x.first > _y.first;
    };
    std::priority_queue<element_type, std::vector<element_type>, decltype(greater_cost)> _q(greater_cost);
    std::unordered_set<key_type> _u;
    _u.reserve(_g.order() - 1);
    for (const auto& [_key, _vertex] : _g.vertices()) {
        if (_k == _key) { continue; }
        _u.insert(_key);
        cost_type _c = std::numeric_limits<cost_type>::max();
        _g.for_each(_k, _key, [this, &_c, &_k, &_visitor](const edge_type& _e) {
            _c = std::min(_c, _visitor(_e));
            this->_predecessor[_e.out_key()] = std::make_pair(_c, _k);
        });
        _q.emplace(_c, _key);
    }
    auto get_cost = [this, &_k](const key_type& _x) -> cost_type { // cost from `_k` to `_x`
        if (_x == _k) { return 0; }
        if (this->_predecessor.contains(_x)) {
            return this->_predecessor.at(_x).first;
        }
        return std::numeric_limits<cost_type>::max();
    };
    /**
     * @brief set cost into `_predecessor`
     * @param _x the key
     * @param _pre the predecessor key of the `_x`
     * @param _c the cost from `_pre` to `_x`
     */
    auto set_cost = [this, &get_cost](const key_type& _x, const key_type& _pre, cost_type _c) -> void {
        this->_predecessor[_x] = std::make_pair(get_cost(_pre) + _c, _pre);
    };
    while (!_q.empty()) {
        const element_type _e = _q.top(); _q.pop();
        if (!_u.contains(_e.second)) { continue; }
        _u.erase(_e.second);
        if (_e.first == std::numeric_limits<cost_type>::max()) { break; }
        // update
        const auto _outs = _g.get_vertex(_e.second)->out();
        for (auto _i = _outs.first; _i != _outs.second; ++_i) { // _i -> <key_type, edge_type*>
            if (!_u.contains(_i->first)) { continue; }
            const cost_type _cost = _visitor(*_i->second); // cost from `_e.second` to `_i->first`
            if (greater_than(get_cost(_i->first), get_cost(_e.second), _cost)) {
                set_cost(_i->first, _e.second, _cost);
                _q.emplace(_cost, _i->first);
            }
        }
    }
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Cost, typename _Hash, typename _Alloc> auto
dijkstra<_Vk, _Multi, _Vv, _Ev, _Cost, _Hash, _Alloc>::trail(const key_type& _x, trailer&& _trailer) const -> void {
    if (!_g.contains(_x)) {
        throw std::out_of_range("dijkstra::trail(...)");
    }
    std::function<void(const key_type&)> _M_dijkstra_trail = [&_M_dijkstra_trail, this, _trailer] (const key_type& _x) -> void {
        if (this->_k == _x) {
            _trailer(this->_k);
            return;
        }
        if (!this->_predecessor.contains(_x)) { return; }
        _M_dijkstra_trail(this->_predecessor.at(_x).second);
        _trailer(_x);
    };
    _M_dijkstra_trail(_x);
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Cost, typename _Hash, typename _Alloc> auto
dijkstra<_Vk, _Multi, _Vv, _Ev, _Cost, _Hash, _Alloc>::cost(const key_type& _x) const -> cost_type {
    if (!_g.contains(_x)) {
        throw std::out_of_range("dijkstra::cost(...)");
    }
    if (_k == _x) { return 0; }
    if (_predecessor.contains(_x)) {
        return _predecessor.at(_x).first;
    }
    return std::numeric_limits<cost_type>::max();
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Cost, typename _Hash, typename _Alloc>
floyd<_Vk, _Multi, _Vv, _Ev, _Cost, _Hash, _Alloc>::
floyd(const base& _g, typename base::template edge_visitor<cost_type>&& _visitor) : _g(_g) {
    icy::graph<key_type, void, cost_type> _costs; // i->i cost, i->j cost
    for (const auto& [_k, _v] : _g.vertices()) {
        _intermediary.insert(_k);
        // _costs.insert(_k);
    }
    auto get_cost = [this, &_g](const key_type& _i, const key_type& _j) -> cost_type {
        assert(_g.contains(_i) && _g.contains(_j));
        if (_i == _j) { return 0; }
        else if (!this->_intermediary.adjacent(_i, _j)) { return std::numeric_limits<cost_type>::max(); }
        else { return this->_intermediary.get_edge(_i, _j)->value().first; }
    };
    auto set_intermediary = [this, &_g](const key_type& _i, const key_type& _j, cost_type _cost, const key_type& _k) -> void {
        assert(_g.contains(_i) && _g.contains(_j) && _i != _j);
        if (!this->_intermediary.adjacent(_i, _j)) { this->_intermediary.connect(_i, _j, _cost, _k); }
        else { this->_intermediary.get_edge(_i, _j)->set_value(std::make_pair(_cost, _k)); }
    };
    for (const edge_type* _e : _g.edges()) {
        const key_type& _ik = _e->in_key();
        const key_type& _ok = _e->out_key();
        const cost_type _cost = _visitor(*_e);
        if (get_cost(_ik, _ok) > _cost) {
            set_intermediary(_ik, _ok, _cost, _ik); //for graph and multigraph
        }
    }
    for (const auto& [_k, _kv] : _g.vertices()) {
        for (const auto& [_i, _iv] : _g.vertices()) {
            if (_i == _k) { continue; }
            for (const auto& [_j, _jv] : _g.vertices()) {
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
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Cost, typename _Hash, typename _Alloc> auto
floyd<_Vk, _Multi, _Vv, _Ev, _Cost, _Hash, _Alloc>::trail(const key_type& _x, const key_type& _y, trailer&& _trailer) const -> void {
    if (!_g.contains(_x) || !_g.contains(_y)) {
        throw std::out_of_range("floyd::trail(...)");
    }
    if (!_intermediary.contains(_x) || !_intermediary.contains(_y)) {
        return; // would not reach
    }
    if (_x == _y) {
        _trailer(_x);
        return;
    }
    if (!_intermediary.adjacent(_x, _y)) { return; }
    _trailer(_x);
    std::function<void(const _Vk&, const _Vk&, std::function<void(const _Vk&)>&&)> _M_floyd_trail =
    [&_M_floyd_trail, this] (const _Vk& _x, const _Vk& _y, std::function<void(const _Vk&)>&& _trailer) -> void {
        const _Vk& _k = this->_intermediary.get_edge(_x, _y)->value().second;
        if (_x == _k) { _trailer(_y); return; }
        _M_floyd_trail(_x, _k, std::move(_trailer));
        _M_floyd_trail(_k, _y, std::move(_trailer));
    };
    _M_floyd_trail(_x, _y, std::move(_trailer));
}
template <typename _Vk, bool _Multi, typename _Vv, typename _Ev, typename _Cost, typename _Hash, typename _Alloc> auto
floyd<_Vk, _Multi, _Vv, _Ev, _Cost, _Hash, _Alloc>::cost(const key_type& _x, const key_type& _y) const -> cost_type {
    if (!_g.contains(_x) || !_g.contains(_y)) {
        throw std::out_of_range("floyd::cost(...)");
    }
    if (_x == _y) { return 0; }
    if (_intermediary.adjacent(_x, _y)) {
        return _intermediary.get_edge(_x, _y)->value().first;
    }
    return std::numeric_limits<cost_type>::max();
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
struct graph : public __details__::graph::graph_base<_Vk, false, _Vv, _Ev, _Hash, _Alloc> {
public:
    using base = __details__::graph::graph_base<_Vk, false, _Vv, _Ev, _Hash, _Alloc>;
    using key_type = typename base::key_type;
    using vertex_type = typename base::vertex_type;
    using edge_type = typename base::edge_type;
    template <typename _R> using vertex_visitor = typename base::template vertex_visitor<_R>;
    template <typename _R> using edge_visitor = typename base::template edge_visitor<_R>;
    template <typename _R> using vertex_modifier = typename base::template vertex_modifier<_R>;
    template <typename _R> using edge_modifier = typename base::template edge_modifier<_R>;
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
struct multigraph : public __details__::graph::graph_base<_Vk, true, _Vv, _Ev, _Hash, _Alloc> {
public:
    using base = __details__::graph::graph_base<_Vk, true, _Vv, _Ev, _Hash, _Alloc>;
    using key_type = typename base::key_type;
    using vertex_type = typename base::vertex_type;
    using edge_type = typename base::edge_type;
    template <typename _R> using vertex_visitor = typename base::template vertex_visitor<_R>;
    template <typename _R> using edge_visitor = typename base::template edge_visitor<_R>;
    template <typename _R> using vertex_modifier = typename base::template vertex_modifier<_R>;
    template <typename _R> using edge_modifier = typename base::template edge_modifier<_R>;
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
        const vertex_type* _vertex = this->get_vertex(_k);
        const vertex_type* _rhs_vertex = _rhs.get_vertex(_k);
        if (*_vertex != *_rhs_vertex || _vertex->indegree() != _rhs_vertex->indegree() || _vertex->outdegree() != _rhs_vertex->outdegree()) {
            return false;
        }
        _remains.push(_k);
    }
    while (!_remains.empty()) {
        const key_type& _k = _remains.front(); _remains.pop();
        const auto _outs = _rhs.get_vertex(_k)->out();
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
    return this->get_vertex(_x)->edge_to(_y);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::get_edge(const key_type& _x, const key_type& _y) const -> const edge_type* {
    return this->get_vertex(_x)->edge_to(_y);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename... _Args> auto
graph<_Vk, _Vv, _Ev, _Hash, _Alloc>::connect(const key_type& _x, const key_type& _y, _Args&&... _args) -> void {
    if (!this->contains(_x) || !this->contains(_y) || _x == _y || this->adjacent(_x, _y)) { return; }
    vertex_type* const _from = this->get_vertex(_x);
    vertex_type* const _to = this->get_vertex(_y);
    edge_type* const _e = this->_M_allocate_edge(_x, _from, _y, _to, std::forward<_Args>(_args)...);
    this->_edges.insert(_e);
    _from->insert_to(_y, _e);
    _to->insert_from(_x, _e);
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
            this->insert(_k, static_cast<const typename vertex_type::base&>(*_rhs.get_vertex(_k)));
        }
        const auto _outs = _rhs.get_vertex(_k)->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (!this->contains(_out->first)) {
                this->insert(_out->first, static_cast<const typename vertex_type::base&>(*_rhs.get_vertex(_out->first)));
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
    for (const auto& [_k, _v] : _rhs.vertices()) {
        _remains.push(_k);
    }
    while (!_remains.empty()) {
        const key_type& _k = _remains.front(); _remains.pop();
        if (!this->contains(_k)) {
            this->insert(_k, static_cast<const typename vertex_type::base&>(*_rhs.get_vertex(_k)));
        }
        const auto _outs = _rhs.get_vertex(_k)->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (!this->contains(_out->first)) {
                this->insert(_out->first, static_cast<const typename vertex_type::base&>(*_rhs.get_vertex(_out->first)));
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
        const vertex_type* _vertex = this->get_vertex(_k);
        const vertex_type* _rhs_vertex = _rhs.get_vertex(_k);
        if (*_vertex != *_rhs_vertex || _vertex->indegree() != _rhs_vertex->indegree() || _vertex->outdegree() != _rhs_vertex->outdegree()) {
            return false;
        }
        _remains.push(_k);
    }
    while (!_remains.empty()) {
        const key_type& _k = _remains.front(); _remains.pop();
        const auto _outs = _rhs.get_vertex(_k)->out();
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
    return this->get_vertex(_x)->edge_to(_y);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::get_edge(const key_type& _x, const key_type& _y) const -> std::pair<const_iterator, const_iterator> {
    return this->get_vertex(_x)->edge_to(_y);
}
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc> template <typename... _Args> auto
multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>::connect(const key_type& _x, const key_type& _y, _Args&&... _args) -> void {
    if (!this->contains(_x) || !this->contains(_y)) { return; }
    vertex_type* const _from = this->get_vertex(_x);
    vertex_type* const _to = this->get_vertex(_y);
    edge_type* const _e = this->_M_allocate_edge(_x, _from, _y, _to, std::forward<_Args>(_args)...);
    this->_edges.insert(_e);
    _from->insert_to(_y, _e);
    _to->insert_from(_x, _e);
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
            this->insert(_k, static_cast<const typename vertex_type::base&>(*_rhs.get_vertex(_k)));
        }
        const auto _outs = _rhs.get_vertex(_k)->out();
        for (auto _out = _outs.first; _out != _outs.second; ++_out) {
            if (!this->contains(_out->first)) {
                this->insert(_out->first, static_cast<const typename vertex_type::base&>(*_rhs.get_vertex(_out->first)));
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