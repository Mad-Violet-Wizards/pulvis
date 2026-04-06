#pragma once

#include "FastFunction.hpp"
#include "Signal.hpp"

namespace pulvis::tl
{
	template<class Cx, typename Rx, typename... Ax,
		class Cy, typename Ry, typename... Ay>
	void Connect(Cx* _instance_x, Rx(Cx::* _function_x)(Ax...), Cy* _instance_y, Ry(Cy::* _function_y)(Ay...))
	{
		static ConnectionId id = 0;
		g_SigSlotConnections[id].emplace(_instance_x, _function_x, _instance_y, _function_y);
		id++;

	}
}