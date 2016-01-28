//
// Created by michael on 06.10.15.
//

#pragma once


#include <World/WorldEntity.h>

namespace weave {

    struct TestEntity : public WorldEntity {
        TestEntity();

        std::string ToString() const;

        std::string GetType() const;

    private:
        explicit TestEntity(ID id);

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::make_nvp("id", GetId()));
        }

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<TestEntity> &construct) {
            ID id;

            ar(cereal::make_nvp("id", id));
            construct(id);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::TestEntity);
