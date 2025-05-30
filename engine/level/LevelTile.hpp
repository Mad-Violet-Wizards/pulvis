namespace engine::level
{
	class CLevelTile
	{
		public:

			CLevelTile(unsigned int _x, unsigned int _y, unsigned int _script_tile_id);

			const math::Vector2<unsigned int>& GetPosition() const { return m_Position; }
			unsigned int GetScriptTileID() const { return m_ScriptTileID; }

			unsigned int GetX() const { return m_Position[0]; }
			unsigned int GetY() const { return m_Position[1]; }

		private:
			
			math::Vector2<unsigned int> m_Position;
			unsigned int m_ScriptTileID;
	};

	class CLevelTileLayer
	{
		public:

			CLevelTileLayer(unsigned int _index);

			void AddTile(const CLevelTile& _tile) { m_Tiles.push_back(_tile); }
			const std::vector<CLevelTile>& GetTiles() const { return m_Tiles; }

		private:

			unsigned int m_Index;
			std::vector<CLevelTile> m_Tiles;
	};
}