#include <iostream>
#include <string>
#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include <vector>
#include <algorithm>
#include <cctype>
#include <map>
#include <iomanip>

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

std::string urlEncode(const std::string& value){
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        return value;
    }

    char* encoded = curl_easy_escape(curl, value.c_str(), static_cast<int>(value.length()));
    std::string result = encoded ? encoded : value;

    if (encoded)
    {
        curl_free(encoded);
    }

    curl_easy_cleanup(curl);

    return result;
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
                    requestedCountryName,
                    countryName;
        std::vector<std::string> languages;
        std::vector<Currency> currencies;
        double area = 0.0;

        bool isRequestedCountry(const json& country){
            std::string requestedName = toLower(requestedCountryName);

            if (country.contains("name") && country["name"].is_object())
            {
                std::string commonName = toLower(getStringSafe(country["name"], "common", ""));
                std::string officialName = toLower(getStringSafe(country["name"], "official", ""));

                if (requestedName == commonName || requestedName == officialName)
                {
                    return true;
                }
            }

            if (requestedName == toLower(getStringSafe(country, "cca2", "")) ||
                requestedName == toLower(getStringSafe(country, "cca3", "")) ||
                requestedName == toLower(getStringSafe(country, "cioc", "")))
            {
                return true;
            }

            if (country.contains("altSpellings") && country["altSpellings"].is_array())
            {
                for (const auto& spelling : country["altSpellings"])
                {
                    if (spelling.is_string() && requestedName == toLower(spelling.get<std::string>()))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            requestedCountryName = name;
            std::string api="https://raw.githubusercontent.com/mledoze/countries/master/countries.json";
            APIKey=api;
            return APIKey;
        }

        void JSON_Data_Parsing(std::string response) override{
            try
            {
                json data = json::parse(response);

                if (!data.is_array() || data.empty())
                {
                    std::cout << "No country data found.\n";
                    return;
                }

                const json* matchedCountry = nullptr;

                for (const auto& country : data)
                {
                    if (isRequestedCountry(country))
                    {
                        matchedCountry = &country;
                        break;
                    }
                }

                if (matchedCountry == nullptr)
                {
                    std::cout << "No country data found for " << requestedCountryName << ".\n";
                    return;
                }

                const json& country = *matchedCountry;

                countryName = getStringSafe(country["name"], "common", requestedCountryName);
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
            std::cout<<"Basic Information"<<std::endl;
            std::cout<<"Country: "<<countryName<<std::endl;
            std::cout<<"Area: "<<area<<" km^2"<<std::endl;
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
        static const int START_YEAR = 2018;
        static const int END_YEAR = 2024;
        static const int TARGET_YEAR = 2024;

        std::map<int, long long> totalPopulation;
        std::map<int, long long> childYouthPopulation;
        std::map<int, long long> workingAgePopulation;
        std::map<int, long long> elderlyPopulation;

        double childYouthPercentage = 0.0;
        double workingAgePercentage = 0.0;
        double elderlyPercentage = 0.0;
        double birthRate = 0.0;
        double deathRate = 0.0;
        double lifeExpectancyAtBirth = 0.0;
        double secondaryEducationEnrollmentRate = 0.0;
        double tertiaryEducationEnrollmentRate = 0.0;

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://api.worldbank.org/v2/country/MYS/indicator/"
                            "SP.POP.TOTL;"
                            "SP.POP.0014.TO;"
                            "SP.POP.1564.TO;"
                            "SP.POP.65UP.TO;"
                            "SP.POP.0014.TO.ZS;"
                            "SP.POP.1564.TO.ZS;"
                            "SP.POP.65UP.TO.ZS;"
                            "SP.DYN.CBRT.IN;"
                            "SP.DYN.CDRT.IN;"
                            "SP.DYN.LE00.IN;"
                            "SE.SEC.ENRL.TC.ZS;"
                            "SE.TER.ENRL.TC.ZS"
                            "?format=json&date=2018:2024&per_page=1000&source=2";
            APIKey=api;
            return APIKey;

        }

        void JSON_Data_Parsing(std::string response) override{
            try {
                json data = json::parse(response);

                if (!data.is_array() || data.size() < 2 || !data[1].is_array())
                {
                    std::cerr << "No social data found." << std::endl;
                    return;
                }

                totalPopulation.clear();
                childYouthPopulation.clear();
                workingAgePopulation.clear();
                elderlyPopulation.clear();

                for (const auto& item : data[1])
                {
                    if (!item.contains("indicator") || !item["indicator"].contains("id") ||
                        item["indicator"]["id"].is_null() || item["date"].is_null() ||
                        item["value"].is_null())
                    {
                        continue;
                    }

                    std::string indicatorCode = item["indicator"]["id"].get<std::string>();
                    int year = std::stoi(item["date"].get<std::string>());
                    double value = item["value"].get<double>();

                    if (indicatorCode == "SP.POP.TOTL" && year >= START_YEAR && year <= END_YEAR)
                    {
                        totalPopulation[year] = static_cast<long long>(value);
                    }
                    else if (indicatorCode == "SP.POP.0014.TO" && year >= START_YEAR && year <= END_YEAR)
                    {
                        childYouthPopulation[year] = static_cast<long long>(value);
                    }
                    else if (indicatorCode == "SP.POP.1564.TO" && year >= START_YEAR && year <= END_YEAR)
                    {
                        workingAgePopulation[year] = static_cast<long long>(value);
                    }
                    else if (indicatorCode == "SP.POP.65UP.TO" && year >= START_YEAR && year <= END_YEAR)
                    {
                        elderlyPopulation[year] = static_cast<long long>(value);
                    }
                    else if (year == TARGET_YEAR)
                    {
                        if (indicatorCode == "SP.POP.0014.TO.ZS")
                        {
                            childYouthPercentage = value;
                        }
                        else if (indicatorCode == "SP.POP.1564.TO.ZS")
                        {
                            workingAgePercentage = value;
                        }
                        else if (indicatorCode == "SP.POP.65UP.TO.ZS")
                        {
                            elderlyPercentage = value;
                        }
                        else if (indicatorCode == "SP.DYN.CBRT.IN")
                        {
                            birthRate = value;
                        }
                        else if (indicatorCode == "SP.DYN.CDRT.IN")
                        {
                            deathRate = value;
                        }
                        else if (indicatorCode == "SP.DYN.LE00.IN")
                        {
                            lifeExpectancyAtBirth = value;
                        }
                        else if (indicatorCode == "SE.SEC.ENRL.TC.ZS")
                        {
                            secondaryEducationEnrollmentRate = value;
                        }
                        else if (indicatorCode == "SE.TER.ENRL.TC.ZS")
                        {
                            tertiaryEducationEnrollmentRate = value;
                        }
                    }
                }
            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            } catch (std::exception& e) {
                std::cerr << "Social data parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{
            std::cout << "Social Information" << std::endl;
            std::cout << "Total population (2018-2024):" << std::endl;
            for (int year = END_YEAR; year >= START_YEAR; year--)
            {
                std::cout << year << ": " << totalPopulation[year] << std::endl;
            }

            std::cout << "Children/youth population, ages 0-14 (2018-2024):" << std::endl;
            for (int year = END_YEAR; year >= START_YEAR; year--)
            {
                std::cout << year << ": " << childYouthPopulation[year] << std::endl;
            }

            std::cout << "Working-age population, ages 15-64 (2018-2024):" << std::endl;
            for (int year = END_YEAR; year >= START_YEAR; year--)
            {
                std::cout << year << ": " << workingAgePopulation[year] << std::endl;
            }

            std::cout << "Elderly population, ages 65 and above (2018-2024):" << std::endl;
            for (int year = END_YEAR; year >= START_YEAR; year--)
            {
                std::cout << year << ": " << elderlyPopulation[year] << std::endl;
            }

            std::cout << "Children/youth share of population (%), 2024: " << childYouthPercentage << std::endl;
            std::cout << "Working-age share of population (%), 2024: " << workingAgePercentage << std::endl;
            std::cout << "Elderly share of population (%), 2024: " << elderlyPercentage << std::endl;
            std::cout << "Birth rate (per 1,000 people), 2024: " << birthRate << std::endl;
            std::cout << "Death rate (per 1,000 people), 2024: " << deathRate << std::endl;
            std::cout << "Life expectancy at birth, total, 2024: " << lifeExpectancyAtBirth << std::endl;
            std::cout << "Secondary education enrollment rate (%), 2024: " << secondaryEducationEnrollmentRate << std::endl;
            std::cout << "Tertiary education enrollment rate (%), 2024: " << tertiaryEducationEnrollmentRate << std::endl;
        }
};

class EconomicInformation : public Data{
    private:
        std::string APIKey;
        static const int START_YEAR = 2018;
        static const int END_YEAR = 2024;
        static const int TARGET_YEAR = 2024;

        std::map<int, double> gdpCurrentUsd;
        std::map<int, double> gdpPerCapitaCurrentUsd;
        std::map<int, double> gdpAnnualGrowthRate;

        double exportsOfGoodsAndServicesPercentGDP = 0.0;
        double importsOfGoodsAndServicesPercentGDP = 0.0;
        double inflationConsumerPricesAnnualPercent = 0.0;
        double centralGovernmentExternalDebtCurrentUsd = 0.0;

        bool hasExportsOfGoodsAndServicesPercentGDP = false;
        bool hasImportsOfGoodsAndServicesPercentGDP = false;
        bool hasInflationConsumerPricesAnnualPercent = false;
        bool hasCentralGovernmentExternalDebtCurrentUsd = false;

        void showYearlyData(const std::string& title, const std::map<int, double>& values){
            std::cout << title << " (2018-2024):" << std::endl;

            for (int year = END_YEAR; year >= START_YEAR; year--)
            {
                std::cout << year << ": ";

                auto item = values.find(year);
                if (item == values.end())
                {
                    std::cout << "N/A";
                }
                else
                {
                    std::cout << item->second;
                }

                std::cout << std::endl;
            }
        }

        void showTargetYearData(const std::string& title, double value, bool hasValue){
            std::cout << title << ", 2024: ";

            if (hasValue)
            {
                std::cout << value;
            }
            else
            {
                std::cout << "N/A";
            }

            std::cout << std::endl;
        }

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://api.worldbank.org/v2/country/MYS/indicator/"
                            "NY.GDP.MKTP.CD;"
                            "NY.GDP.PCAP.CD;"
                            "NY.GDP.MKTP.KD.ZG;"
                            "NE.EXP.GNFS.ZS;"
                            "NE.IMP.GNFS.ZS;"
                            "FP.CPI.TOTL.ZG;"
                            "DT.DOD.DECT.CD"
                            "?format=json&date=2018:2024&per_page=1000&source=2";
            APIKey=api;
            return APIKey;
        }

        void JSON_Data_Parsing(std::string response) override{
            try {
                json data = json::parse(response);

                if (!data.is_array() || data.size() < 2 || !data[1].is_array())
                {
                    std::cerr << "No economic data found." << std::endl;
                    return;
                }

                gdpCurrentUsd.clear();
                gdpPerCapitaCurrentUsd.clear();
                gdpAnnualGrowthRate.clear();

                hasExportsOfGoodsAndServicesPercentGDP = false;
                hasImportsOfGoodsAndServicesPercentGDP = false;
                hasInflationConsumerPricesAnnualPercent = false;
                hasCentralGovernmentExternalDebtCurrentUsd = false;

                for (const auto& item : data[1])
                {
                    if (!item.contains("indicator") || !item["indicator"].contains("id") ||
                        item["indicator"]["id"].is_null() || item["date"].is_null() ||
                        item["value"].is_null())
                    {
                        continue;
                    }

                    std::string indicatorCode = item["indicator"]["id"].get<std::string>();
                    int year = std::stoi(item["date"].get<std::string>());
                    double value = item["value"].get<double>();

                    if (indicatorCode == "NY.GDP.MKTP.CD" && year >= START_YEAR && year <= END_YEAR)
                    {
                        gdpCurrentUsd[year] = value;
                    }
                    else if (indicatorCode == "NY.GDP.PCAP.CD" && year >= START_YEAR && year <= END_YEAR)
                    {
                        gdpPerCapitaCurrentUsd[year] = value;
                    }
                    else if (indicatorCode == "NY.GDP.MKTP.KD.ZG" && year >= START_YEAR && year <= END_YEAR)
                    {
                        gdpAnnualGrowthRate[year] = value;
                    }
                    else if (year == TARGET_YEAR)
                    {
                        if (indicatorCode == "NE.EXP.GNFS.ZS")
                        {
                            exportsOfGoodsAndServicesPercentGDP = value;
                            hasExportsOfGoodsAndServicesPercentGDP = true;
                        }
                        else if (indicatorCode == "NE.IMP.GNFS.ZS")
                        {
                            importsOfGoodsAndServicesPercentGDP = value;
                            hasImportsOfGoodsAndServicesPercentGDP = true;
                        }
                        else if (indicatorCode == "FP.CPI.TOTL.ZG")
                        {
                            inflationConsumerPricesAnnualPercent = value;
                            hasInflationConsumerPricesAnnualPercent = true;
                        }
                        else if (indicatorCode == "DT.DOD.DECT.CD")
                        {
                            centralGovernmentExternalDebtCurrentUsd = value;
                            hasCentralGovernmentExternalDebtCurrentUsd = true;
                        }
                    }
                }
            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            } catch (std::exception& e) {
                std::cerr << "Economic data parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "Economic Information" << std::endl;
            showYearlyData("GDP (current US$)", gdpCurrentUsd);
            showYearlyData("GDP per capita (current US$)", gdpPerCapitaCurrentUsd);
            showYearlyData("GDP annual growth rate (%)", gdpAnnualGrowthRate);

            showTargetYearData("Exports of goods and services (% of GDP)", exportsOfGoodsAndServicesPercentGDP, hasExportsOfGoodsAndServicesPercentGDP);
            showTargetYearData("Imports of goods and services (% of GDP)", importsOfGoodsAndServicesPercentGDP, hasImportsOfGoodsAndServicesPercentGDP);
            showTargetYearData("Inflation, consumer prices (annual %)", inflationConsumerPricesAnnualPercent, hasInflationConsumerPricesAnnualPercent);
            showTargetYearData("Central government external debt, total (current US$)", centralGovernmentExternalDebtCurrentUsd, hasCentralGovernmentExternalDebtCurrentUsd);
            std::cout << std::defaultfloat;
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
                    title,
                    description,
                    extract,
                    pageUrl,
                    thumbnailUrl,
                    lastModified;

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://en.wikipedia.org/api/rest_v1/page/summary/"+urlEncode(name);
            
            APIKey=api;
            return APIKey;
        }


        void JSON_Data_Parsing(std::string response) override{
            try {
                json data = json::parse(response);

                if (data.contains("type") && getStringSafe(data, "type", "").find("not_found") != std::string::npos)
                {
                    std::cerr << "Wikipedia page not found." << std::endl;
                    return;
                }

                title = getStringSafe(data, "title");
                description = getStringSafe(data, "description");
                extract = getStringSafe(data, "extract");
                lastModified = getStringSafe(data, "timestamp");
                pageUrl = "N/A";
                thumbnailUrl = "N/A";

                if (data.contains("content_urls") && data["content_urls"].contains("desktop"))
                {
                    pageUrl = getStringSafe(data["content_urls"]["desktop"], "page");
                }

                if (data.contains("thumbnail") && data["thumbnail"].is_object())
                {
                    thumbnailUrl = getStringSafe(data["thumbnail"], "source");
                }

            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{
            std::cout << "History Information" << std::endl;
            std::cout << "Title: " << title << std::endl;
            std::cout << "Description: " << description << std::endl;
            std::cout << "Summary: " << extract << std::endl;
            std::cout << "Last modified: " << lastModified << std::endl;
            std::cout << "Wikipedia URL: " << pageUrl << std::endl;
            std::cout << "Thumbnail URL: " << thumbnailUrl << std::endl;
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
        int articleCount = 0;

        friend class InformationSystem;

    public:
        std::string setAPIkey(std::string lan, std::string lon, std::string name) override{
            std::string api="https://newsapi.org/v2/everything?q="+urlEncode(name)+"&language=en&sortBy=publishedAt&pageSize=3&apiKey=21045aad20f3482c810a0e9e212f7ab3";
            
            APIKey=api;
            return APIKey;
        }


        void JSON_Data_Parsing(std::string response) override{
            try {
                json data = json::parse(response);

                articleCount = 0;

                for (int i = 0; i < 3; i++)
                {
                    title[i] = "N/A";
                    source[i] = "N/A";
                    description[i] = "N/A";
                    published[i] = "N/A";
                    url[i] = "N/A";
                }

                if (getStringSafe(data, "status", "") == "error")
                {
                    std::cerr << "News API Error: " << getStringSafe(data, "message") << std::endl;
                    return;
                }

                if (!data.contains("articles") || !data["articles"].is_array())
                {
                    std::cerr << "No news data found." << std::endl;
                    return;
                }

                articleCount = std::min(3, static_cast<int>(data["articles"].size()));

                for(int i=0; i<articleCount; i++){
                    const json& article = data["articles"][i];

                    title[i]= getStringSafe(article, "title");
                    description[i]= getStringSafe(article, "description");
                    published[i]= getStringSafe(article, "publishedAt");
                    url[i]= getStringSafe(article, "url");

                    if (article.contains("source") && article["source"].is_object())
                    {
                        source[i]=getStringSafe(article["source"], "name");
                    }
                }

            } catch (json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        void showData() override{
            std::cout << "News Information" << std::endl;

            if (articleCount == 0)
            {
                std::cout << "No news articles found." << std::endl;
                return;
            }

            for(int i=0; i<articleCount; i++){
                    std::cout<<"Article "<<i + 1<<std::endl;
                    std::cout<<"Title: "<<title[i]<<std::endl;
                    std::cout<<"Source: "<<source[i]<<std::endl;
                    std::cout<<"Description: "<<description[i]<<std::endl;
                    std::cout<<"Published at: "<<published[i]<<std::endl;
                    std::cout<<"URL: "<<url[i]<<std::endl;
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
        Data *economic = new EconomicInformation();

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
            social-> JSON_Data_Parsing(response);
            social->showData();
        }

        void showEconomicInformation(){
            std::string API= economic->setAPIkey(std::to_string(latitude), std::to_string(longitude),countryName);
            std::string response=economic->HTTP_Request(API);
            economic-> JSON_Data_Parsing(response);
            economic->showData();
        }

        void showNewsInformation(){
            std::string API= news->setAPIkey(std::to_string(latitude), std::to_string(longitude),countryName);
            std::string response=news->HTTP_Request(API);
            news-> JSON_Data_Parsing(response);
            news->showData();
        }

        void showHistoryInformation(){
            std::string API= history->setAPIkey(std::to_string(latitude), std::to_string(longitude),countryName);
            std::string response=history->HTTP_Request(API);
            history-> JSON_Data_Parsing(response);
            history->showData();
        }

        void showBasicInformation(){
            std::string API= basic->setAPIkey(std::to_string(latitude), std::to_string(longitude),countryName);
            std::string response=basic->HTTP_Request(API);
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
    std::cin >> std::ws;
    std::getline(std::cin, name);

    information.setLatitude(lat);
    information.setLongitude(longi);
    information.setCountryName(name);

    information.showNewsInformation();
    information.showHistoryInformation();

    return 0;
}
