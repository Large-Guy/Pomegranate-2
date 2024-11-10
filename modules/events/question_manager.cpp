#include "question_manager.h"

std::unordered_map<QuestionID, std::vector<Function>> Question::_events;
std::unordered_map<std::string, QuestionID> Question::_eventIndex;
QuestionID Question::_eventCounter = 0;

void Question::answer(EventID id, const Function& callback) {
    Question::_events[id].push_back(callback);
}

void Question::answer(const std::string &name, const Function& callback) {
    QuestionID id = Question::getQuestionId(name);
    Question::_events[id].push_back(callback);
}

QuestionID Question::createQuestion() {
    return Question::_eventCounter++;
}

QuestionID Question::getQuestionId(const std::string &name) {
    if(Question::_eventIndex.count(name)) {
        return Question::_eventIndex[name];
    }
    else
    {
        QuestionID id = Question::createQuestion();
        Question::_eventIndex[name] = id;
        return id;
    }
}