//
// Created by michael on 20.11.15.
//

#include "TestEntity.h"

weave::TestEntity::TestEntity(weave::ID id) : WorldEntity(id) {
}

weave::TestEntity::TestEntity() : TestEntity(NoID) {
}

std::string weave::TestEntity::ToString() const {
    return "TestEntity";
}

std::string weave::TestEntity::GetType() const {
    return "TestEntityType";
}
