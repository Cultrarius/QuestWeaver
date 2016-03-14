//
// Created by Michael on 10.02.2016.
//

#include "TestQuestTemplateFactory.h"
#include "TestQuestTemplate.h"

using namespace weave;
using namespace std;

TestQuestTemplateFactory::TestQuestTemplateFactory(string templateFile) : templateFile(templateFile) {
}

shared_ptr<QuestTemplate> TestQuestTemplateFactory::createFromJsonValues(const Json::Value &) const {
    vector<TemplateQuestProperty> properties;
    vector<TemplateQuestDescription> descriptions;
    return make_shared<TestQuestTemplate>(properties, descriptions);
}

const char *TestQuestTemplateFactory::getTemplateFile() const {
    return templateFile.c_str();
}
