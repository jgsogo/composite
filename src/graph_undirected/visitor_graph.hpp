#pragma once

#include <set>
#include <queue>
#include <stack>

namespace composite::_impl::graph {

    template<typename GraphNodeT, typename GraphEdgeT>
    class VisitorGraph {
    public:
        using GraphNode = GraphNodeT;
        using GraphEdge = GraphEdgeT;

    public:
        VisitorGraph() = default;

        virtual void visit(GraphNode &) {}

        virtual void visit(GraphEdge &) {}

        virtual bool enterNode(GraphNode &n) {
            return true;
        }

        virtual void exitNode(GraphNode &) {}

        virtual bool enterEdge(GraphEdge &e) {
            return true;
        }

        virtual void exitEdge(GraphEdge &) {}

        virtual void start(GraphNode &node) = 0;

    protected:

    };

    template<typename GraphNodeT, typename GraphEdgeT>
    class DFSPreOrderVisitorGraph : public VisitorGraph<GraphNodeT, GraphEdgeT> {
    public:
        DFSPreOrderVisitorGraph() = default;

        void start(GraphNodeT &initNode) override {
            std::set<int> visitedNodes;
            std::set<int> visitedEdges;

            visitedNodes.insert(initNode._uniqueId);
            this->enterNode(initNode);

            std::function<void(GraphNodeT &)> onNode = [&](GraphNodeT &item) {
                this->visit(item);
                for (auto &it: item._edges) {
                    auto[_, insertedEdge] = visitedEdges.insert(it.second->_uniqueId);
                    if (insertedEdge && this->enterEdge(*it.second)) {
                        this->visit(*it.second);
                        auto[_1, insertedNode] = visitedNodes.insert(it.first.get()._uniqueId);
                        if (insertedNode && this->enterNode(it.first)) {
                            onNode(it.first);
                        }
                        this->exitEdge(*it.second);
                    }
                    this->exitNode(item);
                }
            };

            onNode(initNode);
        }
    };

    template<typename GraphNodeT, typename GraphEdgeT>
    class BFSVisitorGraph : public VisitorGraph<GraphNodeT, GraphEdgeT> {
    public:
        BFSVisitorGraph() = default;

        void start(GraphNodeT &initNode) override {
            std::set<int> visitedNodes;
            std::set<int> visitedEdges;
            std::queue<std::pair<std::reference_wrapper<GraphEdgeT>, std::reference_wrapper<GraphNodeT>>> queue;

            auto onNode = [&](GraphNodeT &item) {
                auto[_, inserted] = visitedNodes.insert(item._uniqueId);
                if (inserted && this->enterNode(item)) {
                    this->visit(item);
                    for (auto &it: item._edges) {
                        queue.push(std::make_pair(std::ref(*it.second), it.first));
                    }
                    this->exitNode(item);
                }
            };

            onNode(initNode);
            while (!queue.empty()) {
                auto[edge, node] = queue.front();
                queue.pop();
                auto[_, inserted] = visitedEdges.insert(edge.get()._uniqueId);
                if (inserted && this->enterEdge(edge)) {
                    this->visit(edge);
                    onNode(node);
                    this->exitEdge(edge);
                }
            }
        }

    };


}
