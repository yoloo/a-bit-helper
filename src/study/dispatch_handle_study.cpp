#include <iostream>

namespace Exa1 {
    class Msg1;
    class Msg2;

    class Handler {
    public:
        void handle(Msg1* msg) {
            std::cout << "msg-1 handle" << std::endl;
        }

        void handle(Msg2* msg) {
            std::cout << "msg-2 handle" << std::endl;
        }
    };

    class Message {
    public:
        virtual ~Message() = default;

    public:
        void dispatch(Handler* handler) {
            dispatchIml(handler);
        }

    protected:
        virtual void dispatchIml(Handler* handler) = 0;
    };

    class Msg1 : public Message {
    public:
        void dispatchIml(Handler* handler) override {
            // same
            handler->handle(this);
        }
    };

    class Msg2 : public Message {
    public:
        void dispatchIml(Handler* handler) override {
            // same
            handler->handle(this);
        }
    };

    void run() {
        auto handler = new Handler{};
        Message* message = new Msg1{};
        message->dispatch(handler);

        delete message;
        delete handler;
    }
}

namespace Exa2 {
    class Msg1;
    class Msg2;

    class Handler {
    public:
        void handle(Msg1* msg) {
            std::cout << "msg-1 handle" << std::endl;
        }

        void handle(Msg2* msg) {
            std::cout << "msg-2 handle" << std::endl;
        }
    };

    class Message {
    public:
        virtual ~Message() = default;

    public:
        void dispatch(Handler* handler) {
            dispatch_impl(handler);
        }

    protected:
        virtual void dispatch_impl(Handler* handler) = 0;
    };

    template<class DerivedMsgT>
    class MsgBase : public Message{
    protected:
        void dispatch_impl(Handler* handler) override {
            handler->handle(static_cast<DerivedMsgT*>(this));
        }
    };

    struct Msg1 : public MsgBase<Msg1> {
        // ...
    };

    struct Msg2 : public MsgBase<Msg2> {
        // ...
    };

    void run() {
        auto handler = new Handler{};
        Message* message = new Msg1{};
        message->dispatch(handler);

        delete message;
        delete handler;
    }
}

namespace Exa3 {
    class Handler;

    // msg interface
    class Message {
    public:
        virtual ~Message() = default;

        virtual void do_it() = 0;

    public:
        void dispatch(Handler* handler) {
           dispatchIml(handler);
        }

    protected:
        virtual void dispatchIml(Handler* handler) = 0;
    };

    // handler
    class Handler {
    public:
        void handle(Message* msg) {
            if (msg != nullptr) {
                msg->do_it();
            }
        }
    };

    // msg-base
    template <typename MessageDerivedT>
    class MessageBase : public Message {
    protected:
        void dispatchIml(Handler* handler) override {
            handler->handle(static_cast<MessageDerivedT*>(this));
        }
    };

    // msg-1
    class Msg1 : public MessageBase<Msg1> {
    public:
        void do_it() override {
            std::cout << "msg-1 do it" << std::endl;
        }
    };

    // msg-2
    class Msg2 : public MessageBase<Msg2> {
    public:
        void do_it() override {
            std::cout << "msg-2 do it" << std::endl;
        }
    };

    void run() {
        auto handler = new Handler{};
        Message* message = new Msg2{};
        message->dispatch(handler);

        delete handler;
        delete message;
    }
}

int main() {
    //Exa1::run();
    Exa2::run();
    //Exa3::run();

    return 0;
}