#pragma once

namespace pulvis::tl
{
    template<class C>
    class Singleton
    {
      public:

        ~Singleton()
        {
          Destroy();
        }

        static void Destroy()
        {
          if (s_Instance)
          {
            delete s_Instance;
            s_Instance = nullptr;
          }
        }

        static C& Get()
        {
          if (!s_Instance)
          {
            s_Instance = new C();
          }

          return *s_Instance;
        }

        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

      protected:

        static inline C* s_Instance;

        Singleton() noexcept = default;
    };
}