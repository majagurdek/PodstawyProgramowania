#include <iostream>
#include "VectoredList.h"
#include <vector>

using namespace std;

// kod z zajęć 11 od Tymoteusza
VectoredList::VectoredList()
{
    head = nullptr;
    tail = nullptr;
    size = 0;
    capasity = 0;
}

VectoredList::~VectoredList()
{
    while (head)
    {
        Bucket *tmp = head;
        head = tmp->next;
        delete tmp;
    }

}

VectoredList::Bucket::Bucket(): next(nullptr), pre(nullptr)
{
    lastIndex = 0;
    for (int i = 0; i < NodeSize; i++)
    {
        tab[i]=-1;
    }
}

VectoredList::Bucket::~Bucket()
{

}

void VectoredList::push_back(string value)
{
    if(!tail)
    {
        tail = new Bucket();
        size+=1;
        capasity+=NodeSize;
        head = tail;
        tail->add(value);
    }
    else
    {
        if(tail->add(value))
            return;

        Bucket *tmp = tail;
        tail = new Bucket();

        tail->pre = tmp;
        tmp->next = tail;
        tail->add(value);

        size+=1;
        capasity+=NodeSize;
    }

}

VectoredList::Iterator VectoredList::begin() const
{
    Iterator it;
    if( head )
    {
        it.Bcursor = head;
        it.Ecursor = &head->tab[0];
    }
    return it;
}

VectoredList::Iterator VectoredList::end() const
{
    Iterator it;
    if( tail )
    {
        it.Bcursor = tail;
        it.Ecursor = &tail->tab[tail->lastIndex];
    }
    return it;
}

void VectoredList::erase(const Iterator &pos)
{
    Bucket *tmp = head;

    while(tmp != pos.Bcursor)
    {
        tmp = tmp->next;
    }

    int index = pos.Ecursor - tmp->tab;

    while (tmp->next)
    {
        for(int i=index; i<tmp->lastIndex && i<NodeSize-1; i++)
        {
            tmp->tab[i]=tmp->tab[i+1];
        }
        tmp->lastIndex--;
        if (tmp->next)
        {
            tmp->tab[tmp->lastIndex] = tmp->next->tab[0];
            tmp->lastIndex++;
            tmp = tmp->next;
            index = 0;
        }
    };

    for(int i=index; i<tmp->lastIndex && i<NodeSize-1; i++)
    {
        tmp->tab[i]=tmp->tab[i+1];
    }
    tmp->lastIndex--;

    if (tail->lastIndex == 0)
    {
        pop_back();
    }

}

void VectoredList::erase(const Iterator &start, const Iterator &end)
{

    int count = 0;
    for (Iterator it = start; it != end; ++it,count++);
    for (int i = 0; i < count-1; i++)
    {
        this->erase(start);
    }
}

VectoredList& VectoredList::operator = (const VectoredList &other)
{
    if(this == &other)
    {
        return *this;
    }

    this->erase(this->begin(),this->end());

    for(auto &e : other )
    {
        this->push_back(e);
    }

    return *this;
}

int VectoredList::getSize() const
{
    Bucket *tmp = head;
    int count = 0;
    while (tmp)
    {
        count += tmp->lastIndex;
        tmp = tmp->next;
    }
    return count;
}

string & VectoredList::operator [] (const size_t index)
{
    if(index > capasity)
        throw out_of_range("index is out of range");

    int bucketNum = index/NodeSize;
    int itemNum = index%NodeSize;
    Bucket *tmp = head;

    for(int i=0; i<bucketNum; i++)
    {
        tmp = tmp->next;
    }

    if(tmp->lastIndex<itemNum)
        throw out_of_range("index is out of range");

    return (tmp->tab[itemNum]);
}

void VectoredList::show()
{
    Bucket* tmp = head;
    cout<<"{"<<endl;
    while (tmp)
    {
        tmp->show();
        tmp = tmp->next;
    }
    cout<<"}"<<endl;
    cout<<"size "<<size<<endl;
    cout<<"capasity "<<capasity<<endl;
}

void VectoredList::Bucket::show()
{
    cout<<"[ ";
    for (int i = 0; i < lastIndex; i++)
    {
        cout<<tab[i]<<", ";
    }
    cout<<"]"<<endl;

}

void VectoredList::pop_back()
{
    Bucket* tmp = tail;
    if (tail->pre)
    {
        tail = tail->pre;
        tail ->next = nullptr;
    }
    // else
    // {
    //     tail = head;
    // }

    if(tmp == head)
    {
        head = nullptr;
        tail = nullptr;
    }
    delete tmp;
    size--;
    capasity-=NodeSize;
}

VectoredList::Iterator::Iterator()
{
    Bcursor = nullptr;
    Ecursor = nullptr;
}

VectoredList::Iterator::~Iterator()
{

}

bool VectoredList::Iterator::operator != (const Iterator& other)
{
    if (this == &other)
    {
        return false;
    }

    if (this->Ecursor == other.Ecursor)
    {
        return false;
    }

    return true;
}

VectoredList::Iterator& VectoredList::Iterator::operator+= (const int count)
{
    for (int i = 0; i < count; i++)
    {
        ++(*this);
    }

    return *this;
}

string& VectoredList::Iterator::operator* ()
{
    return *Ecursor;
}

VectoredList::Iterator& VectoredList::Iterator::operator++ ()
{
    if(this->Ecursor == &this->Bcursor->tab[NodeSize-1])
    {
        this->Bcursor = this->Bcursor->next;
        if(this->Bcursor)
        {
            this->Ecursor = &this->Bcursor->tab[0];
        }
        // else
        // {
        //     this->Ecursor = nullptr;
        // }
    }
    else
    {
        this->Ecursor++;
    }

    return *this;
}

bool VectoredList::Bucket::add(string &value)
{
    if(lastIndex < NodeSize)
    {
        tab[lastIndex] = value;
        lastIndex++;
        return true;
    }
    else
    {
        return false;
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VectoredList::VectoredList(VectoredList &&other) noexcept
{
    head = other.head;
    tail = other.tail;
    size = other.size;
    capasity = other.capasity;

    other.head = nullptr;
    other.tail = nullptr;
    other.size = 0;
    other.capasity = 0;
}

VectoredList& VectoredList::operator = (VectoredList &&other)
{
    if (this != &other)
    {
        head = other.head;
        tail = other.tail;
        size = other.size;
        capasity = other.capasity;

        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
        other.capasity = 0;
    }
    return *this;
}


VectoredList VectoredList::operator + (const VectoredList &other) const
{
    VectoredList temp;
    for (auto &e: *this)
    {
        temp.push_back(e);
    }
    for (auto &e: other)
    {
        temp.push_back(e);
    }
    return temp;
}

VectoredList& VectoredList::assign(std::vector<string> &v)
{
    tail=head= nullptr;
    capasity=size = 0;
    for (auto &e : v)
    {
        this->push_back(e);
    }
    v.clear();
    return *this;
}
