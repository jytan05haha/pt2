#include <iostream>
#include <string>
#include "curl/curl.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// Callback for libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string response;
    
    double lat,
           longi;

    std::cout<<"Please enter the latitude for your location => ";
    std::cin>>lat;
    std::cout<<"Please enter the longitude for your location => ";
    std::cin>>longi;
    
    std::string lat=std::to_string(lat);
    std::string longi=std::to_string(longi);
    
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl" << std::endl;
        return 1;
    }
    //https://api.openweathermap.org/data/2.5/weather?lat=2.094&lon=102.65&appid=f88e5e3d6507f132ce088f99235063fb
    curl_easy_setopt(curl, CURLOPT_URL,
        "https://api.worldbank.org/v2/country/MYS?format=json");

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Static build: skip SSL verification (for testing)
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    res = curl_easy_perform(curl);
    std::cout<<response;
    if (res != CURLE_OK) {
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return 1;
    }
    curl_easy_cleanup(curl);

    // Parse JSON
    try {
    json data = json::parse(response);

    // Check API status
    if (data.contains("cod") && data["cod"] != 200) {
        std::cerr << "API Error: " << data["message"] << std::endl;
        return 1;
    }

    std::string city = data["name"];
    double temp = data["main"]["temp"];
    int humidity = data["main"]["humidity"];
    std::string desc = data["weather"][0]["description"];

    std::cout << city << "\n";
    std::cout << " Temp: " << temp - 273.15 << " °C\n";
    std::cout << " Humidity: " << humidity << "%\n";
    std::cout << desc << "\n";

    } catch (json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }

    system("pause");
    return 0;
}