#pragma once

using key_type = std::string;
using value_type = char;
using cost_type = unsigned;

#define TYPE(type) type<key_type, value_type, cost_type>

// g++ -E test/main.hpp -I include -DMERMAID
#ifndef MERMAID
#include "graph.hpp"
#include <string>
#define BUILD_BEGIN(type) TYPE(type) ANONYMOUS_GRAPH
#define BUILD_END return ANONYMOUS_GRAPH
#define BUILD_VERTEX(x, v) ANONYMOUS_GRAPH.insert(#x, #v[0])
#define BUILD_EDGE(x, y, e) ANONYMOUS_GRAPH.connect(#x, #y, e)
#else // MERMAID
#define BUILD_BEGIN(type) ~~~mermaid
#define BUILD_END ~~~
#define BUILD_VERTEX(x, v) x((#v))
#define BUILD_EDGE(x, y, e) x--#e-->y
#endif // MERMAID

template <size_t _I> auto graph_instance() -> TYPE(icy::graph);
template <size_t _I> auto multigraph_instance() -> TYPE(icy::multigraph);

template <> auto graph_instance<0>() -> TYPE(icy::graph) {
    BUILD_BEGIN(icy::graph);
    BUILD_VERTEX(A, a);
    BUILD_VERTEX(B, B);
    BUILD_VERTEX(C, c);
    BUILD_VERTEX(D, d);
    BUILD_VERTEX(E, E);
    BUILD_VERTEX(F, f);
    BUILD_EDGE(A, B, 50);
    BUILD_EDGE(A, C, 10);
    BUILD_EDGE(A, E, 45);
    BUILD_EDGE(B, C, 15);
    BUILD_EDGE(B, E, 10);
    BUILD_EDGE(C, A, 20);
    BUILD_EDGE(C, D, 15);
    BUILD_EDGE(D, B, 20);
    BUILD_EDGE(D, E, 35);
    BUILD_EDGE(E, D, 30);
    BUILD_EDGE(F, D, 3);
    BUILD_END;
}
/**
 * @brief disconnected graph
 */
template <> auto graph_instance<1>() -> TYPE(icy::graph) {
    BUILD_BEGIN(icy::graph);
    BUILD_VERTEX(A, a);
    BUILD_VERTEX(B, B);
    BUILD_VERTEX(C, c);
    BUILD_VERTEX(D, d);
    BUILD_VERTEX(E, E);
    BUILD_VERTEX(F, f);
    BUILD_VERTEX(G, G);
    BUILD_VERTEX(H, h);
    BUILD_VERTEX(I, i);
    BUILD_VERTEX(J, j);
    BUILD_VERTEX(K, k);
    BUILD_VERTEX(L, L);
    BUILD_VERTEX(M, m);
    BUILD_EDGE(A, B, 2);
    BUILD_EDGE(A, C, 2);
    BUILD_EDGE(A, F, 1);
    BUILD_EDGE(A, L, 3);
    BUILD_EDGE(C, A, 1);
    BUILD_EDGE(B, A, 1);
    BUILD_EDGE(B, M, 1);
    BUILD_EDGE(M, L, 1);
    BUILD_EDGE(J, M, 1);
    BUILD_EDGE(L, J, 1);
    BUILD_EDGE(D, E, 2);
    BUILD_EDGE(E, D, 3);
    BUILD_EDGE(I, G, 3);
    BUILD_EDGE(G, I, 2);
    BUILD_EDGE(G, H, 2);
    BUILD_EDGE(G, K, 2);
    BUILD_EDGE(H, K, 2);
    BUILD_EDGE(H, G, 2);
    BUILD_END;
}

template <> auto multigraph_instance<0>() -> TYPE(icy::multigraph) {
    BUILD_BEGIN(icy::multigraph);
    BUILD_VERTEX(A, a);
    BUILD_VERTEX(B, B);
    BUILD_VERTEX(C, c);
    BUILD_VERTEX(D, d);
    BUILD_VERTEX(E, E);
    BUILD_EDGE(A, B, 2);
    BUILD_EDGE(A, C, 3);
    BUILD_EDGE(B, A, 2);
    BUILD_EDGE(B, C, 2);
    BUILD_EDGE(B, C, 4);
    BUILD_EDGE(B, C, 6);
    BUILD_EDGE(C, D, 2);
    BUILD_EDGE(D, A, 4);
    BUILD_EDGE(D, B, 3);
    BUILD_EDGE(D, D, 1);
    BUILD_EDGE(D, E, 2);
    BUILD_EDGE(D, E, 4);
    BUILD_EDGE(E, A, 2);
    BUILD_EDGE(E, C, 3);
    BUILD_END;
}
template <> auto multigraph_instance<1>() -> TYPE(icy::multigraph) {
    BUILD_BEGIN(icy::multigraph);
    BUILD_VERTEX(A, a);
    BUILD_VERTEX(B, B);
    BUILD_VERTEX(C, c);
    BUILD_VERTEX(D, d);
    BUILD_VERTEX(E, E);
    BUILD_VERTEX(F, f);
    BUILD_VERTEX(G, G);
    BUILD_VERTEX(H, h);
    BUILD_VERTEX(I, i);
    BUILD_EDGE(A, B, 3);
    BUILD_EDGE(B, C, 15);
    BUILD_EDGE(B, E, 12);
    BUILD_EDGE(C, F, 18);
    BUILD_EDGE(D, A, 5);
    BUILD_EDGE(D, E, 18);
    BUILD_EDGE(E, D, 10);
    BUILD_EDGE(E, F, 25);
    BUILD_EDGE(E, G, 9);
    BUILD_EDGE(E, G, 14);
    BUILD_EDGE(F, F, 3);
    BUILD_EDGE(G, D, 11);
    BUILD_EDGE(G, H, 6);
    BUILD_EDGE(I, A, 7);
    BUILD_EDGE(I, B, 13);
    BUILD_END;
}

#undef TYPE
#undef BUILD_BEGIN
#undef BUILD_END
#undef BUILD_VERTEX
#undef BUILD_EDGE

#ifndef MERMAID
#include "graph.hpp"
#include <string>
#endif // MERMAID