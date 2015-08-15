//
// Created by michael on 15.08.15.
//

#include "TemplateFactory.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace Json;

Json::Value TemplateFactory::readTemplateFile(const char *fileName) {
    Value root;
    CharReaderBuilder readBuilder;
    readBuilder["collectComments"] = false;
    readBuilder["rejectDupKeys"] = true;
    string errorMessage;
    ifstream inStream;

    inStream.open(fileName);
    if (!Json::parseFromStream(readBuilder, inStream, &root, &errorMessage)) {
        cerr << "Error parsing template file: " << errorMessage;
    } else {
        cout << "Success parsing file! Template Key: " << root["key"].asString() << endl;
        //TODO check root value for consistency
        const Json::Value descriptions = root["descriptions"];
        for (int i = 0; i < descriptions.size(); ++i) {
            const Json::Value description = descriptions[i];
            cout << description["text"].asString() << endl;
        }
    }
    return root;
}

TemplateFactory::TemplateFactory() {

}
