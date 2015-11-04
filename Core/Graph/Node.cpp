//
// Created by michael on 04.11.15.
//

#include "Node.h"

weave::Node::Node(const std::string &groupName, weave::ID nodeId) {
    this->group = groupName;
    this->id = nodeId;
}

std::string weave::Node::GetGroup() const {
    return group;
}

weave::ID weave::Node::GetId() const {
    return id;
}

weave::Node::Node() : group(""), id(0) {
}
