#ifndef MESSAGE_H
#define MESSAGE_H

constexpr int MESSAGE_USER_BUFFER_SIZE = 64;
constexpr int MESSAGE_CONTENT_BUFFER_SIZE = 512 - MESSAGE_USER_BUFFER_SIZE;
constexpr int MESSAGE_TOTAL_BUFFER_SIZE = MESSAGE_USER_BUFFER_SIZE + MESSAGE_CONTENT_BUFFER_SIZE;

class Message {
public:
    Message();
    Message(const char* serializedMessage);

    const char* serializeMessage() const; 
    void deserializeMessage(const char*);

    void user(const char* user);
    const char* getUser() const;

    void content(const char* content);
    const char* getContent() const;

private:
    char mUser[MESSAGE_USER_BUFFER_SIZE];
    char mContent[MESSAGE_CONTENT_BUFFER_SIZE];
};

#endif //MESSAGE_H
