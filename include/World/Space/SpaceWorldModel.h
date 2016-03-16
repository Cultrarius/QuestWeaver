//
// Created by michael on 06.10.15.
//

#pragma once

#include "../WorldModel.h"
#include "SpaceLocation.h"
#include "../../Core/WeaverUtils.h"
#include "SpaceAgent.h"
#include "SolarSystem.h"
#include "SpaceNameGenerator.h"

namespace weave {
    struct ModelParameters {
        int minLocation = 0;
        int maxLocation = 1000;

        template<class Archive>
        void serialize(Archive &ar) {
            ar(CEREAL_NVP(minLocation), CEREAL_NVP(maxLocation));
        }
    };

    class SpaceWorldModel : public WorldModel {
    public:
        SpaceWorldModel() = default;

        explicit SpaceWorldModel(std::shared_ptr<RandomStream> randomStream);

        ModelParameters GetParameters();

        void SetParameters(ModelParameters parameters);

        const NameGenerator &GetNameGenerator() const override;

        // entity creation methods

        std::shared_ptr<SpaceLocation> CreateLocation() const;

        std::shared_ptr<SpaceAgent> CreateAgent() const;

        std::shared_ptr<SolarSystem> CreateSolarSystem() const;

    private:
        ModelParameters param;

        SpaceNameGenerator nameGenerator;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &ar) {
            ar( cereal::base_class<WorldModel>(this), CEREAL_NVP(param));
        }
    };
}

CEREAL_REGISTER_TYPE(weave::SpaceWorldModel);
