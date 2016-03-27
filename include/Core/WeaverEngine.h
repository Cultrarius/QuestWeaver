//
// Created by michael on 10.08.15.
//

#pragma once

#include <Template/QuestTemplate.h>
#include <Core/WeaverUtils.h>
#include <Core/WeaverTypes.h>
#include <QuestModel/QuestModel.h>
#include <Core/Graph/WeaverGraph.h>
#include <Core/Graph/GraphAnalyzer.h>
#include <Story/StoryWriter.h>

namespace weave {
    struct EngineParameters {
        float chooseOldOverNew = 0.9f;
        AnalyzerParameters analyzerParameters;

        template<class Archive>
        void serialize(Archive &ar) {
            ar(CEREAL_NVP(chooseOldOverNew), CEREAL_NVP(analyzerParameters));
        }
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

        EngineResult fillTemplate(std::shared_ptr<QuestTemplate> questTemplate,
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
                                std::map<std::string, std::vector<PropertyCandidate>> candidates) const;

        void addGraphEdges(const QuestModel &questModel, WeaverGraph &graph,
                           std::unordered_set<ID> &candidateIds) const;
    };
}
