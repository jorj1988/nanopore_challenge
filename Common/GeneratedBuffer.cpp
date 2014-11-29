#include "GeneratedBuffer.h"
#include "H5Cpp.h"

GeneratedBuffer::GeneratedBuffer()
    : m_revisionCount(0)
  {
  }

H5::DataSpace GeneratedBuffer::createDataSpace()
  {
  hsize_t readdim[] = { BlockElementCount };
  H5::DataSpace readspace( 1, readdim );

  return readspace;
  }

LockedData::LockedData(GeneratedBuffer *buf)
    : scoped_lock(buf->m_mutex),
      m_buffer(buf)
  {
  }

LockedData::~LockedData()
  {
  // Increase the revision count so users of the buffer know its changed.
  ++m_buffer->m_revisionCount;

  // The data has changed, signal any people awaiting this.
  m_buffer->m_condition.notify_all();
  }

GeneratedBuffer::Buffer *LockedData::data()
  {
  return &m_buffer->m_elements;
  }

const GeneratedBuffer::Buffer *LockedData::constData() const
  {
  return &m_buffer->m_elements;
  }
