//
// Created by michael on 10.08.15.
//

#pragma once

#include "../Template/Template.h"
#include "WeaverUtils.h"
#include "../QuestModel/QuestModel.h"
#include "Graph/WeaverGraph.h"
#include "GraphAnalyzer.h"

namespace weave {
    struct EngineParameters {
        float chooseOldOverNew = 0.9;
        bool useDice = false;
        AnalyzerParameters analyzerParameters;

        template<class Archive>
        void serialize(Archive &ar) {
            ar(CEREAL_NVP(chooseOldOverNew), CEREAL_NVP(analyzerParameters));
        }
    };

    struct PropertyCandidate {
        PropertyCandidate(const TemplateQuestProperty &property, ModelAction action, std::vector<ModelAction> history)
                : property(property), action(action), history(history) { }

        TemplateQuestProperty property;
        ModelAction action;
        std::vector<ModelAction> history;
    };

    class WeaverEngine {
    public:
        std::vector<QuestPropertyValue> fillTemplate(std::shared_ptr<Template> questTemplate,
                                                     const QuestModel &questModel,
                                                     const WorldModel &worldModel,
                                                     std::shared_ptr<RandomStream> randomStream,
                                                     std::vector<ModelAction> *modelActions) const;

        EngineParameters GetParameters();

        void SetParameters(EngineParameters parameters);

    private:
        EngineParameters parameters;

        std::vector<QuestPropertyValue> fillWithRandomDice(const std::shared_ptr<Template> &questTemplate,
                                                           const WorldModel &worldModel,
                                                           std::shared_ptr<RandomStream> randomStream,
                                                           std::vector<ModelAction> *modelActions) const;

        WeaverGraph createGraph(const QuestModel &questModel, const WorldModel &worldModel,
                                std::unordered_set<std::string> mandatoryProperties,
                                std::map<std::string, std::vector<ModelAction>> candidates) const;
    };
}
