#ifndef ERROR_INJECTION_H
#define ERROR_INJECTION_H

#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <bitset>

// Error injection method types
enum class ErrorInjectionMethod {
    BIT_FLIP,
    CHAR_SUBSTITUTION,
    CHAR_DELETION,
    CHAR_INSERTION,
    CHAR_SWAPPING,
    MULTIPLE_BIT_FLIPS,
    BURST_ERROR
};

class ErrorInjection {
private:
    static std::mt19937& getRandomGenerator() {
        static std::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
        return gen;
    }

public:
    // Convert string to binary and back
    static std::string binaryToString(const std::string& binary) {
        std::string result;
        for (size_t i = 0; i < binary.length(); i += 8) {
            std::string byte = binary.substr(i, 8);
            if (byte.length() == 8) {
                char c = static_cast<char>(std::bitset<8>(byte).to_ulong());
                result += c;
            }
        }
        return result;
    }

    static std::string stringToBinary(const std::string& str) {
        std::string binary;
        for (char c : str) {
            binary += std::bitset<8>(c).to_string();
        }
        return binary;
    }

    // 1. Bit Flip
    static std::string injectBitFlip(const std::string& data) {
        if (data.empty()) return data;
        
        std::string binary = stringToBinary(data);
        std::mt19937& gen = getRandomGenerator();
        std::uniform_int_distribution<> dis(0, binary.length() - 1);
        
        int pos = dis(gen);
        binary[pos] = (binary[pos] == '0') ? '1' : '0';
        
        return binaryToString(binary);
    }

    // 2. Character Substitution
    static std::string injectCharSubstitution(const std::string& data) {
        if (data.empty()) return data;
        
        std::mt19937& gen = getRandomGenerator();
        std::uniform_int_distribution<> posDis(0, data.length() - 1);
        std::uniform_int_distribution<> charDis(32, 126); // Printable ASCII
        
        std::string corrupted = data;
        int pos = posDis(gen);
        corrupted[pos] = static_cast<char>(charDis(gen));
        
        return corrupted;
    }

    // 3. Character Deletion
    static std::string injectCharDeletion(const std::string& data) {
        if (data.empty()) return data;
        
        std::mt19937& gen = getRandomGenerator();
        std::uniform_int_distribution<> dis(0, data.length() - 1);
        
        std::string corrupted = data;
        corrupted.erase(dis(gen), 1);
        
        return corrupted;
    }

    // 4. Random Character Insertion
    static std::string injectCharInsertion(const std::string& data) {
        if (data.empty()) {
            std::mt19937& gen = getRandomGenerator();
            std::uniform_int_distribution<> charDis(32, 126);
            return std::string(1, static_cast<char>(charDis(gen)));
        }
        
        std::mt19937& gen = getRandomGenerator();
        std::uniform_int_distribution<> posDis(0, data.length());
        std::uniform_int_distribution<> charDis(32, 126);
        
        std::string corrupted = data;
        int pos = posDis(gen);
        corrupted.insert(pos, 1, static_cast<char>(charDis(gen)));
        
        return corrupted;
    }

    // 5. Character Swapping
    static std::string injectCharSwapping(const std::string& data) {
        if (data.length() < 2) return data;
        
        std::mt19937& gen = getRandomGenerator();
        std::uniform_int_distribution<> dis(0, data.length() - 2);
        
        std::string corrupted = data;
        int pos = dis(gen);
        std::swap(corrupted[pos], corrupted[pos + 1]);
        
        return corrupted;
    }

    // 6. Multiple Bit Flips
    static std::string injectMultipleBitFlips(const std::string& data) {
        if (data.empty()) return data;
        
        std::string binary = stringToBinary(data);
        std::mt19937& gen = getRandomGenerator();
        std::uniform_int_distribution<> countDis(2, 5); // 2-5 bit flips
        std::uniform_int_distribution<> posDis(0, binary.length() - 1);
        
        int flips = countDis(gen);
        for (int i = 0; i < flips; i++) {
            int pos = posDis(gen);
            binary[pos] = (binary[pos] == '0') ? '1' : '0';
        }
        
        return binaryToString(binary);
    }

    // 7. Burst Error (3-8 consecutive characters corrupted)
    static std::string injectBurstError(const std::string& data) {
        if (data.empty()) return data;
        
        std::mt19937& gen = getRandomGenerator();
        std::uniform_int_distribution<> burstDis(3, std::min(8, static_cast<int>(data.length())));
        std::uniform_int_distribution<> posDis(0, std::max(0, static_cast<int>(data.length()) - 3));
        std::uniform_int_distribution<> charDis(32, 126);
        
        int burstSize = burstDis(gen);
        int startPos = posDis(gen);
        
        std::string corrupted = data;
        for (int i = 0; i < burstSize && (startPos + i) < static_cast<int>(corrupted.length()); i++) {
            corrupted[startPos + i] = static_cast<char>(charDis(gen));
        }
        
        return corrupted;
    }

    // Inject error using random method
    static std::string injectError(const std::string& data) {
        if (data.empty()) return data;
        
        std::mt19937& gen = getRandomGenerator();
        std::uniform_int_distribution<> methodDis(0, 6);
        
        ErrorInjectionMethod method = static_cast<ErrorInjectionMethod>(methodDis(gen));
        
        switch (method) {
            case ErrorInjectionMethod::BIT_FLIP:
                return injectBitFlip(data);
            case ErrorInjectionMethod::CHAR_SUBSTITUTION:
                return injectCharSubstitution(data);
            case ErrorInjectionMethod::CHAR_DELETION:
                return injectCharDeletion(data);
            case ErrorInjectionMethod::CHAR_INSERTION:
                return injectCharInsertion(data);
            case ErrorInjectionMethod::CHAR_SWAPPING:
                return injectCharSwapping(data);
            case ErrorInjectionMethod::MULTIPLE_BIT_FLIPS:
                return injectMultipleBitFlips(data);
            case ErrorInjectionMethod::BURST_ERROR:
                return injectBurstError(data);
            default:
                return injectCharSubstitution(data);
        }
    }
};

#endif // ERROR_INJECTION_H

