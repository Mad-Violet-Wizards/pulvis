#pragma once

#include "ISystem.hpp"
#include "ComponentTypeId.hpp"

#include <tuple>
#include <utility>

namespace pulvis::ecs
{
  template<typename TOwnedComponent,
    typename TReads = std::tuple<>,
    typename TWrites = std::tuple<TOwnedComponent>>
    class CSystemBase : public ISystem
    {
      public:
      
        CSystemBase(std::string _name, ESystemPhase _phase, bool _thread_safe = true)
          : m_Name(std::move(_name))
          , m_Phase(_phase)
          , m_ThreadSafe(_thread_safe)
        {
          FillBitset<TReads>(m_Reads);
          FillBitset<TWrites>(m_Writes);
          m_Writes.set(GetComponentId<TOwnedComponent>());
        }

        [[nodiscard]] ESystemPhase Phase() const override { return m_Phase; }
        [[nodiscard]] const std::string& Name() const override { return m_Name; }
        [[nodiscard]] const std::bitset<MAX_COMPONENT_TYPES>& Reads() const override { return m_Reads; }
        [[nodiscard]] const std::bitset<MAX_COMPONENT_TYPES>& Writes() const override { return m_Writes; }
        [[nodiscard]] bool IsThreadSafe() const override { return m_ThreadSafe; }

      private:

        template<typename TTuple, size_t... Is>
        static void FillBitsetImpl(std::bitset<MAX_COMPONENT_TYPES>& _bitset, std::index_sequence<Is...>)
        {
          (_bitset.set(GetComponentId<std::tuple_element_t<Is, TTuple>>()), ...);
        }

        template<typename TTuple>
        static void FillBitset(std::bitset<MAX_COMPONENT_TYPES>& _bs)
        {
          FillBitsetImpl<TTuple>(_bs, std::make_index_sequence<std::tuple_size_v<TTuple>>{});
        }

      private:

        std::string m_Name;
				ESystemPhase m_Phase;
        bool m_ThreadSafe;
				std::bitset<MAX_COMPONENT_TYPES> m_Reads;
        std::bitset<MAX_COMPONENT_TYPES> m_Writes;
    };
 }
