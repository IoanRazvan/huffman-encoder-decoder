#ifndef STACKALLOCATOR_HPP
#define STACKALLOCATOR_HPP

#include <stdexcept>

template <class T>
class StackAllocator
{
private:
    size_t stackSize;
    size_t top;
    char * buffer;
public:
    StackAllocator(const StackAllocator &) = delete;
    StackAllocator & operator=(const StackAllocator &) = delete;
    StackAllocator & operator=(StackAllocator && st);
    StackAllocator() : stackSize(0), top(0), buffer(nullptr) {}
    StackAllocator(size_t stackSie);
    T * nextAddress();
    void freeLast();
    ~StackAllocator();
};

template<class T>
StackAllocator<T>::StackAllocator(size_t stackSize) : stackSize(stackSize), top(0), buffer(new char[stackSize * sizeof(T)])
{
}

template<class T>
StackAllocator<T> & StackAllocator<T>::operator=(StackAllocator && st)
{
    if (this == &st)
        return *this;
    
    delete[] this->buffer;
    top = st.top;
    stackSize = st.stackSize;
    buffer = st.buffer;
    
    st.top = 0;
    st.buffer = nullptr;
    st.stackSize = 0;

    return *this;
}

template<class T>
T * StackAllocator<T>::nextAddress()
{
    if (top == stackSize)
        throw std::runtime_error("StackAllocator is filled");
    return reinterpret_cast<T *>(buffer) + top++;
}

template<class T>
void StackAllocator<T>::freeLast()
{
    if (top == 0)
        throw std::runtime_error("StackAllocator is empty");
    --top;
}

template<class T>
StackAllocator<T>::~StackAllocator()
{
    for (int i = 0; i < stackSize; i++)
        reinterpret_cast<T *>(buffer + i * sizeof(T))->~T();
    delete[] buffer;
}

#endif