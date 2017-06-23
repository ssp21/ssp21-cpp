
#ifndef SSP21_ICOLLECTION_H
#define SSP21_ICOLLECTION_H

#include <cstdint>

namespace ssp21
{
	/**
	*
	* A unmodifiable collection of some abstract type T
	*
	*/
	template <class T>
	class ICollection
	{

	public:

		virtual uint32_t count() const = 0;		

		virtual T const* get(uint32_t i) const = 0;

		bool is_empty() const { return this->count() == 0; }

		bool is_not_empty() const { return this->count() != 0; }

		template <class Action>
		void foreach(const Action& action) const
		{
			for (uint32_t i = 0; i < count(); ++i)
			{
				action(*this->get(i));
			}
		}
	};
}

#endif
