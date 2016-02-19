//
// Created by michael on 14.08.15.
//

#pragma once

#include "../QuestModel/Quest.h"
#include "../World/WorldModel.h"
#include "../Core/WeaverUtils.h"
#include "../Core/WeaverTypes.h"

namespace weave {

    /*!
     * Marks parts of a quest template that represent a WorldEntity.
     *
     * The quest system usually has to choose between multiple possible entities for each quest property.
     * Instances of this class allow the quest system to tell the template which entity should be used for which part
     * of a quest.
     *
     * @ingroup questApi
     */
    class TemplateQuestProperty {
    public:
        /*!
         * Creates a new quest property with the given name.
         *
         * @param isMandatory If true, then the quest template is unable to create a quest without this property.
         * Otherwise, this property is optional.
         * @param name The name of this quest property.
         */
        TemplateQuestProperty(bool isMandatory, const std::string &name) noexcept;

        /*!
         * Returns true if the quest template is unable to create a quest without this property.
         */
        bool IsMandatory() const noexcept;

        /*!
         * Returns the name of this quest property.
         */
        std::string GetName() const noexcept;

    private:
        bool isMandatory;
        std::string name;
    };


    /*!
     * Data container linking quest templates and world entities.
     *
     * Each quest template requires a number of world entities, which it describes with TemplateQuestProperty values.
     * Once the quest system has found a suitable WorldEntity for such a value, it creates a QuestPropertyValue object.
     */
    class QuestPropertyValue {
    public:
        /*!
         * Creates a new instance linking the given quest property and world entity.
         *
         * @param property the quest property this object describes
         * @param value the world entity to use for the quest property
         */
        QuestPropertyValue(const TemplateQuestProperty &property, std::shared_ptr<WorldEntity> value) noexcept;

        /*!
         * Returns the quest property value described by this object.
         */
        TemplateQuestProperty GetProperty() const noexcept;

        /*!
         * Returns the world entity to use for this object's quest property.
         */
        std::shared_ptr<WorldEntity> GetValue() const noexcept;

        /*!
         * Returns a formatted string version of the WorldEntity contained in this object.
         */
        std::string GetValueString(FormatterType format) const noexcept;

    private:
        TemplateQuestProperty property;
        std::shared_ptr<WorldEntity> value;
    };


    /*!
     * Wraps the quest description texts for the QuestTemplate.
     * Contains a text from the template files and the required conditions for the text to appear.
     */
    class TemplateQuestDescription {
    public:
        /*!
         * Creates a new description that is valid for the given conditions.
         *
         * @param conditions The names of the TemplateQuestProperty values required for the description to be used.
         * @param text the description text containing placeholders for the conditions.
         * A placeholder is a string with a leading %-sign.
         * When the quest is created, each placeholder will be substituted with the corresponding QuestPropertyValue.
         */
        TemplateQuestDescription(const std::vector<std::string> &conditions, const std::string &text) noexcept;

        /*!
         * Returns true if this description text can be constructed under the given conditions.
         * @param conditions the **sorted** list of conditions.
         */
        bool SupportsConditions(const std::vector<std::string> &conditions) const noexcept;

        /*!
         * Returns the description text containing placeholders for the corresponding QuestPropertyValue.
         */
        std::string GetText(FormatterType format) const noexcept;

    private:
        std::vector<std::string> descriptionConditions;
        std::string text;
    };


    /*!
     * Defines a blueprint from which a specific type of quest can be generated.
     * Each template contains the information necessary to create variations of a specific quest.
     *
     * The template data itself is kept in JSON format in a file and loaded via the QuestTemplateFactory.
     *
     * ### Example
     * A template might define the variables and entities necessary to create a "fetch" quest.
     * Some of these entities might be optional, e.g. who gave the player the quest.
     * Some of them are mandatory, e.g. what the player has to fetch.
     *
     * A quest generated from such a template might be "Get the king's crown from the dragon" or
     * "fetch 10 eggs to make breakfast", depending on the entities the world model creates.
     */
    class QuestTemplate {
    public:
        /*!
         * Creates a new quest template.
         * @param title The quest title (can contain placeholders for *mandatory* properties).
         * @param properties The properties used by the quest system to select world entities for the quest.
         * The names of these properties will also be used when substituting the placeholders in the title and
         * description texts.
         * @param descriptions The possible description texts. One of these descriptions will be used to create the
         * quest description, depending on the optional properties used by the quest system.
         * @param formatterType The format used when creating the texts for a new quest.
         */
        QuestTemplate(std::string title,
                      std::vector<TemplateQuestProperty> properties,
                      std::vector<TemplateQuestDescription> descriptions,
                      FormatterType formatterType);

        virtual ~QuestTemplate() = default;

        /*!
         * Creates a new quest based on this template and the given parameters.
         *
         * @param questPropertyValues the values for the template's mandatory and optional properties.
         * @param questStory the quest's background story.
         */
        virtual std::shared_ptr<Quest> ToQuest(const std::vector<QuestPropertyValue> &questPropertyValues,
                                               const std::string &questStory) const = 0;

        /*!
         * Creates a new quest based on this template and the given parameters.
         * The resulting quest has no background story.
         *
         * @param questPropertyValues the values for the template's mandatory and optional properties.
         */
        std::shared_ptr<Quest> ToQuest(const std::vector<QuestPropertyValue> &questPropertyValues) const;

        /*!
         * This method creates a list of possible candidates for a given template property.
         * Subclasses implementing this method may search the world model for fitting entities or they may use
         * the world model to create new entities.
         *
         * For every candidate, the template may add some metadata (e.g. relationship changes caused by the quest
         * or markers to not choose the entity for another quest). If a candidate is chosen from the list,
         * the corresponding WorldModelAction is executed in the world model.
         */
        virtual std::vector<WorldModelAction> GetPropertyCandidates(const TemplateQuestProperty &property,
                                                                    const WorldModel &worldModel) const = 0;

        /*!
         * Returns a list of all the template parts that (mandatory) must be filled and (optional) can be filled with
         * world entities.
         * Which entities can be used to fill a cartain TemplateQuestProperty is determined by the template itself, by
         * the GetPropertyCandidates() method.
         */
        std::vector<TemplateQuestProperty> GetProperties() const noexcept;

    protected:

        /*!
         * Returns the first description that supports all of the given property values.
         */
        std::string getBestFittingDescription(const std::vector<QuestPropertyValue> &questPropertyValues) const;

        /*!
         * Returns the quest title and uses the given property values to replace any placeholders.
         */
        std::string getTitle(const std::vector<QuestPropertyValue> &questPropertyValues) const noexcept;

        /*!
         * Returns the ID of the entitiy referenced by the property with the given name or returns 0 if no property
         * with such a name can be found.
         */
        static ID getEntityIdFromProperty(std::string propertyName,
                                          const std::vector<QuestPropertyValue> &questPropertyValues) noexcept;

    private:
        std::string title;
        std::vector<TemplateQuestProperty> properties;
        std::vector<TemplateQuestDescription> descriptions;
        FormatterType formatterType;
    };
}
