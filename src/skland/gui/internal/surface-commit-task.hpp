/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SKLAND_GUI_INTERNAL_SURFACE_COMMIT_TASK_HPP_
#define SKLAND_GUI_INTERNAL_SURFACE_COMMIT_TASK_HPP_

#include <skland/gui/surface.hpp>
#include <skland/gui/task.hpp>

namespace skland {

SKLAND_NO_EXPORT struct Surface::CommitTask : public Task {
  CommitTask(const CommitTask &) = delete;
  CommitTask &operator=(const CommitTask &) = delete;

  CommitTask(Surface *surface)
      : Task(), surface(surface) {}

  virtual ~CommitTask() {}

  virtual void Run() const;

  Surface *surface;
};

}

#endif // SKLAND_GUI_INTERNAL_SURFACE_COMMIT_TASK_HPP_
