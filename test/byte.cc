#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

struct Message {
    std::string sender;
    int type;
    std::vector<std::uint8_t> payload;
};

// 编码
std::string encode_message(const Message& message) {
    std::stringstream ss;

    // Write the sender field to the stringstream
    ss << message.sender;

    // Write the type field to the stringstream
    ss.write(reinterpret_cast<const char*>(&message.type), sizeof(message.type));

    // Write the payload field to the stringstream
    ss.write(reinterpret_cast<const char*>(message.payload.data()), message.payload.size());

    // Return the contents of the stringstream as a string
    return ss.str();
}

// 解码
Message decode_message(const std::string& str) {
    std::stringstream ss(str);
    Message message;

    // Read the sender field from the stringstream
    ss >> message.sender;

    // Read the type field from the stringstream
    ss.read(reinterpret_cast<char*>(&message.type), sizeof(message.type));

    // Read the payload field from the stringstream
    message.payload.resize(str.size() - ss.tellg());
    ss.read(reinterpret_cast<char*>(message.payload.data()), message.payload.size());

    return message;
}

void show_message(const Message& message) {
    std::cout << "Sender: " << message.sender << std::endl;
    std::cout << "Type: " << message.type << std::endl;
    std::cout << "Payload: [";
    for (auto byte : message.payload) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cout << "]" << std::endl;

}

int main() {
    // 将此对象编码
    Message message {
        "Alice", // sender
        1,       // type
        { 0x01, 0x02, 0x03 } // payload
    };

    std::string str = encode_message(message);
    Message receiver = decode_message(str);
    show_message(receiver);

    return 0;   
}
