#pragma once

namespace Vin {

	template<typename DerivedT, typename IdT>
	class Identifiable {
	public:
		using IdType = IdT;
	public:
		Identifiable() {
			static IdT lastId{ 0 };
			m_Id = ++lastId;
		}

		IdT GetId() {
			return m_Id;
		}
	private:
		IdT m_Id{ 0 };
	};

}
