//
// Created by michael on 04.11.15.
//

#pragma once

#include "../WeaverTypes.h"
#include "../../World/MetaData.h"

namespace weave {
    class Node {
    public:
        Node();

        Node(const std::string &groupName, ID nodeId);

        Node(const std::string &groupName, ID nodeId, const std::vector<MetaData> &history);

        std::string GetGroup() const;

        ID GetId() const;

        const std::vector<MetaData> &GetHistory() const;

        bool operator==(const Node &other) const;

        bool operator<(const Node &other) const;

    private:
        std::string group;
        ID id;
        std::vector<MetaData> history;
    };
}
