// task_4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <iterator>

// Abstract base class for transformations (dynamic polymorphism)
template <typename T>
class Transformer {
public:
    virtual T operator()(const T& c) const = 0;
    virtual ~Transformer() {}
};

// Template class String
template <typename T>
class String {
private:
    T* data;
    size_t length;

    void allocate(size_t n) {
        data = new T[n];
        length = n;
    }

    void deallocate() {
        delete[] data;
        data = nullptr;
        length = 0;
    }

public:
    String() : data(nullptr), length(0) {}

    String(const String& other) {
        allocate(other.length);
        std::copy(other.data, other.data + other.length, data);
    }

    String(String&& other) noexcept : data(other.data), length(other.length) {
        other.data = nullptr;
        other.length = 0;
    }

    String(size_t n, const T& c) {
        allocate(n);
        std::fill(data, data + n, c);
    }

    String(const T* str) {
        size_t len = 0;
        while (str[len] != T()) len++;
        allocate(len);
        std::copy(str, str + len, data);
    }

    template <typename Iter>
    String(Iter begin, Iter end) {
        if (begin > end) throw std::invalid_argument("Invalid iterator range");
        size_t len = std::distance(begin, end);
        allocate(len);
        std::copy(begin, end, data);
    }

    template <typename U>
    String(const String<U>& other) {
        allocate(other.size());
        for (size_t i = 0; i < other.size(); ++i) {
            data[i] = static_cast<T>(other[i]);
        }
    }

    // Destructor
    ~String() {
        deallocate();
    }

    // Copy assignment operator
    String& operator=(const String& other) {
        if (this != &other) {
            deallocate();
            allocate(other.length);
            std::copy(other.data, other.data + other.length, data);
        }
        return *this;
    }

    // Move assignment operator
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            deallocate();
            data = other.data;
            length = other.length;
            other.data = nullptr;
            other.length = 0;
        }
        return *this;
    }

    // Get length
    size_t size() const { return length; }

    // Check if empty
    bool empty() const { return length == 0; }

    // Clear string
    void clear() { deallocate(); }

    // Access character by index
    T& operator[](size_t index) {
        if (index >= length) throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= length) throw std::out_of_range("Index out of range");
        return data[index];
    }

    // Get substring
    String substr(size_t start, size_t len) const {
        if (start >= length || start + len > length) throw std::out_of_range("Substring out of range");
        return String(data + start, data + start + len);
    }

    // Concatenation
    String operator+(const String& other) const {
        String result;
        result.allocate(length + other.length);
        std::copy(data, data + length, result.data);
        std::copy(other.data, other.data + other.length, result.data + length);
        return result;
    }

    String operator+(const T& c) const {
        String result;
        result.allocate(length + 1);
        std::copy(data, data + length, result.data);
        result.data[length] = c;
        return result;
    }

    // Repetition
    String operator*(size_t n) const {
        if (n == 0) return String();
        String result;
        result.allocate(length * n);
        for (size_t i = 0; i < n; ++i) {
            std::copy(data, data + length, result.data + i * length);
        }
        return result;
    }

    // Comparison
    bool operator==(const String& other) const {
        if (length != other.length) return false;
        for (size_t i = 0; i < length; ++i) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    bool operator!=(const String& other) const { return !(*this == other); }

    bool operator<(const String& other) const {
        size_t min_len = std::min(length, other.length);
        for (size_t i = 0; i < min_len; ++i) {
            if (data[i] < other.data[i]) return true;
            if (data[i] > other.data[i]) return false;
        }
        return length < other.length;
    }

    bool operator>(const String& other) const { return other < *this; }
    bool operator<=(const String& other) const { return !(*this > other); }
    bool operator>=(const String& other) const { return !(*this < other); }

    // Transformations (dynamic polymorphism)
    void apply(const Transformer<T>& t) {
        for (size_t i = 0; i < length; ++i) {
            data[i] = t(data[i]);
        }
    }

    String transform(const Transformer<T>& t) const {
        String result(*this);
        result.apply(t);
        return result;
    }

    // Transformations (static polymorphism)
    template <typename Trans>
    void modify(const Trans& t) {
        for (size_t i = 0; i < length; ++i) {
            data[i] = t(data[i]);
        }
    }

    template <typename Trans>
    String modify_new(const Trans& t) const {
        String result(*this);
        result.modify(t);
        return result;
    }
};

// Specialization for char
template <>
class String<char> {
private:
    char* data;
    size_t length;

    void allocate(size_t n) {
        data = new char[n];
        length = n;
    }

    void deallocate() {
        delete[] data;
        data = nullptr;
        length = 0;
    }

public:
    String() : data(nullptr), length(0) {}
    String(const String& other) {
        allocate(other.length);
        std::copy(other.data, other.data + other.length, data);
    }
    String(String&& other) noexcept : data(other.data), length(other.length) {
        other.data = nullptr;
        other.length = 0;
    }
    String(size_t n, char c) {
        allocate(n);
        std::fill(data, data + n, c);
    }
    String(const char* str) {
        size_t len = std::strlen(str);
        allocate(len);
        std::copy(str, str + len, data);
    }
    template <typename Iter>
    String(Iter begin, Iter end) {
        if (begin > end) throw std::invalid_argument("Invalid iterator range");
        size_t len = std::distance(begin, end);
        allocate(len);
        std::copy(begin, end, data);
    }
    template <typename U>
    String(const String<U>& other) {
        allocate(other.size());
        for (size_t i = 0; i < other.size(); ++i) {
            data[i] = static_cast<char>(other[i]);
        }
    }
    ~String() { deallocate(); }

    String& operator=(const String& other) {
        if (this != &other) {
            deallocate();
            allocate(other.length);
            std::copy(other.data, other.data + other.length, data);
        }
        return *this;
    }
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            deallocate();
            data = other.data;
            length = other.length;
            other.data = nullptr;
            other.length = 0;
        }
        return *this;
    }

    size_t size() const { return length; }
    bool empty() const { return length == 0; }
    void clear() { deallocate(); }
    char& operator[](size_t index) {
        if (index >= length) throw std::out_of_range("Index out of range");
        return data[index];
    }
    const char& operator[](size_t index) const {
        if (index >= length) throw std::out_of_range("Index out of range");
        return data[index];
    }
    String substr(size_t start, size_t len) const {
        if (start >= length || start + len > length) throw std::out_of_range("Substring out of range");
        return String(data + start, data + start + len);
    }
    String operator+(const String& other) const {
        String result;
        result.allocate(length + other.length);
        std::copy(data, data + length, result.data);
        std::copy(other.data, other.data + other.length, result.data + length);
        return result;
    }
    String operator+(char c) const {
        String result;
        result.allocate(length + 1);
        std::copy(data, data + length, result.data);
        result.data[length] = c;
        return result;
    }
    String operator*(size_t n) const {
        if (n == 0) return String();
        String result;
        result.allocate(length * n);
        for (size_t i = 0; i < n; ++i) {
            std::copy(data, data + length, result.data + i * length);
        }
        return result;
    }
    bool operator==(const String& other) const {
        if (length != other.length) return false;
        for (size_t i = 0; i < length; ++i) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }
    bool operator!=(const String& other) const { return !(*this == other); }
    bool operator<(const String& other) const {
        size_t min_len = std::min(length, other.length);
        for (size_t i = 0; i < min_len; ++i) {
            if (data[i] < other.data[i]) return true;
            if (data[i] > other.data[i]) return false;
        }
        return length < other.length;
    }
    bool operator>(const String& other) const { return other < *this; }
    bool operator<=(const String& other) const { return !(*this > other); }
    bool operator>=(const String& other) const { return !(*this < other); }

    void apply(const Transformer<char>& t) {
        for (size_t i = 0; i < length; ++i) {
            data[i] = t(data[i]);
        }
    }
    String transform(const Transformer<char>& t) const {
        String result(*this);
        result.apply(t);
        return result;
    }
    template <typename Trans>
    void modify(const Trans& t) {
        for (size_t i = 0; i < length; ++i) {
            data[i] = t(data[i]);
        }
    }
    template <typename Trans>
    String modify_new(const Trans& t) const {
        String result(*this);
        result.modify(t);
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const String& str) {
        for (size_t i = 0; i < str.length; ++i) {
            os << str.data[i];
        }
        return os;
    }
    friend std::istream& operator>>(std::istream& is, String& str) {
        str.clear();
        char c;
        while (is.get(c) && c != '\n') {
            str = str + c;
        }
        return is;
    }
};

// Function to create a string
template <typename Iter>
String<typename std::iterator_traits<Iter>::value_type> make_string(Iter begin, Iter end) {
    return String<typename std::iterator_traits<Iter>::value_type>(begin, end);
}

template <typename T>
String<T> make_string(const T* str) {
    return String<T>(str);
}

// Transformers
template <typename T>
class UpperCase : public Transformer<T> {
public:
    T operator()(const T& c) const override {
        return c; // Default: no transformation
    }
};

// Specialization for char
template <>
class UpperCase<char> : public Transformer<char> {
public:
    char operator()(const char& c) const override {
        return std::toupper(c);
    }
};

struct ToLower {
    char operator()(char c) const { return std::tolower(c); }
};

// Class without comparison operators
struct NoCompare {
    int value;
    NoCompare() : value(0) {}
    NoCompare(int v) : value(v) {}
};

// Interactive interface
int main() {
    while (true) {
        try {
            std::cout << "Enter a string: ";
            String<char> s1;
            std::cin >> s1;
            std::cout << "Your string: " << s1 << "\n";

            std::cout << "String length: " << s1.size() << "\n";
            std::cout << "Is string empty? " << (s1.empty() ? "Yes" : "No") << "\n";

            std::cout << "Enter index for access: ";
            size_t index;
            std::cin >> index;
            std::cout << "Character at index " << index << ": " << s1[index] << "\n";

            std::cout << "Enter new character for replacement: ";
            char new_char;
            std::cin >> new_char;
            s1[index] = new_char;
            std::cout << "Updated string: " << s1 << "\n";

            std::cout << "Enter start index and length for substring: ";
            size_t start, len;
            std::cin >> start >> len;
            String<char> s2 = s1.substr(start, len);
            std::cout << "Substring: " << s2 << "\n";

            std::cout << "Enter second string for concatenation: ";
            String<char> s3;
            std::cin.ignore();
            std::cin >> s3;
            String<char> s4 = s1 + s3;
            std::cout << "Concatenation: " << s4 << "\n";

            std::cout << "Enter number of repetitions: ";
            size_t repeats;
            std::cin >> repeats;
            String<char> s5 = s1 * repeats;
            std::cout << "Repetition: " << s5 << "\n";

            std::cout << "Comparing s1 and s3: " << (s1 == s3 ? "Equal" : "Not equal") << "\n";
            std::cout << "s1 < s3: " << (s1 < s3 ? "Yes" : "No") << "\n";

            UpperCase<char> uc;
            s1.apply(uc);
            std::cout << "After uppercase: " << s1 << "\n";

            ToLower tl;
            String<char> s6 = s1.modify_new(tl);
            std::cout << "New lowercase string: " << s6 << "\n";

            // Test with NoCompare
            String<NoCompare> s7(3, NoCompare(42));
            std::cout << "Created NoCompare string\n";

        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }

        // Prompt to repeat
        char choice;
        do {
            std::cout << "Do you want to enter another string? (y/n): ";
            std::cin >> choice;
            choice = std::tolower(choice);
            std::cin.ignore();
        } while (choice != 'y' && choice != 'n');

        if (choice == 'n') {
            break;
        }

        // Clear console (cross-platform)
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
    return 0;
}