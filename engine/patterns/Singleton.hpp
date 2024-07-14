#pragma once

template<class C>
class Singleton
{
public:

    template<typename... Args>
    static void Init(Args&&... args)
    {

    }

    static void Destroy()
    {
        if (s_Instance)
        {
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    static C& GetInstance() noexcept
    {
        if (!s_Instance)
            throw(std::logic_error)
    }

private:

    static inline C* s_Instance { nullptr };

    Singleton() = default;
    ~Singleton() noexcept { Destroy(); }
};