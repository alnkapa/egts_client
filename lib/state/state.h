#pragma once
#ifndef STATE_H
#define STATE_H
#include <memory>
#include <iostream>

namespace state_machine
{
    class State
    {
    public:
        virtual void connect() = 0;
        virtual void auth() = 0;
        virtual void send() = 0;
        virtual void receive() = 0;
        virtual void disconnect() = 0;        
    };    

    class Context
    {
    private:
        std::shared_ptr<State> state;

    public:
        void setState(std::shared_ptr<State> newState)
        {
            state = newState;
        }

        void request()
        {
            state->handle();
        }
    };
}
#endif
/*
как сделать 1 читатель и 1 писатель из tcp сокета для boost::asio

напиши мне машину состояния для алгоритма:
1) ожидание новых пакетов на клиента для отправки на сервер, если такие есть то отправка.
2) ожидание подтверждения получения пакетов от сервера
    1) если подтверждение не получено повторить 3 раза пункт 1 через 5 секунд.
3) ожидание новых пакетов от сервера, если такие есть то получить.
4) отправить подтверждение получения.

    


*/