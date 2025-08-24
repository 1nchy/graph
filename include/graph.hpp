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

namespace __details__::graph {
enum type { SIMPLE, MULTI, };
enum direction { DIRECTED, UNDIRECTED, };
template <typename _Tp> struct storage;
template <typename _Vk, type _Gt, typename _Vv, typename _Ev, direction _Gd, typename _Hash, typename _Alloc> struct alloc;
template <typename _Vk, type _Gt, typename _Vv, typename _Ev, direction _Gd, typename _Hash, typename _Alloc> struct basis;
template <typename _Vk, typename _Vv, typename _Ev, direction _Gd, typename _Hash, typename _Alloc> struct graph_basis;
template <typename _Vk, typename _Vv, typename _Ev, direction _Gd, typename _Hash, typename _Alloc> struct multigraph_basis;
template <typename _Vk, type _Gt, typename _Vv, typename _Ev, direction _Gd, typename _Cost, typename _Hash, typename _Alloc> struct dijkstra;
template <typename _Vk, type _Gt, typename _Vv, typename _Ev, direction _Gd, typename _Cost, typename _Hash, typename _Alloc> struct floyd;
template <typename _Vk, type _Gt, typename _Vv, typename _Ev, direction _Gd, typename _Hash, typename _Alloc> struct tarjan;
}
template <typename _Ev, __details__::graph::direction _Gd, typename _Vk, __details__::graph::type _Gt, typename _Vv, typename _Hash> struct edge;
template <typename _Vk, __details__::graph::type _Gt, typename _Vv, typename _Ev, __details__::graph::direction _Gd, typename _Hash> struct vertex;
template <typename _Vk, typename _Vv = void, typename _Ev = void, typename _Hash = std::hash<_Vk>, typename _Alloc = std::allocator<_Vk>> struct graph;
template <typename _Vk, typename _Vv = void, typename _Ev = void, typename _Hash = std::hash<_Vk>, typename _Alloc = std::allocator<_Vk>> struct multigraph;
template <typename _Vk, typename _Vv = void, typename _Ev = void, typename _Hash = std::hash<_Vk>, typename _Alloc = std::allocator<_Vk>> struct undirected_graph;
template <typename _Vk, typename _Vv = void, typename _Ev = void, typename _Hash = std::hash<_Vk>, typename _Alloc = std::allocator<_Vk>> struct undirected_multigraph;

namespace __details__::graph { // @def __details__::graph::storage
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

template <typename _Vk, typename _Vv, typename _Ev, __details__::graph::direction _Gd, typename _Hash>
struct vertex<_Vk, __details__::graph::MULTI, _Vv, _Ev, _Gd, _Hash> : public __details__::graph::storage<_Vv> {
public:
    using enum __details__::graph::type;
    using base = __details__::graph::storage<_Vv>;
    using key_type = _Vk;
    using value_type = typename base::value_type;
    using vertex_type = vertex<_Vk, MULTI, _Vv, _Ev, _Gd, _Hash>;
    using edge_type = edge<_Ev, _Gd, _Vk, MULTI, _Vv, _Hash>;
    using iterator = typename std::unordered_multimap<key_type, edge_type*>::iterator;
    using const_iterator = typename std::unordered_multimap<key_type, edge_type*>::const_iterator;
public:
    template <typename... _Args> vertex(_Args&&...);
    vertex(const vertex_type&) = delete;
    vertex(vertex_type&&) = default;
    auto operator=(const vertex_type&) -> vertex_type& = delete;
    auto operator=(vertex_type&&) -> vertex_type& = delete;
    virtual ~vertex() = default;
    template <typename __Vk, __details__::graph::type __Gt, typename __Vv, typename __Ev, __details__::graph::direction __Gd, typename __Hash, typename __Alloc>
    friend struct __details__::graph::basis;
    template <typename __Vk, typename __Vv, typename __Ev, typename __Hash, typename __Alloc> friend struct multigraph;
    template <typename __Vk, typename __Vv, typename __Ev, typename __Hash, typename __Alloc> friend struct undirected_multigraph;
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
template <typename _Vk, typename _Vv, typename _Ev, __details__::graph::direction _Gd, typename _Hash>
struct vertex<_Vk, __details__::graph::SIMPLE, _Vv, _Ev, _Gd, _Hash> : public __details__::graph::storage<_Vv> {
public:
    using enum __details__::graph::type;
    using base = __details__::graph::storage<_Vv>;
    using key_type = _Vk;
    using value_type = typename base::value_type;
    using vertex_type = vertex<_Vk, SIMPLE, _Vv, _Ev, _Gd, _Hash>;
    using edge_type = edge<_Ev, _Gd, _Vk, SIMPLE, _Vv, _Hash>;
    using iterator = typename std::unordered_map<key_type, edge_type*>::iterator;
    using const_iterator = typename std::unordered_map<key_type, edge_type*>::const_iterator;
public:
    template <typename... _Args> vertex(_Args&&...);
    vertex(const vertex_type&) = delete;
    vertex(vertex_type&&) = default;
    auto operator=(const vertex_type&) -> vertex_type& = delete;
    auto operator=(vertex_type&&) -> vertex_type& = delete;
    virtual ~vertex() = default;
    template <typename __Vk, __details__::graph::type __Gt, typename __Vv, typename __Ev, __details__::graph::direction __Gd, typename __Hash, typename __Alloc>
    friend struct __details__::graph::basis;
    template <typename __Vk, typename __Vv, typename __Ev, typename __Hash, typename __Alloc> friend struct graph;
    template <typename __Vk, typename __Vv, typename __Ev, typename __Hash, typename __Alloc> friend struct undirected_graph;
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
template <typename _Ev, typename _Vk, __details__::graph::type _Gt, typename _Vv, typename _Hash>
struct edge<_Ev, __details__::graph::DIRECTED, _Vk, _Gt, _Vv, _Hash> : public __details__::graph::storage<_Ev> {
public:
    using enum __details__::graph::direction;
    using base = __details__::graph::storage<_Ev>;
    using value_type = typename base::value_type;
    using vertex_type = vertex<_Vk, _Gt, _Vv, _Ev, DIRECTED, _Hash>;
    using edge_type = edge<_Ev, DIRECTED, _Vk, _Gt, _Vv, _Hash>;
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
    auto other(const key_type&) const -> const vertex_type&;
    auto other_key(const key_type&) const -> const key_type&;
private:
    const std::pair<key_type, vertex_type*> _in;
    const std::pair<key_type, vertex_type*> _out;
};
template <typename _Ev, typename _Vk, __details__::graph::type _Gt, typename _Vv, typename _Hash>
struct edge<_Ev, __details__::graph::UNDIRECTED, _Vk, _Gt, _Vv, _Hash> : public __details__::graph::storage<_Ev> {
public:
    using base = __details__::graph::storage<_Ev>;
    using value_type = typename base::value_type;
    using vertex_type = vertex<_Vk, _Gt, _Vv, _Ev, __details__::graph::UNDIRECTED, _Hash>;
    using edge_type = edge<_Ev, __details__::graph::UNDIRECTED, _Vk, _Gt, _Vv, _Hash>;
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
    auto either() const -> const vertex_type&;
    auto either_key() const -> const key_type&;
    auto other(const key_type&) const -> const vertex_type&;
    auto other_key(const key_type&) const -> const key_type&;
private:
    const std::pair<key_type, vertex_type*> _x;
    const std::pair<key_type, vertex_type*> _y;
};

namespace __details__::graph { // @def __details__::graph::alloc
template <typename _Vk, type _Gt, typename _Vv, typename _Ev, direction _Gd, typename _Hash, typename _Alloc> struct alloc: public _Alloc {
public:
    using vertex_type = vertex<_Vk, _Gt, _Vv, _Ev, _Gd, _Hash>;
    using edge_type = edge<_Ev, _Gd, _Vk, _Gt, _Vv, _Hash>;
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
    template <typename... _Args> auto _M_allocate_vertex(_Args&&... _args) const -> vertex_type*;
    auto _M_deallocate_vertex(vertex_type* _p) const -> void;
    template <typename... _Args> auto _M_allocate_edge(_Args&&... _args) const -> edge_type*;
    auto _M_deallocate_edge(edge_type* _p) const -> void;
};
}

namespace __details__::graph { // @def __details__::graph::basis
/**
 * @implements
 * `graph_basis` and `multigraph_basis` derived from `basis`
 * `graph` and `undirected_graph` derived from `graph_basis`
 * `multigraph` and `undirected_multigraph` derived from `multigraph_basis`
 * @implements
 * methods specific to directed or undirected graphs are implemented in different way,
 * moreover, methods specific to simple or multi graph have different return type.
 * so, methods specific to directed or undirected graphs are implemented in specialized template,
 * and methods specific to simple or multi graph are implemented in derived class.
 * @implements
 * | diff \\ specific | undirected/directed | simple/multi |       all        |
 * |:----------------:|:-------------------:|:------------:|:----------------:|
 * |  implementation  |      disconnect     |   for_each   | (bi)connect<...> |
 * |   +return type   |          \          |   get_edge   |                  |
 * `basis::erase` depends on `disconnect`
 * @implements
 * UNDIRECTED --> DIRECTED, SIMPLE --> MULTI
 */
template <typename _Vk, type _Gt, typename _Vv, typename _Ev, direction _Gd, typename _Hash, typename _Alloc>
struct basis : public alloc<_Vk, _Gt, _Vv, _Ev, _Gd, _Hash, _Alloc> {
public:
    using vertex_type = vertex<_Vk, _Gt, _Vv, _Ev, _Gd, _Hash>;
    using edge_type = edge<_Ev, _Gd, _Vk, _Gt, _Vv, _Hash>;
    using key_type = typename vertex_type::key_type;
    template <typename _R> using vertex_visitor = std::function<_R(const key_type&, const vertex_type&)>;
    template <typename _R> using edge_visitor = std::function<_R(const edge_type&)>;
    template <typename _R> using vertex_modifier = std::function<_R(const key_type&, vertex_type&)>;
    template <typename _R> using edge_modifier = std::function<_R(edge_type&)>;
    template <typename __Vk, type __Gt, typename __Vv, typename __Ev, direction __Gd, typename __Cost, typename __Hash, typename __Alloc> friend struct dijkstra;
    template <typename __Vk, type __Gt, typename __Vv, typename __Ev, direction __Gd, typename __Cost, typename __Hash, typename __Alloc> friend struct floyd;
protected:
    basis() = default;
    basis(const basis&) {} // implement in derived class
    basis(basis&&) {} // implement in derived class
public:
    virtual ~basis() { clear(); }
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
    template <typename... _Args> auto insert(const key_type& _k, _Args&&... _args) -> void;
    /**
     * @brief erase vertex with specific key
     * @param _k the key of the vertex
     */
    auto erase(const key_type& _k) -> void;
    /**
     * @brief disconnect vertex with all vertices
     * @param _k the key of the vertex
     */
    auto disconnect(const key_type& _k) -> void;
    auto disconnect(const key_type& _x, const key_type& _y) -> void;
    auto clear() -> void;
public: // algorithm
    /**
     * @brief dijkstra algorithm
     * @param _k start vertex key
     * @param visitor edge visitor, _R(*)(const edge_type&)
     * @return dijkstra result wrapper
     * @throw out_of_range if the key not exists
     * @note can't handle negative weight edge
     */
    template <typename _R> auto dijkstra(const key_type& _k, edge_visitor<_R>&& visitor) const -> graph::dijkstra<_Vk, _Gt, _Vv, _Ev, _Gd, _R, _Hash, _Alloc>;
    /**
     * @brief floyd algorithm
     * @tparam _R cost type
     * @param visitor edge visitor, _R(*)(const edge_type&)
     * @return floyd result wrapper
     * @note can't handle negative weight loop
     */
    template <typename _R> auto floyd(edge_visitor<_R>&& visitor) const -> graph::floyd<_Vk, _Gt, _Vv, _Ev, _Gd, _R, _Hash, _Alloc>;
    auto tarjan() const -> graph::tarjan<_Vk, _Gt, _Vv, _Ev, _Gd, _Hash, _Alloc>;
    auto bfs(const key_type& _k, vertex_modifier<void>&& modifier) -> size_t;
    auto bfs(const key_type& _k, vertex_visitor<void>&& visitor) const -> size_t;
    /**
     * @brief depth first search
     * @param _k the begin of dfs
     * @param pre_modifier pre-order modifier (traverse vertex in pre-order)
     * @param post_modifer post-order modifier (traverse vertex in post-order)
     * @param backtracer invoke when backtracing from dfs
     * @return the count of traversed vertices
     * @note backtracer : `std::function<void(const key_type&, vertex_type&, const key_type&)>&&`
     * @example 
     * traditional dfs
     * ```c++
     * dfs(u):
     *   if traversed(u) return;
     *   pre_modifier(key(u), u)
     *   for v : u.outs()
     *     if not traversed(v)
     *       dfs(v)
     *       backtracer(key(u), u, key(v))
     *   post_modifier(key(u), u)
     * ```
     */
    auto dfs(const key_type& _k,
             vertex_modifier<void>&& pre_modifier = nullptr,
             vertex_modifier<void>&& post_modifier = nullptr,
             std::function<void(const key_type&, vertex_type&, const key_type&)>&& backtracer = nullptr
            ) -> size_t;
    /**
     * @brief depth first search
     * @param _k the begin of dfs
     * @param pre_visitor pre-order visitor (traverse vertex in pre-order)
     * @param post_visitor post-order visitor (traverse vertex in post-order)
     * @param backtracer invoke when backtracing from dfs
     * @return the count of traversed vertices
     * @note backtracer : `std::function<void(const key_type&, const vertex_type&, const key_type&)>&&`
     * @example 
     * traditional dfs
     * ```c++
     * dfs(u):
     *   if traversed(u) return;
     *   pre_visitor(key(u), u)
     *   for v : u.outs()
     *     if not traversed(v)
     *       dfs(v)
     *       backtracer(key(u), u, key(v))
     *   post_visitor(key(u), u)
     * ```
     */
    auto dfs(const key_type& _k,
             vertex_visitor<void>&& pre_visitor = nullptr,
             vertex_visitor<void>&& post_visitor = nullptr,
             std::function<void(const key_type&, const vertex_type&, const key_type&)>&& backtracer = nullptr
            ) const -> size_t;
protected:
    /**
     * @brief data and structure assignment
     * @tparam _S source type
     * @tparam _D destination type
     * @tparam _Sd graph::direction of source
     * @tparam _Dd graph::direction of destination
     * @param _src `_rhs`
     * @param _dst `*this`
     * @implements traverse each edge once for UNDIRECTED
     * | dst \\ src |    UNDIRECTED    |    DIRECTED    |
     * |:----------:|:----------------:|:--------------:|
     * | UNDIRECTED |   connect(...)   |       --       |
     * |  DIRECTED  |  biconnect(...)  |  connect(...)  |
     */
    template <typename _S, typename _D, direction _Sd, direction _Dd> static auto assign(const _S& _src, _D& _dst) -> void;
    template <typename _G> static auto equal(const _G&, const _G&, std::function<bool(const key_type&, const key_type&, const edge_type&)>&&) -> bool;
    virtual auto for_each(const key_type& _x, const key_type& _y, edge_modifier<void>&& modifier) -> void = 0;
    virtual auto for_each(const key_type& _x, const key_type& _y, edge_visitor<void>&& visitor) const -> void = 0;
protected:
    std::unordered_map<key_type, vertex_type*, _Hash> _vertices;
    std::unordered_set<edge_type*> _edges;
};
template <typename _Vk, typename _Vv, typename _Ev, direction _Gd, typename _Hash, typename _Alloc>
struct graph_basis : public basis<_Vk, SIMPLE, _Vv, _Ev, _Gd, _Hash, _Alloc> {
public:
    using base = basis<_Vk, SIMPLE, _Vv, _Ev, _Gd, _Hash, _Alloc>;
    using vertex_type = vertex<_Vk, SIMPLE, _Vv, _Ev, _Gd, _Hash>;
    using edge_type = edge<_Ev, _Gd, _Vk, SIMPLE, _Vv, _Hash>;
    using key_type = typename vertex_type::key_type;
    template <typename _R> using vertex_visitor = std::function<_R(const key_type&, const vertex_type&)>;
    template <typename _R> using edge_visitor = std::function<_R(const edge_type&)>;
    template <typename _R> using vertex_modifier = std::function<_R(const key_type&, vertex_type&)>;
    template <typename _R> using edge_modifier = std::function<_R(edge_type&)>;
protected:
    graph_basis() = default;
    graph_basis(const graph_basis&) {} // implement in derived class
    graph_basis(graph_basis&&) {} // implement in derived class
public:
    virtual ~graph_basis() = default;
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
protected:
    auto for_each(const key_type& _x, const key_type& _y, edge_modifier<void>&& modifier) -> void override;
    auto for_each(const key_type& _x, const key_type& _y, edge_visitor<void>&& visitor) const -> void override;
};
template <typename _Vk, typename _Vv, typename _Ev, direction _Gd, typename _Hash, typename _Alloc>
struct multigraph_basis : public basis<_Vk, MULTI, _Vv, _Ev, _Gd, _Hash, _Alloc> {
public:
    using base = basis<_Vk, MULTI, _Vv, _Ev, _Gd, _Hash, _Alloc>;
    using vertex_type = vertex<_Vk, MULTI, _Vv, _Ev, _Gd, _Hash>;
    using edge_type = edge<_Ev, _Gd, _Vk, MULTI, _Vv, _Hash>;
    using key_type = typename vertex_type::key_type;
    template <typename _R> using vertex_visitor = std::function<_R(const key_type&, const vertex_type&)>;
    template <typename _R> using edge_visitor = std::function<_R(const edge_type&)>;
    template <typename _R> using vertex_modifier = std::function<_R(const key_type&, vertex_type&)>;
    template <typename _R> using edge_modifier = std::function<_R(edge_type&)>;
private:
    using iterator = typename vertex_type::iterator;
    using const_iterator = typename vertex_type::const_iterator;
protected:
    multigraph_basis() = default;
    multigraph_basis(const multigraph_basis&) {} // implement in derived class
    multigraph_basis(multigraph_basis&&) {} // implement in derived class
public:
    virtual ~multigraph_basis() = default;
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
protected:
    auto for_each(const key_type& _x, const key_type& _y, edge_modifier<void>&& modifier) -> void override;
    auto for_each(const key_type& _x, const key_type& _y, edge_visitor<void>&& visitor) const -> void override;    
};
/**
 * @brief dijkstra result wrapper
 */
template <typename _Vk, type _Gt, typename _Vv, typename _Ev, direction _Gd, typename _Cost, typename _Hash, typename _Alloc> struct dijkstra {
public:
    using base = basis<_Vk, _Gt, _Vv, _Ev, _Gd, _Hash, _Alloc>;
    using vertex_type = typename base::vertex_type;
    using edge_type = typename base::edge_type;
    using key_type = typename base::key_type;
    using cost_type = _Cost;
    using trailer = std::function<void(const key_type&)>;
    template <typename __Vk, type __Gt, typename __Vv, typename __Ev, direction __Gd, typename __Hash, typename __Alloc> friend struct basis;
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
template <typename _Vk, type _Gt, typename _Vv, typename _Ev, direction _Gd, typename _Cost, typename _Hash, typename _Alloc> struct floyd {
public:
    using base = basis<_Vk, _Gt, _Vv, _Ev, _Gd, _Hash, _Alloc>;
    using vertex_type = typename base::vertex_type;
    using edge_type = typename base::edge_type;
    using key_type = typename base::key_type;
    using cost_type = _Cost;
    using trailer = std::function<void(const key_type&)>;
    template <typename __Vk, type __Gt, typename __Vv, typename __Ev, direction __Gd, typename __Hash, typename __Alloc> friend struct basis;
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
/**
 * @brief tarjan result wrapper
 */
template <typename _Vk, type _Gt, typename _Vv, typename _Ev, direction _Gd, typename _Hash, typename _Alloc> struct tarjan {
public:
    using base = basis<_Vk, _Gt, _Vv, _Ev, _Gd, _Hash, _Alloc>;
    using vertex_type = typename base::vertex_type;
    using edge_type = typename base::edge_type;
    using key_type = typename base::key_type;
    template <typename __Vk, type __Gt, typename __Vv, typename __Ev, direction __Gd, typename __Hash, typename __Alloc> friend struct basis;
private:
    tarjan(const base&);
public:
    tarjan(const tarjan&) = default;
    tarjan(tarjan&&) = default;
    auto operator=(const tarjan&) -> tarjan& = delete;
    auto operator=(tarjan&&) -> tarjan& = delete;
    ~tarjan() = default;
public:
    const auto& vertices() const { return _vertices; }
    const auto& edges() const { return _edges; }
private:
    const base& _g;
    std::unordered_set<key_type> _vertices;
    std::unordered_set<const edge_type*> _edges;
};

}


/**
 * @brief undirected simple graph
 * @note no multi edge, no loop
 * @tparam _Vk vertex key
 * @tparam _Vv vertex value (`void` by default)
 * @tparam _Ev edge value (`void` by default)
 */
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc>
struct undirected_graph : public __details__::graph::graph_basis<_Vk, _Vv, _Ev, __details__::graph::UNDIRECTED, _Hash, _Alloc> {
public:
    using enum __details__::graph::direction;
    using base = __details__::graph::graph_basis<_Vk, _Vv, _Ev, UNDIRECTED, _Hash, _Alloc>;
    using key_type = typename base::key_type;
    using vertex_type = typename base::vertex_type;
    using edge_type = typename base::edge_type;
    template <typename _R> using vertex_visitor = typename base::template vertex_visitor<_R>;
    template <typename _R> using edge_visitor = typename base::template edge_visitor<_R>;
    template <typename _R> using vertex_modifier = typename base::template vertex_modifier<_R>;
    template <typename _R> using edge_modifier = typename base::template edge_modifier<_R>;
public:
    undirected_graph() = default;
    undirected_graph(const undirected_graph&);
    auto operator=(const undirected_graph&) -> undirected_graph&;
    virtual ~undirected_graph() = default;
public:
    auto operator==(const undirected_graph&) const -> bool;
    auto operator!=(const undirected_graph&) const -> bool;
public:
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
};
/**
 * @brief undirected multi graph
 * @tparam _Vk vertex key
 * @tparam _Vv vertex value (`void` by default)
 * @tparam _Ev edge value (`void` by default)
 */
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc>
struct undirected_multigraph : public __details__::graph::multigraph_basis<_Vk, _Vv, _Ev, __details__::graph::UNDIRECTED, _Hash, _Alloc> {
public:
    using enum __details__::graph::direction;
    using base = __details__::graph::multigraph_basis<_Vk, _Vv, _Ev, UNDIRECTED, _Hash, _Alloc>;
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
    undirected_multigraph() = default;
    undirected_multigraph(const undirected_multigraph&);
    undirected_multigraph(const undirected_graph<_Vk, _Vv, _Ev, _Hash, _Alloc>&);
    auto operator=(const undirected_multigraph&) -> undirected_multigraph&;
    virtual ~undirected_multigraph() = default;
public:
    auto operator==(const undirected_multigraph&) const -> bool;
    auto operator!=(const undirected_multigraph&) const -> bool;
public:
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
};
/**
 * @brief directed simple graph
 * @note no multi edge, no loop
 * @tparam _Vk vertex key
 * @tparam _Vv vertex value (`void` by default)
 * @tparam _Ev edge value (`void` by default)
 */
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc>
struct graph : public __details__::graph::graph_basis<_Vk, _Vv, _Ev, __details__::graph::DIRECTED, _Hash, _Alloc> {
public:
    using enum __details__::graph::direction;
    using base = __details__::graph::graph_basis<_Vk, _Vv, _Ev, DIRECTED, _Hash, _Alloc>;
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
    graph(const undirected_graph<_Vk, _Vv, _Ev, _Hash, _Alloc>&);
    auto operator=(const graph&) -> graph&;
    virtual ~graph() = default;
public:
    auto operator==(const graph&) const -> bool;
    auto operator!=(const graph&) const -> bool;
public:
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
};
/**
 * @brief directed multi graph
 * @tparam _Vk vertex key
 * @tparam _Vv vertex value (`void` by default)
 * @tparam _Ev edge value (`void` by default)
 */
template <typename _Vk, typename _Vv, typename _Ev, typename _Hash, typename _Alloc>
struct multigraph : public __details__::graph::multigraph_basis<_Vk, _Vv, _Ev, __details__::graph::DIRECTED, _Hash, _Alloc> {
public:
    using enum __details__::graph::direction;
    using base = __details__::graph::multigraph_basis<_Vk, _Vv, _Ev, DIRECTED, _Hash, _Alloc>;
    using key_type = typename base::key_type;
    using vertex_type = typename base::vertex_type;
    using edge_type = typename base::edge_type;
    template <typename _R> using vertex_visitor = typename base::template vertex_visitor<_R>;
    template <typename _R> using edge_visitor = typename base::template edge_visitor<_R>;
    template <typename _R> using vertex_modifier = typename base::template vertex_modifier<_R>;
    template <typename _R> using edge_modifier = typename base::template edge_modifier<_R>;
public:
    multigraph() = default;
    multigraph(const multigraph&);
    multigraph(const graph<_Vk, _Vv, _Ev, _Hash, _Alloc>&);
    multigraph(const undirected_graph<_Vk, _Vv, _Ev, _Hash, _Alloc>&);
    multigraph(const undirected_multigraph<_Vk, _Vv, _Ev, _Hash, _Alloc>&);
    auto operator=(const multigraph&) -> multigraph&;
    virtual ~multigraph() = default;
public:
    auto operator==(const multigraph&) const -> bool;
    auto operator!=(const multigraph&) const -> bool;
public:
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
};

}

#include "graph.inl"

#endif // _ICY_GRAPH_HPP_