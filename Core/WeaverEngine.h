//
// Created by michael on 10.08.15.
//

#pragma once

#include "../Template/Template.h"
#include "WeaverUtils.h"
#include "WeaverTypes.h"
#include "../QuestModel/QuestModel.h"
#include "Graph/WeaverGraph.h"
#include "Graph/GraphAnalyzer.h"
#include "../Story/StoryWriter.h"

namespace weave {
    struct EngineParameters {
        float chooseOldOverNew = 0.9;
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

    class EngineResult {
    public:
        EngineResult(const std::vector<WorldModelAction> &actions,
                     const std::vector<QuestPropertyValue> &propertyValues,
                     const std::string &story);

        const std::vector<WorldModelAction> &GetModelActions() const;

        const std::vector<QuestPropertyValue> &GetQuestPropertyValues() const;

        const std::string &GetStory() const;

    private:
        std::vector<WorldModelAction> actions;
        std::vector<QuestPropertyValue> propertyValues;
        std::string story;
    };

    class WeaverEngine {
    public:
        explicit WeaverEngine(std::shared_ptr<RandomStream> randomStream);

        EngineResult fillTemplate(std::shared_ptr<Template> questTemplate,
                                  const QuestModel &questModel,
                                  const WorldModel &worldModel,
                                  const StoryWriter &storyWriter) const;

        EngineParameters GetParameters();

        void SetParameters(EngineParameters parameters);

    private:
        EngineParameters parameters;
        std::shared_ptr<RandomStream> randomStream;

        WeaverGraph createGraph(const QuestModel &questModel, const WorldModel &worldModel,
                                std::unordered_set<std::string> mandatoryProperties,
                                std::map<std::string, std::vector<WorldModelAction>> candidates) const;

        void addGraphEdges(const QuestModel &questModel, WeaverGraph &graph,
                           std::unordered_set<ID> &candidateIds) const;
    };
}
