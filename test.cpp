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

std::string tolower(std::string n){
    std::string variable;
    variable=n;

    for(int i=0; i<n.size(); i++){
        variable[i]=tolower(n.at(i));
    }
    return variable;
}

class InformationSystem;

class Data{
    public:
        virtual std::string setAPIkey(std::string lan, std::string lon, std::string name){
            std::cout<<"API key is not being set up"<<std::endl;
            exit(1);
        }

        std::string HTTP_Request(std::string a){
            CURL* curl;
            CURLcode res;
            std::string response="";
            const char *api = a.c_str();
            curl = curl_easy_init();
            if (!curl) {
                std::cerr << "Failed to initialize curl" << std::endl;
                exit(1);
            }
            curl_easy_setopt(curl, CURLOPT_URL,
                api);

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
                exit(1);
            }
            curl_easy_cleanup(curl);
            std::cout<<response<<"\n";
            return response;
        }

        virtual void JSON_Data_Parsing(std::string response){
            // Parse JSON
            try {
            json data = json::parse(response);

            // Check API status
            if (data.contains("cod") && data["cod"] != 200) {
                std::cerr << "API Error: " << data["message"] << std::endl;
                exit(1);
                }
            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        virtual void showData(){
            std::cout<<"wrong call for data output"<<std::endl;
            exit(1);
        }
};

class BasicInformation : public Data{
    private:
        std::string APIKey;

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://api.openweathermap.org/data/2.5/weather?lat="+lan+"&lon="+lon+"&appid=f88e5e3d6507f132ce088f99235063fb";
            APIKey=api;
            return APIKey;
        }

        void JSON_Data_Parsing(std::string response) override{
            // Parse JSON
            try {
            json data = json::parse(response);

            // Check API status
            if (data.contains("cod") && data["cod"] != 200) {
                std::cerr << "API Error: " << data["message"] << std::endl;
                exit(1);
                }
            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{

        }
};

class SocialInformation : public Data{
    private:
        std::string APIKey;

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://api.worldbank.org/v2/country/MYS/indicator/SP.POP.TOTL?format=json&date=2018:2026";
            APIKey=api;
            return APIKey;

        }

        void JSON_Data_Parsing(std::string response) override{
            // Parse JSON
            try {
            json data = json::parse(response);

            // Check API status
            if (data.contains("cod") && data["cod"] != 200) {
                std::cerr << "API Error: " << data["message"] << std::endl;
                exit(1);
            }

            std::cout<<response<<std::endl;
            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{
            
        }
};

class EnvironmentInformation : public Data{
    private:
        
        friend class InformationSystem;

        std::string APIKey,
                    city;
        double temp;
        int humidity;
        std::string desc;

        

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://api.openweathermap.org/data/2.5/weather?lat="+lan+"&lon="+lon+"&appid=de98ee3d42280791a2854b08d57c9dc0";
            return api;
        }

        void JSON_Data_Parsing(std::string response) override{
            // Parse JSON
            try {
                json data = json::parse(response);

                // Check API status
                if (data.contains("cod") && data["cod"] != 200) {
                    std::cerr << "API Error: " << data["message"] << std::endl;
                    exit(1);
                }
                
                city = data["name"];
                temp = data["main"]["temp"];
                humidity = data["main"]["humidity"];
                desc = data["weather"][0]["description"];

            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{
            std::cout <<"City: " << city<< "\n";
            std::cout << " Temp: " << temp - 273.15 << " °C\n";
            std::cout << " Humidity: " << humidity << "%\n";
            std::cout <<"Weather Description: "<< desc << "\n";
        }
};

class NewsAndHistoryInformation : public Data{
    private:
        std::string APIKey;

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://api.openweathermap.org/data/2.5/weather?lat="+lan+"&lon="+lon+"&units=metricappid=f88e5e3d6507f132ce088f99235063fb";
            APIKey=api;
            return APIKey;
        }


        void JSON_Data_Parsing(std::string response) override{
            // Parse JSON
            try {
            json data = json::parse(response);

            // Check API status
            if (data.contains("cod") && data["cod"] != 200) {
                std::cerr << "API Error: " << data["message"] << std::endl;
                exit(1);
                }
            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{
            
        }
};

class InformationSystem {
    private:
        double latitude,
               longitude;
        std::string countryName;
        Data *environment = new EnvironmentInformation();
        Data *basic = new BasicInformation();
        Data *news = new NewsAndHistoryInformation();
        Data *social = new SocialInformation();

    public:
        void setLatitude(double lat){
            latitude=lat;
        }

        void setLongitude(double longi){
            longitude=longi;
        }

        void setCountryName(std::string name){
            countryName=tolower(name);
        }

        double getLatitude(){
            double lat;

            return lat;
        }

        double getLongitude(){
            double lon;
            
            return lon;
        }

        std::string getCountryName(){
            std::string country;

            return country;
        }

        void showEnvironmentInformation(){
            std::string API= environment->setAPIkey(std::to_string(latitude), std::to_string(longitude),countryName);
            std::string response=environment->HTTP_Request(API);
            environment->JSON_Data_Parsing(response);
            environment->showData();
        }

        void showSocialInformation(){
            std::string API= social->setAPIkey(std::to_string(latitude), std::to_string(longitude),countryName);
            std::string response=social->HTTP_Request(API);
            std::cout<<response<<std::endl;
            social-> JSON_Data_Parsing(response);
        }
};

int main() {
    std::string response;
    InformationSystem information;
    double lat,
           longi;

    std::cout<<"Please enter the latitude for your location => ";
    std::cin>>lat;
    std::cout<<"Please enter the longitude for your location => ";
    std::cin>>longi;
    
    information.setLatitude(lat);
    information.setLongitude(longi);
    
    information.showEnvironmentInformation();

    system("pause");
    return 0;
}