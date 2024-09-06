# EmbedCalendar


```
#include <curl/curl.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<bool> running(true);

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string data(ptr, size * nmemb);
    std::cout << "Received: " << data << std::endl;
    return size * nmemb;
}

void receiveMessages(CURL* curl) {
    curl_multi_perform(curl, NULL);
    CURLM* multi_handle = curl_multi_init();
    curl_multi_add_handle(multi_handle, curl);
    
    int still_running = 1;
    while (running) {
        curl_multi_perform(multi_handle, &still_running);
        if (still_running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    curl_multi_cleanup(multi_handle);
}

void sendMessage(CURL* curl, const std::string& message) {
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
    curl_easy_perform(curl);
}

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "ws://your.websocket.server");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        std::thread receiverThread(receiveMessages, curl);
        
        std::string input;
        while (true) {
            std::cout << "Enter message to send (or 'exit' to quit): ";
            std::getline(std::cin, input);
            
            if (input == "exit") {
                running = false;
                break;
            }

            sendMessage(curl, input);
        }

        receiverThread.join();
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}
```