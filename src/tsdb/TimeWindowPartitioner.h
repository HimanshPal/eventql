/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <fnord/stdtypes.h>
#include <fnord/option.h>
#include <fnord/datetime.h>
#include <fnord/duration.h>
#include <fnord/SHA1.h>

using namespace fnord;

namespace tsdb {

class TimeWindowPartitioner : public RefCounted {
public:

  static SHA1Hash partitionKeyFor(
      const String& stream_key,
      DateTime time,
      Duration window_size);

  static Vector<SHA1Hash> partitionKeysFor(
      const String& stream_key,
      DateTime from,
      DateTime until,
      Duration window_size);

};

}
