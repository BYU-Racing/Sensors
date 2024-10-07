#include "SensorData.h"

SensorData::SensorData(const uint32_t id, const size_t msgCount)
{
    this->id = id;
    this->msgCount = msgCount;
    msgs = new CAN_message_t[msgCount];
}
size_t SensorData::getMsgCount() const { return msgCount; }
CAN_message_t* SensorData::getMsgs() const { return msgs; }
CAN_message_t SensorData::newMsg(const uint8_t* buf, const size_t len) const
{
    CAN_message_t msg;
    msg.id = id;
    const size_t maxLen = len > 8 ? 8 : len; // Len can't exceed 8
    msg.len = maxLen;
    for (size_t i = 0; i < maxLen; i++)
    {
        msg.buf[i] = buf[i];
    }
    return msg;
}
bool SensorData::setMsg(const uint8_t* buf, const size_t len, const size_t msgIndex)
{
    if (!buf || msgIndex >= msgCount) { return false; }
    const CAN_message_t msg = newMsg(buf, len);
    msgs[msgIndex] = msg;
    return true;
}
