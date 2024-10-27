#ifndef QUEUE_H
#define QUEUE_H

#include "record/subrecord/firmware/firmware.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <record/subrecord/sr_command_data/sr_command_data.h>
#include <record/subrecord/sr_record_response/sr_record_response.h>
#include <record/subrecord/sr_result_code/sr_result_code.h>
#include <transport/transport.h>
#include <variant>

struct Done
{
};

using queue_type = std::variant<
    egts::v1::transport::Packet,
    egts::v1::record::subrecord::SRRecordResponse,
    egts::v1::record::subrecord::SrResultCode,
    egts::v1::record::subrecord::SrCommandData,
    egts::v1::record::subrecord::SrPartData,
    egts::v1::record::subrecord::SrFullData,
    Done>;


class MyQueue
{
  private:
    std::mutex m;
    std::queue<queue_type> m_queue;
    std::condition_variable cv;

  public:
    MyQueue() = default;

    MyQueue(const MyQueue &) = delete; // !mutex
    MyQueue &
    operator=(const MyQueue &) = delete; // !mutex

    queue_type
    pop() noexcept;

    void
    push(queue_type &&) noexcept;
};

#endif