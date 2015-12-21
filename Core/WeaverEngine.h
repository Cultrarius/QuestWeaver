//
// Created by michael on 10.08.15.
//

#pragma once

#include "../Template/Template.h"
#include "WeaverUtils.h"
#include "../QuestModel/QuestModel.h"
#include "Graph/WeaverGraph.h"
#include "Graph/GraphAnalyzer.h"

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
        PropertyCandidate(const TemplateQuestProperty &property, WorldModelAction action,
                          std::vector<WorldModelAction> history)
                : property(property), action(action), history(history) { }

        TemplateQuestProperty property;
        WorldModelAction action;
        std::vector<WorldModelAction> history;
    };

    class WeaverEngine {
    public:
        std::vector<QuestPropertyValue> fillTemplate(std::shared_ptr<Template> questTemplate,
                                                     const QuestModel &questModel,
                                                     const WorldModel &worldModel,
                                                     std::shared_ptr<RandomStream> randomStream,
                                                     std::vector<WorldModelAction> *modelActions) const;

        EngineParameters GetParameters();

        void SetParameters(EngineParameters parameters);

    private:
        EngineParameters parameters;

        std::vector<QuestPropertyValue> fillWithRandomDice(const std::shared_ptr<Template> &questTemplate,
                                                           const WorldModel &worldModel,
                                                           std::shared_ptr<RandomStream> randomStream,
                                                           std::vector<WorldModelAction> *modelActions) const;

        WeaverGraph createGraph(const QuestModel &questModel, const WorldModel &worldModel,
                                std::unordered_set<std::string> mandatoryProperties,
                                std::map<std::string, std::vector<WorldModelAction>> candidates) const;

        void addGraphEdges(const QuestModel &questModel, WeaverGraph &graph,
                           std::unordered_set<ID> &candidateIds) const;
    };
}
