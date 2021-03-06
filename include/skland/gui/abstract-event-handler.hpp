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

#ifndef SKLAND_GUI_ABSTRACT_EVENT_HANDLER_HPP_
#define SKLAND_GUI_ABSTRACT_EVENT_HANDLER_HPP_

#include "../core/defines.hpp"
#include "../core/sigcxx.hpp"

#include "task.hpp"
#include <memory>

namespace skland {

class MouseEvent;
class KeyEvent;

class Surface;
class AbstractView;
class Context;

/**
 * @ingroup gui
 * @brief The base class to handle events and interact with surface
 */
SKLAND_EXPORT class AbstractEventHandler : public Trackable {

  friend class Input;
  friend class Application;
  friend class Display;

 public:

  struct RedrawTask;
  struct EventTask;

  class MouseTaskIterator;
  class RedrawTaskIterator;

  AbstractEventHandler();

  virtual ~AbstractEventHandler();

 protected:

  virtual void OnMouseEnter(MouseEvent *event) = 0;

  virtual void OnMouseLeave(MouseEvent *event) = 0;

  virtual void OnMouseMove(MouseEvent *event) = 0;

  virtual void OnMouseButton(MouseEvent *event) = 0;

  virtual void OnKeyboardKey(KeyEvent *event) = 0;

  /**
   * @brief A view request an update
   * @param view A view in hierachy or null to update this object
   */
  virtual void OnUpdate(AbstractView *view) = 0;

  /**
   * @brief Get surface for the given view
   * @param view A view in hierachy or null to get shell surface for this object
   * @return A pointer to a surface or nullptr
   */
  virtual Surface *GetSurface(const AbstractView *view) const = 0;

  virtual void OnDraw(const Context *context) = 0;

  static void Damage(AbstractEventHandler *object, int surface_x, int surface_y, int width, int height);

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  /**
 * @brief Initialize the idle task list
 */
  static void InitializeRedrawTaskList();

  /**
   * @brief Destroy the redraw task list
   */
  static void ClearRedrawTaskList();

  static Task kRedrawTaskHead;
  static Task kRedrawTaskTail;

};

}

#endif // SKLAND_GUI_ABSTRACT_EVENT_HANDLER_HPP_
