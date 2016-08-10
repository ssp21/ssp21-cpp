
#include "ssp21/Key.h"

#include <assert.h>

namespace ssp21
{		

Key::Key() : m_length(0)
{}
	
openpal::RSlice Key::GetKey()
{
	return m_buffer.ToRSlice().Take(m_length);
}

openpal::WSlice Key::GetWriteDest()
{
	return m_buffer.GetWSlice();
}

void Key::SetLength(uint32_t length)
{
	assert(length <= m_buffer.Size());
	m_length = length;
}

		
}

