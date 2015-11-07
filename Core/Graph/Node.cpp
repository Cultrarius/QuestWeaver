//
// Created by michael on 04.11.15.
//

#include "Node.h"

using namespace std;
using namespace weave;

Node::Node() : group(""), id(0) {
}

Node::Node(const string &groupName, ID nodeId) {
    this->group = groupName;
    this->id = nodeId;
}

Node::Node(const string &groupName, ID nodeId, const vector<MetaData> &history) {
    this->group = groupName;
    this->id = nodeId;
    this->history = history;
}

string Node::GetGroup() const {
    return group;
}

ID Node::GetId() const {
    return id;
}

bool Node::operator==(const Node &other) const {
    return group == other.group && id == other.id;
}

bool Node::operator<(const Node &other) const {
    return group < other.group || id < other.id;
}

const vector<MetaData> &Node::GetHistory() const {
    return history;
}
