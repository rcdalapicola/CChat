#ifndef MESSAGE_H
#define MESSAGE_H

constexpr int USER_BUFFER_SIZE = 64;
constexpr int CONTENT_BUFFER_SIZE = 512 - USER_BUFFER_SIZE;

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
    char mUser[USER_BUFFER_SIZE];
    char mContent[CONTENT_BUFFER_SIZE];
};

#endif //MESSAGE_H
