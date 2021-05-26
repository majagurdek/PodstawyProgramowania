#ifndef untitled6_VECTORED_LIST_H
#define untitled6_VECTORED_LIST_H

#include <string>
#include <vector>

using namespace std;

constexpr size_t NodeSize = 10;

// kod z zajęć 11 od Tymoteusza
class VectoredList
{
private:

    struct Bucket
    {
        int lastIndex;
        Bucket *next;
        Bucket *pre;
        std::string tab[NodeSize];

        Bucket();
        ~Bucket();

        bool add(std::string &value);
        //Debug
        void show();
    };

    Bucket *head;
    Bucket *tail;

    size_t size;
    size_t capasity;

    void pop_back();

public:

    class Iterator
    {
        friend class VectoredList;
    private:
        Bucket *Bcursor;
        std::string *Ecursor;

    public:
        Iterator();
        ~Iterator();

        Iterator& operator++ ();
        std::string& operator* ();
        bool operator != (const Iterator& other);
        Iterator& operator+= (const int count);
    };

    VectoredList();
    ~VectoredList();

    void push_back(std::string value);
    std::string & operator [] (const size_t index);

    Iterator begin() const;
    Iterator end() const;

    void erase(const Iterator &pos);
    void erase(const Iterator &start, const Iterator &end);

    VectoredList& operator = (const VectoredList &other);

    int getSize() const;
    //Debug
    void show();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    VectoredList(VectoredList &&other) noexcept;
    VectoredList& operator=(VectoredList&& other);
    VectoredList operator+(const VectoredList& other) const;
    VectoredList& assign(std::vector<string> &v);

};

#endif // __VECTOREDLIST_H__




