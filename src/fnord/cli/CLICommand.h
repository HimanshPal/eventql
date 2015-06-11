/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _FNORD_CLI_CLICOMMAND_H
#define _FNORD_CLI_CLICOMMAND_H
#include <string>
#include <vector>
#include <fnord-base/exception.h>
#include <fnord-base/stdtypes.h>
#include <fnord-base/cli/flagparser.h>

namespace fnord {
namespace cli {

class CLICommand : public RefCounted {
public:
  typedef Function<void (const FlagParser& flags)> CallFnType;

  CLICommand(const String& command);

  void onCall(CallFnType fn);
  void call(const Vector<String>& argv);
  FlagParser& flags();

protected:
  FlagParser flags_;
  CallFnType on_call_;
};

}
}
#endif
