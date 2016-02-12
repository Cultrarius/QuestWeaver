#include <Template/Space/SpaceQuestTemplateFactory.h>
#include <World/Space/SpaceWorldModel.h>
#include "QuestWeaver.h"

using namespace std;
using namespace weave;
using namespace cereal;

int main() {
	WeaverConfig config;
    config.formatterType = FormatterType::HTML;
    config.worldModel = std::make_unique<SpaceWorldModel>();
    shared_ptr<QuestTemplateFactory> factory = make_shared<SpaceQuestTemplateFactory>();
    config.questTemplateFactories.push_back(factory);

    QuestWeaver weaver(config);
    shared_ptr<Quest> newQuest = weaver.CreateNewQuest();
    cout << "New Quest created!" << endl;
    cout << "Title: " << newQuest->GetTitle() << endl;
    cout << "Description: " << newQuest->GetDescription() << endl;

    weaver.Tick(1);

    stringstream ss;
    weaver.Serialize(ss, StreamType::BINARY);
    cout << ss.str() << endl;

    cout << "Hey!" << endl;

    QuestWeaver deserialized = QuestWeaver::Deserialize(ss, StreamType::BINARY, config.dirs);

    deserialized.RegisterQuestTemplateFactory(factory);
    cout << "Woot!" << endl;

    shared_ptr<Quest> desQuest = deserialized.GetQuest(newQuest->GetId());

    cout << "Title: " << desQuest->GetTitle() << endl;
    cout << "Description: " << desQuest->GetDescription() << endl;

    newQuest = deserialized.CreateNewQuest();
    cout << "New Quest created!" << endl;
    cout << "Title: " << newQuest->GetTitle() << endl;
    cout << "Description: " << newQuest->GetDescription() << endl;

    cout << weaver.GetWorldModel().GetEntities().size() << endl;
    cout << deserialized.GetWorldModel().GetEntities().size() << endl;
    shared_ptr<WorldListener> outputListener;
    deserialized.GetWorldModel().AddListener(outputListener);

    return 0;
}
