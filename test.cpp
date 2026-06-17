#include <iostream>
#include <string>
#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include <vector>
#include <algorithm>
#include <cctype>

using json = nlohmann::json;

struct Currency {
    std::string code;
    std::string name;
    std::string symbol;
};
// Callback for libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp){
    size_t totalSize = size * nmemb;

    std::string* output = static_cast<std::string*>(userp);
    output->append(static_cast<char*>(contents), totalSize);

    return totalSize;
}
int getIntSafe(const json& obj, const std::string& key, int defaultValue = 0){
    if (obj.contains(key) && !obj[key].is_null() && obj[key].is_number_integer())
    {
        return obj[key].get<int>();
    }

    return defaultValue;
}

double getDoubleSafe(const json& obj, const std::string& key, double defaultValue = 0.0){
    if (obj.contains(key) && !obj[key].is_null() && obj[key].is_number())
    {
        return obj[key].get<double>();
    }

    return defaultValue;
}

long long getLongLongSafe(const json& obj, const std::string& key, long long defaultValue = 0)
{
    if (obj.contains(key) && !obj[key].is_null() && obj[key].is_number_integer())
    {
        return obj[key].get<long long>();
    }

    return defaultValue;
}

std::string getStringSafe(const json& obj, const std::string& key, const std::string& defaultValue = "N/A"){
    if (obj.contains(key) && !obj[key].is_null() && obj[key].is_string())
    {
        return obj[key].get<std::string>();
    }

    return defaultValue;
}

std::string toLower(std::string str){
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    return str;
}

class InformationSystem;

class Data{
    public:
        virtual std::string setAPIkey(std::string lan, std::string lon, std::string name){
            std::cout<<"API key is not being set up"<<std::endl;
            exit(1);
        }

        std::string HTTP_Request(std::string apiUrl){
            CURL* curl;
            CURLcode res;
            std::string response = "";

            curl = curl_easy_init();

            if (!curl)
            {
                std::cerr << "Failed to initialize curl" << std::endl;
                exit(1);
            }

            curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());

            curl_easy_setopt(curl, CURLOPT_USERAGENT,
                            "CountryInfoApp/1.0 (student project)");

            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK)
            {
                std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
                curl_easy_cleanup(curl);
                exit(1);
            }

            curl_easy_cleanup(curl);

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
        std::string APIKey,
                    countryName;
        std::vector<std::string> languages;
        std::vector<Currency> currencies;
        double area;

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://restcountries.com/v3.1/name/" + name +
             "?fields=name,area,languages,currencies";
            APIKey=api;
            return APIKey;
        }

        void JSON_Data_Parsing(std::string response) override{
            try
            {
                if (!response.empty() && response[0] == '<')
                {
                    std::cerr << "Error: API returned HTML instead of JSON.\n";
                    std::cerr << "Your API URL may be wrong or redirected.\n";
                    return;
                }

                json data = json::parse(response);

                if (!data.is_array() || data.empty())
                {
                    std::cout << "No country data found.\n";
                    return;
                }

                json country = data[0];

                area = getDoubleSafe(country, "area");

                languages.clear();
                currencies.clear();

                if (country.contains("languages") && country["languages"].is_object())
                {
                    for (auto& item : country["languages"].items())
                    {
                        if (item.value().is_string())
                        {
                            languages.push_back(item.value().get<std::string>());
                        }
                    }
                }

                if (country.contains("currencies") && country["currencies"].is_object())
                {
                    for (auto& item : country["currencies"].items())
                    {
                        Currency currency;

                        currency.code = item.key();
                        currency.name = getStringSafe(item.value(), "name");
                        currency.symbol = getStringSafe(item.value(), "symbol");

                        currencies.push_back(currency);
                    }
                }
            }
            catch (json::exception& e)
            {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{
            std::cout<<countryName<<std::endl;
            std::cout<<area<<std::endl;
            std::cout << "Languages: ";

            if (languages.empty()){
                std::cout << "N/A";
            } else{
                for (int i = 0; i < languages.size(); i++)
                {
                    std::cout << languages[i];

                    if (i != languages.size() - 1){
                        std::cout << ", ";
                    }
                }
            }

            std::cout << std::endl;

            std::cout << "Currencies: ";

            if (currencies.empty())
            {
                std::cout << "N/A";
            }
            else
            {
                for (int i = 0; i < currencies.size(); i++)
                {
                    std::cout << currencies[i].code << " - "
                            << currencies[i].name << " ("
                            << currencies[i].symbol << ")";

                    if (i != currencies.size() - 1)
                    {
                        std::cout << ", ";
                    }
                }
            }

            std::cout << std::endl;
        }
};

class SocialInformation : public Data{
    private:
        std::string APIKey;

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://api.worldbank.org/v2/country/MYS/indicator/SP.POP.TOTL?format=json&date=2018:2024&per_page=1000&source=2";
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
                    weather;
        double temp,
               tempFeels,
               windSpeed,
               highTemp;
        int windDegree,
            visibility,
            humidity,
            cloud_P,
            gP,
            sP;
        long long sunrise,
                  sunset;
        
        

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://api.openweathermap.org/data/2.5/weather?lat="+lan+"&lon="+lon+"&appid=de98ee3d42280791a2854b08d57c9dc0&units=metric";
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
                
                weather= data["weather"][0]["main"];
                temp=data["main"]["temp"];
                tempFeels=data["main"]["feels_like"];
                windSpeed=data["wind"]["speed"];
                highTemp=data["main"]["humidity"];
                windDegree=data["wind"]["deg"];
                visibility=data["visibility"].get<int>()/1000;
                humidity=data["main"]["humidity"];
                cloud_P=data["clouds"]["all"];
                gP=data["main"]["grnd_level"];
                sP=data["main"]["sea_level"];
                sunrise=data["sys"]["sunrise"];
                sunset=data["sys"]["sunset"];

            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{
            std::cout <<weather<< "\n";
            std::cout <<temp<< "\n";
            std::cout <<tempFeels<< "\n";
            std::cout <<windSpeed<< "\n";
            std::cout <<highTemp<< "\n";
            std::cout <<windDegree<< "\n";
            std::cout <<visibility<< "\n";
            std::cout <<humidity<< "\n";
            std::cout <<cloud_P<< "\n";
            std::cout <<gP<< "\n";
            std::cout <<sP<< "\n";
            std::cout <<sunrise<< "\n";
            std::cout <<sunset<< "\n";
        }
};

class HistoryInformation : public Data{
    private:
        std::string APIKey,
                    title[3],
                    source[3],
                    description[3],
                    published[3],
                    url[3];

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://newsapi.org/v2/everything?q="+name+"&language=en&sortBy=publishedAt&pageSize=5&apiKey=21045aad20f3482c810a0e9e212f7ab3";
            
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
            
                for(int i=0; i<3; i++){
                    title[i]= data["articles"][i]["title"];
                    source[i]=data["articles"][i]["source"]["name"];
                    description[i]=data["articles"][i]["description"];
                    published[i]=data["articles"][i]["publishedAt"];
                    url[i]=data["articles"][i]["url"];
                }

            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{
            
        }
};

class NewsInformation : public Data{
    private:
        std::string APIKey,
                    title[3],
                    source[3],
                    description[3],
                    published[3],
                    url[3];

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://newsapi.org/v2/everything?q="+name+"&language=en&sortBy=publishedAt&pageSize=3&apiKey=21045aad20f3482c810a0e9e212f7ab3";
            
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
            
                for(int i=0; i<3; i++){
                    title[i]= data["articles"][i]["title"];
                    source[i]=data["articles"][i]["source"]["name"];
                    description[i]=data["articles"][i]["description"];
                    published[i]=data["articles"][i]["publishedAt"];
                    url[i]=data["articles"][i]["url"];
                }

            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{
            for(int i=0; i<3; i++){
                    std::cout<<title[i]<<std::endl;
                    std::cout<<source[i]<<std::endl;
                    std::cout<<description[i]<<std::endl;
                    std::cout<<published[i]<<std::endl;
                    std::cout<<url[i]<<std::endl;
            }
        }
};


class InformationSystem {
    private:
        double latitude,
               longitude;
        std::string countryName;
        Data *environment = new EnvironmentInformation();
        Data *basic = new BasicInformation();
        Data *news = new NewsInformation();
        Data *history= new HistoryInformation();
        Data *social = new SocialInformation();

    public:
        void setLatitude(double lat){
            latitude=lat;
        }

        void setLongitude(double longi){
            longitude=longi;
        }

        void setCountryName(std::string name){
            countryName=toLower(name);
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
            return countryName;
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

        void showNewsInformation(){
            std::string API= news->setAPIkey(std::to_string(latitude), std::to_string(longitude),countryName);
            std::string response=news->HTTP_Request(API);
            std::cout<<response<<std::endl;
            news-> JSON_Data_Parsing(response);
            news->showData();
        }

        void showBasicInformation(){
            std::string API= basic->setAPIkey(std::to_string(latitude), std::to_string(longitude),countryName);
            std::string response=basic->HTTP_Request(API);
            std::cout<<response<<std::endl;
            basic-> JSON_Data_Parsing(response);
            basic->showData();
        }
};

int main() {
    std::string response;
    InformationSystem information;
    double lat,
           longi;
    std::string name;

    std::cout<<"Please enter the latitude for your location => ";
    std::cin>>lat;
    std::cout<<"Please enter the longitude for your location => ";
    std::cin>>longi;
    std::cout<<"Country name => ";
    std::cin>>name;

    information.setLatitude(lat);
    information.setLongitude(longi);
    information.setCountryName(name);

    information.showBasicInformation();

    return 0;
}