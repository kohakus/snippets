/* Some Singleton Implementation Examples. */
/* Mainly consider of lazy initialization. */


// Version 1: simple lazy singleton.
// Not thread safe. Can only be used in single thread.
template<typename T>
class Singleton {
public:
    static T& getInstance() {
        if (!value_) {
            value_ = new T();
        }
        return *value_;
    }

private:
    Singleton();
    Singleton(const Singleton<T>&);
    Singleton<T>& operator=(const Singleton<T>&);
    ~Singleton();

private:
    static T* value_;
};

template<typename T>
T* Singleton<T>::value_ = nullptr;


// Version 2: use locking (often mutex).
// Thread safe, but not efficient.
template<typename T>
class Singleton {
public:
    static T& getInstance() {
        {
            // RAII
            std::lock_guard<std::mutex> guard(lock_);
            if (!value_) {
                value_ = new T();
            }
        }
        return *value_;
    }

private:
    Singleton();
    Singleton(const Singleton<T>&);
    Singleton<T>& operator=(const Singleton<T>&);
    ~Singleton();

private:
    static T* value_;
    static std::mutex lock_;
};

template<typename T>
T* Singleton<T>::value_ = nullptr;

template<typename T>
std::mutex Singleton<T>::lock_;


// Version 3: use local static variable.
// Not thread safe before C++11.
// Provide by Meyers, which is called Meyers Singleton.
template<typename T>
class Singleton {
public:
    static T& getInstance() {
        static T value;
        return value;
    }

private:
    Singleton();
    Singleton(const Singleton<T>&);
    Singleton<T>& operator=(const Singleton<T>&);
    ~Singleton();
};


// Version 4: use POSIX pthread_once.
// Thread safe.
template<typename T>
class Singleton {
public:
    static T& getInstace() {
        pthread_once(&ponce_, &Singleton<T>::init);
        return *value_;
    }

private:
    Singleton();
    Singleton(const Singleton<T>&);
    Singleton<T>& operator=(const Singleton<T>&);
    ~Singleton();

    static void init() {
        value_ = new T();
    }

private:
    static pthread_once_t ponce_;
    static T* value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = nullptr;


// Note:
// DCL (double-checked-locking) is used in history. However, DCL is proved to be unsafe.
// Eager initialization avoid the thread safe problem, but it is not recommended.
// A method to make DCL safe again is to utilize memory barrier (but it is not a good solution).
// We can also use atomic to implement a thread safe singleton. However, it is more complex to implement.