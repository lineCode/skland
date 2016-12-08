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

#ifndef SKLAND_GUI_INTERNAL_TASK_NODE_HPP_
#define SKLAND_GUI_INTERNAL_TASK_NODE_HPP_

namespace skland {
namespace gui {

class TaskNode {

  TaskNode(const TaskNode &) = delete;
  TaskNode &operator=(const TaskNode &) = delete;

 public:

  TaskNode()
      : previous_(nullptr), next_(nullptr) {}

  ~TaskNode();

  bool IsLinked() const {
    return (nullptr != previous_) || (nullptr != next_);
  }

  void AddNext(TaskNode *other);

  void AddPrevious(TaskNode *other);

  void Unlink();

  TaskNode *previous() const {
    return previous_;
  }

  TaskNode *next() const {
    return next_;
  }

 private:

  TaskNode *previous_;
  TaskNode *next_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_DETAIL_TASK_NODE_HPP_
