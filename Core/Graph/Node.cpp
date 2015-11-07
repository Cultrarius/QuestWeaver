//
// Created by michael on 04.11.15.
//

#include "Node.h"

using namespace std;
using namespace weave;

Node::Node(const string &groupName, ID nodeId) {
    this->group = groupName;
    this->id = nodeId;
}

string Node::GetGroup() const {
    return group;
}

ID Node::GetId() const {
    return id;
}

Node::Node() : group(""), id(0) {
}

bool Node::operator==(const Node &other) const {
    return group == other.group && id == other.id;
}

bool Node::operator<(const Node &other) const {
    return group < other.group || id < other.id;
}
