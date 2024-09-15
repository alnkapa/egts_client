// queue

template <typename T, typename Index>
class Queue
{
  public:
    T
    wait_for_send();
    void mark_as_sent(Index);
    void mark_as_confirmed(Index);
    void wait_for_push(T);
};

template <typename T, typename Index>
inline T
Queue<T, Index>::wait_for_send()
{
    return T();
}

template <typename T, typename Index>
inline void
Queue<T, Index>::mark_as_sent(Index)
{
}

template <typename T, typename Index>
inline void
Queue<T, Index>::mark_as_confirmed(Index)
{
}

template <typename T, typename Index>
inline void
Queue<T, Index>::wait_for_push(T)
{
}
