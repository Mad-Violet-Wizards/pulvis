#include "SignalCache.hpp"

#include <algorithm>

namespace pulvis::ecs
{
	//////////////////////////////////////////////////////////////////////////
	size_t CSignalCache::SSignalKeyHasher::operator()(const SSignalKey& _key) const noexcept
	{
		size_t h = static_cast<size_t>(_key.Source.Index);
		h ^= static_cast<size_t>(_key.Source.Generation) * 0x9E3779B97F4A7C15ull + (h << 6) + (h >> 2);
		h ^= static_cast<size_t>(_key.SourceComponent) * 0x9E3779B97F4A7C15ull + (h << 6) + (h >> 2);
		h ^= static_cast<size_t>(_key.Signal) * 0x9E3779B97F4A7C15ull + (h << 6) + (h >> 2);
		return h;
	}

	CSignalCache::CSignalCache() = default;
	CSignalCache::~CSignalCache() = default;

	SSignalConnection CSignalCache::Connect(
		SEntityHandle   _source,
		component_id_t  _source_component,
		signal_id_t     _signal,
		slot_callback_t _slot,
		SEntityHandle   _slot_owner,
		component_id_t  _slot_owner_component)
	{
		std::unique_lock lock(m_Mutex);

		const SSignalKey key{ _source, _source_component, _signal };
		auto& slots_ptr = m_Signals[key];
		if (!slots_ptr)
		{
			slots_ptr = std::make_unique<SSignalSlots>();
		}

		const slot_id_t slot_id = ++m_NextSlotId;
		slots_ptr->Slots.push_back(SSlotEntry{
			slot_id,
			std::move(_slot),
			_slot_owner,
			_slot_owner_component,
			false });

		return SSignalConnection{ _source, _source_component, _signal, slot_id };
	}

	SSignalConnection CSignalCache::ConnectByName(
		SEntityHandle   _source,
		component_id_t  _source_component,
		std::string_view _signal_name,
		slot_callback_t _slot,
		SEntityHandle   _slot_owner,
		component_id_t  _slot_owner_component)
	{
		return Connect(_source, _source_component, HashSignalName(_signal_name),
			std::move(_slot), _slot_owner, _slot_owner_component);
	}

	void CSignalCache::Emit(SEntityHandle _source, component_id_t _source_component, signal_id_t _signal, const SSignalArgs& _args)
	{
		SSignalSlots* slots = nullptr;
		{
			std::shared_lock lock(m_Mutex);
			const SSignalKey key{ _source, _source_component, _signal };
			auto it = m_Signals.find(key);
			if (it == m_Signals.end())
			{
				return;
			}
			slots = it->second.get();
		}

		++slots->EmitDepth;
		const size_t snapshot_size = slots->Slots.size();
		for (size_t i = 0; i < snapshot_size; ++i)
		{
			auto& s = slots->Slots[i];
			if (!s.Tombstoned && s.Callback)
			{
				s.Callback(_args);
			}
		}
		--slots->EmitDepth;

		if (slots->EmitDepth == 0)
		{
			std::unique_lock lock(m_Mutex);
			CompactIfIdle(*slots);
		}
	}

	void CSignalCache::EmitByName(SEntityHandle _source, component_id_t _source_component, std::string_view _signal_name, const SSignalArgs& _args)
	{
		Emit(_source, _source_component, HashSignalName(_signal_name), _args);
	}

	void CSignalCache::Disconnect(const SSignalConnection& _connection)
	{
		if (!_connection.IsValid())
		{
			return;
		}

		std::unique_lock lock(m_Mutex);

		const SSignalKey key{ _connection.Source, _connection.SourceComponent, _connection.Signal };
		auto it = m_Signals.find(key);
		if (it == m_Signals.end())
		{
			return;
		}

		auto& slots = *it->second;
		for (auto& s : slots.Slots)
		{
			if (s.Id == _connection.Slot && !s.Tombstoned)
			{
				s.Tombstoned = true;
				s.Callback = nullptr;
				++slots.TombstoneCount;
				break;
			}
		}
		CompactIfIdle(slots);
	}

	void CSignalCache::OnEntityDestroyed(SEntityHandle _entity)
	{
		DisconnectAllSourcedBy(_entity);
		DisconnectAllOwnedBy(_entity);
	}

	void CSignalCache::DisconnectAllOwnedBy(SEntityHandle _owner)
	{
		std::unique_lock lock(m_Mutex);
		for (auto& [_, slots_ptr] : m_Signals)
		{
			auto& slots = *slots_ptr;
			for (auto& s : slots.Slots)
			{
				if (!s.Tombstoned
					&& s.Owner.Index == _owner.Index
					&& s.Owner.Generation == _owner.Generation)
				{
					s.Tombstoned = true;
					s.Callback = nullptr;
					++slots.TombstoneCount;
				}
			}
			CompactIfIdle(slots);
		}
	}

	void CSignalCache::DisconnectAllSourcedBy(SEntityHandle _source)
	{
		std::unique_lock lock(m_Mutex);
		for (auto it = m_Signals.begin(); it != m_Signals.end(); )
		{
			if (it->first.Source.Index == _source.Index
				&& it->first.Source.Generation == _source.Generation)
			{
				it = m_Signals.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void CSignalCache::Clear()
	{
		std::unique_lock lock(m_Mutex);
		m_Signals.clear();
	}

	void CSignalCache::CompactIfIdle(SSignalSlots& _slots)
	{
		if (_slots.EmitDepth != 0 || _slots.TombstoneCount == 0)
		{
			return;
		}
		_slots.Slots.erase(
			std::remove_if(_slots.Slots.begin(), _slots.Slots.end(),
				[](const SSlotEntry& s) { return s.Tombstoned; }),
			_slots.Slots.end());
		_slots.TombstoneCount = 0;
	}
} // namespace pulvis::ecs