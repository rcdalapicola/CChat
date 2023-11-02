#include "message.h"

#include <memory>
#include <string.h>


Message::Message() {
    mUser[USER_BUFFER_SIZE - 1] = '\0';
    mContent[CONTENT_BUFFER_SIZE - 1] = '\0';
}

Message::Message(const char* serializedMessage) : Message() {
    deserializeMessage(serializedMessage);
}

const char* Message::serializeMessage() const {
    return reinterpret_cast<const char*>(this);
}; 

void Message::user(const char* user) {
    strncpy(mUser, user, USER_BUFFER_SIZE - 1);
}

void Message::content(const char* content) {
    strncpy(mContent, content, CONTENT_BUFFER_SIZE - 1);
}

const char* Message::getUser() const {
    return mUser;
}

const char* Message::getContent() const {
    return mContent;
}

void Message::deserializeMessage(const char* message) {
    const Message* deserializedMessage = reinterpret_cast<const Message*>(message);
    user(deserializedMessage->getUser());
    content(deserializedMessage->getContent());
}
