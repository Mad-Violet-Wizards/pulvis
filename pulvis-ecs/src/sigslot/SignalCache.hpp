#pragma once

#include "EntityHandle.hpp"
#include "ComponentTraits.hpp"

#include <any>
#include <cstdint>
#include <functional>
#include <limits>
#include <memory>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace pulvis::ecs
{
	//////////////////////////////////////////////////////////////////////////
	using signal_id_t = uint32_t;
	using slot_id_t = uint64_t;

	static constexpr signal_id_t INVALID_SIGNAL_ID = std::numeric_limits<signal_id_t>::max();
	static constexpr slot_id_t   INVALID_SLOT_ID = std::numeric_limits<slot_id_t>::max();

	[[nodiscard]] signal_id_t HashSignalName(std::string_view _name) noexcept;

	//////////////////////////////////////////////////////////////////////////
	struct SSignalConnection final
	{
		SEntityHandle  Source;
		component_id_t SourceComponent = INVALID_COMPONENT_ID;
		signal_id_t    Signal = INVALID_SIGNAL_ID;
		slot_id_t      Slot = INVALID_SLOT_ID;

		[[nodiscard]] bool IsValid() const noexcept { return Slot != INVALID_SLOT_ID; }
	};

	//////////////////////////////////////////////////////////////////////////
	struct SSignalArgs final
	{
		std::vector<std::any> Values;

		template<typename T>
		[[nodiscard]] const T& Get(size_t _index) const { return std::any_cast<const T&>(Values[_index]); }

		[[nodiscard]] size_t Count() const noexcept { return Values.size(); }
	};

	//////////////////////////////////////////////////////////////////////////
	class CSignalCache final
	{
	public:

		using slot_callback_t = std::function<void(const SSignalArgs&)>;

		CSignalCache();
		~CSignalCache();

		CSignalCache(const CSignalCache&) = delete;
		CSignalCache& operator=(const CSignalCache&) = delete;

		[[nodiscard]] SSignalConnection Connect(
			SEntityHandle  _source,
			component_id_t _source_component,
			signal_id_t    _signal,
			slot_callback_t _slot,
			SEntityHandle  _slot_owner = {},
			component_id_t _slot_owner_component = INVALID_COMPONENT_ID);

		[[nodiscard]] SSignalConnection ConnectByName(
			SEntityHandle  _source,
			component_id_t _source_component,
			std::string_view _signal_name,
			slot_callback_t _slot,
			SEntityHandle  _slot_owner = {},
			component_id_t _slot_owner_component = INVALID_COMPONENT_ID);

		template<typename... Args>
		[[nodiscard]] SSignalConnection ConnectTyped(
			SEntityHandle  _source,
			component_id_t _source_component,
			std::string_view _signal_name,
			std::function<void(Args...)> _slot,
			SEntityHandle  _slot_owner = {},
			component_id_t _slot_owner_component = INVALID_COMPONENT_ID)
		{
			auto wrapper = [fn = std::move(_slot)](const SSignalArgs& _args)
				{
					UnpackAndInvoke<Args...>(fn, _args, std::index_sequence_for<Args...>{});
				};
			return ConnectByName(_source, _source_component, _signal_name, std::move(wrapper), _slot_owner, _slot_owner_component);
		}

		void Emit(SEntityHandle _source, component_id_t _source_component, signal_id_t _signal, const SSignalArgs& _args);
		void EmitByName(SEntityHandle _source, component_id_t _source_component, std::string_view _signal_name, const SSignalArgs& _args);

		template<typename... Args>
		void EmitTyped(SEntityHandle _source, component_id_t _source_component, std::string_view _signal_name, Args&&... _values)
		{
			SSignalArgs pack;
			pack.Values.reserve(sizeof...(Args));
			(pack.Values.emplace_back(std::forward<Args>(_values)), ...);
			EmitByName(_source, _source_component, _signal_name, pack);
		}

		void Disconnect(const SSignalConnection& _connection);

		void OnEntityDestroyed(SEntityHandle _entity);
		void DisconnectAllOwnedBy(SEntityHandle _owner);
		void DisconnectAllSourcedBy(SEntityHandle _source);

		void Clear();

	private:

		template<typename... Args, size_t... I>
		static void UnpackAndInvoke(const std::function<void(Args...)>& _fn, const SSignalArgs& _args, std::index_sequence<I...>)
		{
			_fn(std::any_cast<Args>(_args.Values[I])...);
		}

		struct SSignalKey
		{
			SEntityHandle  Source;
			component_id_t SourceComponent;
			signal_id_t    Signal;

			[[nodiscard]] bool operator==(const SSignalKey& _other) const noexcept
			{
				return Source.Index == _other.Source.Index
					&& Source.Generation == _other.Source.Generation
					&& SourceComponent == _other.SourceComponent
					&& Signal == _other.Signal;
			}
		};

		struct SSignalKeyHasher
		{
			[[nodiscard]] size_t operator()(const SSignalKey& _key) const noexcept;
		};

		struct SSlotEntry
		{
			slot_id_t       Id = INVALID_SLOT_ID;
			slot_callback_t Callback;
			SEntityHandle   Owner;
			component_id_t  OwnerComponent = INVALID_COMPONENT_ID;
			bool            Tombstoned = false;
		};

		struct SSignalSlots
		{
			std::vector<SSlotEntry> Slots;
			size_t                  TombstoneCount = 0;
			int                     EmitDepth = 0;
		};

		void CompactIfIdle(SSignalSlots& _slots);

	private:

		mutable std::shared_mutex                                                       m_Mutex;
		std::unordered_map<SSignalKey, std::unique_ptr<SSignalSlots>, SSignalKeyHasher> m_Signals;
		slot_id_t                                                                       m_NextSlotId = 0;
	};
} // namespace pulvis::ecs