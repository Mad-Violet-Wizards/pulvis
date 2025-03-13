#pragma once

namespace engine::ecs
{
	class IComponent;

	class CEntity
	{
		public:

			CEntity(CEntity* _parent);
			~CEntity();


			template<class C>
			void CreateComponent(C* _component);


			void AttachChild(CEntity* _child);

		private:

			CEntity* m_Parent;
			std::vector<CEntity*> m_Children;
			std::vector<core::UUID> m_ComponentIDs;
	};
}