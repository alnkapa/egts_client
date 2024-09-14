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
