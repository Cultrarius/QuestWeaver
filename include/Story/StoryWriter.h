//
// Created by michael on 21.12.15.
//

#pragma once

#include <Core/WeaverUtils.h>
#include <Core/Graph/WeaverGraph.h>
#include <QuestModel/QuestModel.h>
#include <Template/TemplateEngine.h>
#include <Story/StoryTemplateFactory.h>
#include <Story/Nugget.h>

namespace weave {

    typedef std::unordered_map<ID, const QuestPropertyValue *> QuestValueMap;

    /*!
     * @ingroup storyApi
     */
    struct Story {
        std::string text = "";
        std::vector<WorldModelAction> worldActions;
    };

    class StoryWriterParameters {
    public:
        StoryWriterParameters(const WeaverGraph &graph, const std::vector<QuestPropertyValue> &propertyValues) :
                graph(graph), propertyValues(propertyValues) {}

        StoryWriterParameters(const WeaverGraph &graph, const std::vector<QuestPropertyValue> &propertyValues,
                              const std::vector<WorldModelAction> &modelActions) :
                graph(graph), propertyValues(propertyValues), modelActions(modelActions) {}

        WeaverGraph graph;
        std::vector<QuestPropertyValue> propertyValues;
        std::vector<WorldModelAction> modelActions;
    };

    /*!
     * @ingroup storyApi
     */
    class StoryWriter {
    public:
        explicit StoryWriter(std::shared_ptr<RandomStream> randomStream, const QuestModel &questModel,
                             const TemplateEngine &templateEngine, const WorldModel &worldModel,
                             const Directories &dirs);

        Story CreateStory(const StoryWriterParameters &params) const;

        Story CreateStory(const StoryWriterParameters &params,
                          std::string storyTemplateKey) const;

        void ChangeDirectories(const Directories &newDirs);

        void RegisterTemplateFactory(std::unique_ptr<StoryTemplateFactory> factory);

    private:
        const float storyCharWeight = 1;
        const float storyEntityWeight = 40;
        const float worldActionWeight = -20;
        const float nuggetWeight = 5;
        const float unfinishedQuestWeight = 5;
        const float finishedQuestWeight = 10;

        std::shared_ptr<RandomStream> rs;
        const QuestModel &questModel;
        const TemplateEngine &templateEngine;
        const WorldModel &worldModel;
        Directories dirs;
        std::vector<std::unique_ptr<StoryTemplateFactory>> factories;

        mutable bool isInitialized = false;
        mutable std::unordered_map<std::string, Nugget> nuggets;

        void initialize() const;

        void readNuggets() const;

        void checkValidNuggetJson(Json::Value root, std::string filePath) const;

        bool hasAll(std::set<std::string> requiredEntities,
                    const std::vector<QuestPropertyValue> &propertyValues) const;

        Story CreateStory(const StoryWriterParameters &params,
                          std::unordered_set<std::string> storyTemplateKeys) const;

        std::vector<std::shared_ptr<StoryTemplate>> getFittingTemplates(
                const std::vector<QuestPropertyValue> &propertyValues,
                const std::unordered_set<std::string> &storyTemplateKeys) const;

        EntityMap getPossibleEntitiesForTemplate(const std::shared_ptr<StoryTemplate> &storyTemplate,
                                                 const EntityMap &entitiesByType) const;

        std::map<float, Story> createWeightedStories(
                const WeaverGraph &graph,
                const std::vector<std::shared_ptr<StoryTemplate>> &templates,
                const EntityMap &entitiesByType,
                const QuestValueMap &questValues) const;

        std::vector<NuggetOption> getSupportedNuggets(const std::vector<NuggetOption> &nuggetOptions,
                                                      const QuestValueMap &questValues) const;

        std::string getNuggetText(const QuestValueMap &questValues, const NuggetOption &chosenOption) const;

        void removeStoriesWithInvalidActions(const StoryWriterParameters &params,
                                             std::map<float, Story> *storyMap) const;

        void readRandomizedValues(const Json::Value &nuggetJson, std::unordered_map<std::string, int> &minValues,
                                  std::unordered_map<std::string, int> &maxValues) const;

        void readRandomizedNames(const Json::Value &nuggetJson,
                                 std::unordered_map<std::string, NameType> &randomNames) const;

        uint64_t getNuggetIndexByRarity(const std::vector<NuggetOption> &supportedNuggets) const;
    };
}
