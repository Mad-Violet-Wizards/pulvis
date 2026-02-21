#pragma once

#include "FastFunction.hpp"
#include "Rect.hpp"

#include <array>
#include <vector>

namespace pulvis::tl
{
	template<typename T, unsigned int MAX_DEPTH = 8, unsigned int MAX_ELEMENTS = 8>
	class Quadtree
	{
	public:

		Quadtree(const Rect<float>& _bounds, FastFunction&& _get_bounds_fn)
			: m_GetBounds(std::move(_get_bounds_fn))
		{
			m_Nodes[0].Bounds = _bounds;
			m_Nodes[0].Active = true;
			m_NodesCount = 1;
			m_Elements.reserve(MAX_ELEMENTS * 4);
		}

		void Insert(const T& _element)
		{
			Rect<float> bounds = m_GetBounds.Invoke<Rect<float>>(_element);
			InsertAt(0, 0, _element, bounds);
		}

		void Query(const Rect<float>& _area, std::vector<T>& _out_results) const
		{
			QueryAt(0, _area, _out_results);
		}

		void Clear()
		{
			for (unsigned int i = 0; i < m_NodesCount; ++i)
			{
				m_Nodes[i].ElementStart = INVALID_INDEX;
				m_Nodes[i].ElementCount = 0;
				m_Nodes[i].Active = false;
				m_Nodes[i].Divided = false;
				m_Nodes[i].FirstChildren = INVALID_INDEX;
			}

			m_Elements.clear();
			m_Nodes[0].Active = true;
			m_NodesCount = 1;
		}

		void Rebuild(const std::vector<T>& _elements)
		{
			Rect<float> root_bounds = m_Nodes[0].Bounds;
			Clear();
			m_Nodes[0].Bounds = root_bounds;

			m_Elements.reserve(_elements.size());
			for (const T& element : _elements)
				Insert(element);
		}

		unsigned int Count() const
		{
			return static_cast<unsigned int>(m_Elements.size());
		}

		unsigned int NodeCount() const
		{
			return m_NodesCount;
		}

	private:

		static_assert(MAX_DEPTH <= 8, "Quadtree: Max depth must be less than or equal to 8 to prevent excessive memory usage.");
		static constexpr unsigned int MAX_NODES = ((1u << (2u * MAX_DEPTH)) - 1u) / 3u;
		static constexpr unsigned int INVALID_INDEX = std::numeric_limits<unsigned int>::max();

		struct SElementEntry
		{
			T Element;
			Rect<float> Bounds;
			unsigned int NodeIndex = INVALID_INDEX;
			unsigned int NextInNode = INVALID_INDEX;
		};

		struct SNode
		{
			Rect<float> Bounds;
			unsigned int ElementStart = INVALID_INDEX;
			unsigned int ElementCount = 0;
			unsigned int FirstChildren = INVALID_INDEX;
			bool Active = false;
			bool Divided = false;
		};

	private:

		void InsertAt(unsigned int _node_index, unsigned int _depth, const T& _element, const Rect<float>& _bounds)
		{
			SNode& node = m_Nodes[_node_index];

			if (!node.Bounds.Intersect(_bounds))
				return;

			if (!node.Divided && (node.ElementCount < MAX_ELEMENTS || _depth >= MAX_DEPTH))
			{
				const unsigned int idx = static_cast<unsigned int>(m_Elements.size());
				m_Elements.emplace_back(_element, _bounds, _node_index, INVALID_INDEX);
				LinkElementToNode(idx, _node_index);
				return;
			}

			if (!node.Divided)
			{
				if (!Subdivide(_node_index, _depth))
				{
					const unsigned int idx = static_cast<unsigned int>(m_Elements.size());
					m_Elements.emplace_back(_element, _bounds, _node_index, INVALID_INDEX);
					LinkElementToNode(idx, _node_index);
					return;
				}
			}

			for (unsigned int child_id = 0; child_id < 4; ++child_id)
			{
				unsigned int child_index = node.FirstChildren + child_id;
				if (m_Nodes[child_index].Bounds.ContainsRect(_bounds))
				{
					InsertAt(child_index, _depth + 1, _element, _bounds);
					return;
				}
			}

			const unsigned int idx = static_cast<unsigned int>(m_Elements.size());
			m_Elements.emplace_back(_element, _bounds, _node_index, INVALID_INDEX);
			LinkElementToNode(idx, _node_index);
		}

		void QueryAt(unsigned int _node_index, const Rect<float>& _area, std::vector<T>& _out_results) const
		{
			const SNode& node = m_Nodes[_node_index];

			if (!node.Active || !node.Bounds.Intersect(_area))
			{
				return;
			}

			unsigned int current = node.ElementStart;
			while (current != INVALID_INDEX)
			{
				const SElementEntry& entry = m_Elements[current];
				if (_area.Intersect(entry.Bounds))
				{
					_out_results.push_back(entry.Element);
				}
				current = entry.NextInNode;
			}

			if (node.Divided)
			{
				for (unsigned int i = 0; i < 4; ++i)
				{
					QueryAt(node.FirstChildren + i, _area, _out_results);
				}
			}
		}

		bool Subdivide(unsigned int _node_index, unsigned int _depth)
		{
			SNode& node = m_Nodes[_node_index];

			if (m_NodesCount + 4 > MAX_NODES)
				return false;

			const float half_w = node.Bounds.Size.x * 0.5f;
			const float half_h = node.Bounds.Size.y * 0.5f;
			const float x = node.Bounds.Position.x;
			const float y = node.Bounds.Position.y;

			node.FirstChildren = m_NodesCount;
			m_Nodes[m_NodesCount + 0] = { { x,          y,          half_w, half_h }, INVALID_INDEX, 0, INVALID_INDEX, true, false };
			m_Nodes[m_NodesCount + 1] = { { x + half_w, y,          half_w, half_h }, INVALID_INDEX, 0, INVALID_INDEX, true, false };
			m_Nodes[m_NodesCount + 2] = { { x,          y + half_h, half_w, half_h }, INVALID_INDEX, 0, INVALID_INDEX, true, false };
			m_Nodes[m_NodesCount + 3] = { { x + half_w, y + half_h, half_w, half_h }, INVALID_INDEX, 0, INVALID_INDEX, true, false };

			m_NodesCount += 4;
			node.Divided = true;

			unsigned int current = node.ElementStart;
			node.ElementStart = INVALID_INDEX;
			node.ElementCount = 0;

			while (current != INVALID_INDEX)
			{
				const unsigned int next = m_Elements[current].NextInNode;
				m_Elements[current].NextInNode = INVALID_INDEX;

				bool moved = false;
				for (unsigned int child_id = 0; child_id < 4; ++child_id)
				{
					unsigned int child_index = node.FirstChildren + child_id;
					if (m_Nodes[child_index].Bounds.ContainsRect(m_Elements[current].Bounds))
					{
						LinkElementToNode(current, child_index);
						moved = true;
						break;
					}
				}

				if (!moved)
				{
					LinkElementToNode(current, _node_index);
				}

				current = next;
			}

			return true;
		}

		void LinkElementToNode(unsigned int _element_index, unsigned int _node_index)
		{
			SNode& node = m_Nodes[_node_index];
			m_Elements[_element_index].NodeIndex = _node_index;
			m_Elements[_element_index].NextInNode = node.ElementStart;
			node.ElementStart = _element_index;
			node.ElementCount++;
		}

	private:

		std::array<SNode, MAX_NODES> m_Nodes;
		std::vector<SElementEntry> m_Elements;
		unsigned int m_NodesCount = 0;
		FastFunction m_GetBounds;
	};
}