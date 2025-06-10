#include "ToolSettings.hpp"

namespace harmoniq {

ToolSettings *ToolSettings::m_instance = nullptr;

ToolSettings *ToolSettings::instance()
{
    if (!m_instance) {
        m_instance = new ToolSettings();
    }
    return m_instance;
}

} // namespace harmoniq
