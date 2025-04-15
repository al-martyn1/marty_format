    bool operator==(InputIterator other) const
    {
        return isIteratorEnd()==other.isIteratorEnd();
    }

    bool operator!=(InputIterator other) const
    {
        return isIteratorEnd()!=other.isIteratorEnd();
    }

    InputIterator() : m_ptr(0), m_ptrEnd(0) {}

    InputIterator(const char* b, const char* e) : m_ptr(const_pointer_type(b)), m_ptrEnd(const_pointer_type(e)) {}

    bool isIteratorEnd() const
    {
        return m_ptr==m_ptrEnd;
    }

    const char* m_ptr     = 0;
    const char* m_ptrEnd  = 0;
