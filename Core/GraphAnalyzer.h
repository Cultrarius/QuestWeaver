//
// Created by michael on 07.11.15.
//

#include <unordered_map>
#include "WeaverTypes.h"
#include "Graph/WeaverGraph.h"

namespace weave {
    class GraphAnalyzer {
    public:
        static std::unordered_map<std::string, Node> SolveGraph(WeaverGraph *graph);
    };
}
