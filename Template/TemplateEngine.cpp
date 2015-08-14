//
// Created by michael on 10.08.15.
//

#include <iostream>
#include <fstream>
#include "TemplateEngine.h"
#include "../json/json.h"

using namespace Json;
using namespace std;

Template TemplateEngine::GetTemplateForNewQuest() {
    Value root;
    CharReaderBuilder readBuilder;
    readBuilder["collectComments"] = false;
    readBuilder["rejectDupKeys"] = true;
    string errorMessage;
    ifstream inStream;
    inStream.open("testTemplate.qt");
    if (!Json::parseFromStream(readBuilder, inStream, &root, &errorMessage)) {
        cerr << "Error parsing template file: " << errorMessage;
    } else {
        cout << "Success parsing file! Template Key: " << root["key"].asString() << endl;
        const Json::Value descriptions = root["descriptions"];
        for (int i = 0; i < descriptions.size(); ++i) {
            const Json::Value description = descriptions[i];
            cout << description["text"].asString() << endl;
        }
    }
    return Template();
}
